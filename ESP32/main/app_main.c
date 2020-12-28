/* SPI Slave example, receiver (uses SPI Slave driver to communicate with sender)
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/igmp.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "soc/rtc_periph.h"
#include "driver/spi_slave.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "xtensa/core-macros.h"


/*
SPI receiver (slave) example.
This example is supposed to work together with the SPI sender. It uses the standard SPI pins (MISO, MOSI, SCLK, CS) to
transmit data over in a full-duplex fashion, that is, while the master puts data on the MOSI pin, the slave puts its own
data on the MISO pin.
This example uses one extra pin: GPIO_HANDSHAKE is used as a handshake pin. After a transmission has been set up and we're
ready to send/receive data, this code uses a callback to set the handshake pin high. The sender will detect this and start
sending a transaction. As soon as the transaction is done, the line gets set low again.
*/

/*
Pins in use. The SPI Master can use the GPIO mux, so feel free to change these if needed.
*/
#define GPIO_HANDSHAKE 2
#define GPIO_MOSI 12
#define GPIO_MISO 13
#define GPIO_SCLK 15
#define GPIO_CS 14

#ifdef CONFIG_IDF_TARGET_ESP32
#define RCV_HOST    HSPI_HOST
#define DMA_CHAN    2

#elif defined CONFIG_IDF_TARGET_ESP32S2
#define RCV_HOST    SPI2_HOST
#define DMA_CHAN    RCV_HOST

#endif

#define packetsize      2048 //max 4092
#define numberofruns    1

//Set CPU speed to 240Mhz
//Set Flash bus speed to 80Mhz !Required as sharing cache with PSRAM!
//Set PSRAM bus speed to 80Mhz

//Called after a transaction is queued and ready for pickup by master. We use this to set the handshake line high.
void my_post_setup_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TS_REG, (1<<GPIO_HANDSHAKE));
}

//Called after transaction is sent/received. We use this to set the handshake line low.
void my_post_trans_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TC_REG, (1<<GPIO_HANDSHAKE));
}

//WORD_ALIGNED_ATTR uint8_t rdatabuffer[numberofruns*packetsize]; //databuffer should be word aligned but ok works till now, pbb got lucky
WORD_ALIGNED_ATTR uint8_t* databuffer;
//WORD_ALIGNED_ATTR char recvbuf[packetsize];/
char recvbuf[packetsize];

static void SPItoPSRAM (void* arg)
{
    int n=0;
    esp_err_t ret;
    int64_t bcount=0;
    int64_t acount=0;
    int64_t dcount=0;
    //memset(recvbuf,0,packetsize);
    //databuffer = (uint8_t *) malloc(numberofruns*packetsize); 
    databuffer = (uint8_t *) heap_caps_malloc(numberofruns*packetsize,MALLOC_CAP_DMA); 
    //databuffer = rdatabuffer;
    //Configuration for the SPI bus
    spi_bus_config_t buscfg={
        .mosi_io_num=GPIO_MOSI,
        .miso_io_num=GPIO_MISO,
        .sclk_io_num=GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    //Configuration for the SPI slave interface
    spi_slave_interface_config_t slvcfg={
        .mode=0,
        .spics_io_num=GPIO_CS,
        .queue_size=8,
        .flags=ESP_INTR_FLAG_LEVEL3,
        .post_setup_cb=my_post_setup_cb,
        .post_trans_cb=my_post_trans_cb
    };

    //Configuration for the handshake line
    gpio_config_t io_conf={
        .intr_type=GPIO_INTR_DISABLE,
        .mode=GPIO_MODE_OUTPUT,
        .pin_bit_mask=(1<<GPIO_HANDSHAKE)
    };

    //Configure handshake line as output
    gpio_config(&io_conf);
    //Enable pull-ups on SPI lines so we don't detect rogue pulses when no master is connected.
    gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_SCLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_CS, GPIO_PULLUP_ONLY);

    //Setting interupt before initiatlization
    SPI2.slave.rd_buf_inten=1;

    //printf("dma status %d\n",SPI2.dma_status.rx_en);

    //Initialize SPI slave interface
    ret=spi_slave_initialize(RCV_HOST, &buscfg, &slvcfg, DMA_CHAN);
    assert(ret==ESP_OK);

    //memset(recvbuf, 0x41, packetsize);
    int z = 0x41;
    for (int i =0; i<packetsize;i++) {
        recvbuf[i] = z;
        if (z==0x50) z = 0x41; else z++;
    }
    spi_slave_transaction_t t[5];
    spi_slave_transaction_t* ret_t;
    for (int i = 0; i<5; i++)
    {
        memset(&t[i], 0, sizeof(t));
        t[i].length=packetsize*8;
        t[i].tx_buffer=recvbuf;
    } 

    printf("init t\n");
    while(1==1){   
        
        bcount = XTHAL_GET_CCOUNT();
        while(n<numberofruns) {

            //Set up a transaction of packetsize bytes to send/receive

            t[n].rx_buffer=&databuffer[packetsize*n]; 
            ret = spi_slave_queue_trans(RCV_HOST, &t[n], portMAX_DELAY);
          
            n++; 

        }
        
        for (int i = 0; i<numberofruns;i++)
        {
            ret = spi_slave_get_trans_result(RCV_HOST, &ret_t, portMAX_DELAY);
        }

        acount = XTHAL_GET_CCOUNT();
        dcount = acount - bcount;
        printf("Ticks passed: %lld \n", dcount);
        float fdcount = (float)(packetsize*numberofruns)/(float)(dcount)*(float)(240);
        printf("MB/s: %f \n", fdcount);
        acount = 0; bcount = 0; 
        vTaskDelay(10);
        printf("Full databuffer dump: \n");
        for (int crun=0; crun < numberofruns; crun++) {
            for (int cbuf=0; cbuf < packetsize;cbuf++) {
                printf("%c",databuffer[cbuf+packetsize*crun]);
            }
            printf("\n");
        }
        memset(databuffer, 0x42,numberofruns*packetsize);
        n=0;
    }
}

//Main application
void app_main(void)
{
    TaskHandle_t xHandle=NULL;
    
    xTaskCreatePinnedToCore(&SPItoPSRAM,"NAME", 3584,NULL,configMAX_PRIORITIES-1,&xHandle,0);

}
