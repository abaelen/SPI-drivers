// notes: (*ptr).xxxx.xxxx equivalent ptr->xxxx.xxxx

#include "a_SPI.h"
#include "a_GPIO.h"
#include "a_DMA.h"
#include "a_VC.h"

#define SizeOfRxDatabuffer    2048
#define SizeOfTxDatabuffer    2048

#define SizeOfRxMemory         10240
#define SizeOfTxMemory         10240
#define SizeOfCBRxMemory       10240
#define SizeOfCBTxMemory       10240

int main()
{
    bcm2835_peripheral  perip_SPI0;
    bcm2835_peripheral  perip_GPIO;
    bcm2835_peripheral  perip_DMA;
    volatile aSPI0  *pSPI0;
    volatile aGPIO  *pGPIO;
    volatile aDMA   *pDMA;

//    __attribute__ ((aligned (256))) aCB CBRx;
//    __attribute__ ((aligned (256))) aCB CBTx;

    __attribute__ ((aligned (256))) aCB* pCBRx;
    __attribute__ ((aligned (256))) aCB* pCBTx;


    aVC     VC_Rx;
    aVC     VC_Tx;
    aVC     VC_CBRx;
    aVC     VC_CBTx;
    union
    {
        volatile uint32_t* volatile b32;
        volatile uint8_t* volatile b8;
    }pRxDatabuffer;
    union
    {
        volatile uint32_t* volatile b32;
        volatile uint8_t* volatile b8;
    }pTxDatabuffer;

    /************* ACQUIRE VC PHYSICAL MEMORY BLOCK *************/
    /*It is easier to acquire a VC physical memory block using the VC mailbox system then a 
    linux kernel space memory block. A linux kernel memory space continguous block requires 
    memory allocation early at boot up. It would require the same memory allocation setup as 
    a linux driver. Not investigated but probably would mean setting up a Kernel Module*/

    // Allocate memory on PAGE_SIZE boundary, return handle
    VC_Rx.fd = open_mbox();
    VC_Rx.h = alloc_vc_mem(VC_Rx.fd,SizeOfRxMemory,(VC_ALLOC_FLAGS) DMA_MEM_FLAGS);
    if (VC_Rx.h <= 0) printf("error allocating memory\n");
    VC_Rx.bus_addr = lock_vc_mem(VC_Rx.fd, VC_Rx.h);
    if (VC_Rx.bus_addr == 0) printf("error locking memory\n");
    VC_Rx.phys_addr = VC_Rx.bus_addr - MEMORY_BUS_PHYSICAL_OFFSET;
    VC_Rx.virt_addr = (uint32_t*) map_segment(VC_Rx.phys_addr,SizeOfRxMemory);
    if (VC_Rx.virt_addr ==0) printf("error obtaining virtual address rx\n");

    // Allocate memory on PAGE_SIZE boundary, return handle
    VC_Tx.fd = open_mbox();
    VC_Tx.h = alloc_vc_mem(VC_Tx.fd,SizeOfTxMemory,(VC_ALLOC_FLAGS) DMA_MEM_FLAGS);
    if (VC_Tx.h <= 0) printf("error allocating memory\n");
    VC_Tx.bus_addr = lock_vc_mem(VC_Tx.fd, VC_Tx.h);
    if (VC_Tx.bus_addr == 0) printf("error locking memory\n");
    VC_Tx.phys_addr = VC_Tx.bus_addr - MEMORY_BUS_PHYSICAL_OFFSET;
    VC_Tx.virt_addr = (uint32_t*)map_segment(VC_Tx.phys_addr,SizeOfTxMemory);
    if (VC_Tx.virt_addr ==0) printf("error obtaining virtual address tx\n");

    printf("Rx memory block bus address: %x\n", VC_Rx.bus_addr);
    printf("Tx memory block bus address: %x\n", VC_Tx.bus_addr);
    printf("Rx memory block phys address: %x\n", VC_Rx.phys_addr);
    printf("Tx memory block phys address: %x\n", VC_Tx.phys_addr);
    printf("Rx memory block virt address: %x\n", VC_Rx.virt_addr);
    printf("Tx memory block virt address: %x\n", VC_Tx.virt_addr);

    pRxDatabuffer.b32 = VC_Rx.virt_addr;
    pTxDatabuffer.b32 = VC_Tx.virt_addr;
    printf("pxxDatabuffer set\n");
    for (int i=0;i<SizeOfTxDatabuffer;i++) pTxDatabuffer.b8[i]=0x00;
    for (int i=0;i<SizeOfRxDatabuffer;i++) pRxDatabuffer.b8[i]=0x56;
    printf("pxxDatabuffer nulled\n");
    // Allocate memory on PAGE_SIZE boundary, return handle
    VC_CBRx.fd = open_mbox();
    VC_CBRx.h = alloc_vc_mem(VC_CBRx.fd,SizeOfCBRxMemory,(VC_ALLOC_FLAGS) DMA_MEM_FLAGS);
    if (VC_CBRx.h <= 0) printf("error allocating memory\n");
    VC_CBRx.bus_addr = lock_vc_mem(VC_CBRx.fd, VC_CBRx.h);
    if (VC_CBRx.bus_addr == 0) printf("error locking memory\n");
    VC_CBRx.phys_addr = VC_CBRx.bus_addr - MEMORY_BUS_PHYSICAL_OFFSET;
    VC_CBRx.virt_addr = (uint32_t*) map_segment(VC_CBRx.phys_addr,SizeOfCBRxMemory);
    if (VC_CBRx.virt_addr ==0) printf("error obtaining virtual address rx\n");

    // Allocate memory on PAGE_SIZE boundary, return handle
    VC_CBTx.fd = open_mbox();
    VC_CBTx.h = alloc_vc_mem(VC_CBTx.fd,SizeOfCBTxMemory,(VC_ALLOC_FLAGS) DMA_MEM_FLAGS);
    if (VC_CBTx.h <= 0) printf("error allocating memory\n");
    VC_CBTx.bus_addr = lock_vc_mem(VC_CBTx.fd, VC_CBTx.h);
    if (VC_CBTx.bus_addr == 0) printf("error locking memory\n");
    VC_CBTx.phys_addr = VC_CBTx.bus_addr - MEMORY_BUS_PHYSICAL_OFFSET;
    VC_CBTx.virt_addr = (uint32_t*)map_segment(VC_CBTx.phys_addr,SizeOfCBTxMemory);
    if (VC_CBTx.virt_addr ==0) printf("error obtaining virtual address tx\n");

    printf("CBRx memory block bus address: %x\n", VC_CBRx.bus_addr);
    printf("CBTx memory block bus address: %x\n", VC_CBTx.bus_addr);
    printf("CBRx memory block phys address: %x\n", VC_CBRx.phys_addr);
    printf("CBTx memory block phys address: %x\n", VC_CBTx.phys_addr);
    printf("CBRx memory block virt address: %x\n", VC_CBRx.virt_addr);
    printf("CBTx memory block virt address: %x\n", VC_CBTx.virt_addr);

    pCBRx = (aCB*) VC_CBRx.virt_addr;
    pCBTx = (aCB*) VC_CBTx.virt_addr;

    printf("CBRx databuffer virt address: %x\n",VC_CBRx.virt_addr);
    printf("CBRx control block virt address: %x\n",pCBRx);

    /************* END OF ACQUIRE VC PHYSICAL MEMORY BLOCK *************/

    /************** MAPPING OF SPI REGISTERS ***********************/
    /*Set peripheral SPI0 structure address to virtual DMA address
    The physical SPI0 address is derived from the bus address  */
    perip_SPI0.addr_p = PHYSICAL_SPI0;
                                        printf("BUS_VIRTUAL_OFFSET: %x\n", PERIPH_BUS_PHYSICAL_OFFSET);
                                        printf("VITUAL_SPI0: %x\n", PHYSICAL_SPI0);
                                        printf("perip SPI0 addr_p: %x\n", perip_SPI0.addr_p);
    /*Map the physical SPI0 address space into virtual address space*/
    if(map_peripheral(&perip_SPI0,SPI0_BLOCK_SIZE) == -1) 
    {
        printf("Failed to map the physical SPI0 registers into the virtual memory space.\n");
        return -1;
    }
    
    /*Map the retrieved virtual space address pointer into the register spaces*/
    printf("periph SPI0 paddress: %x\n", perip_SPI0.addr);
    pSPI0 = (volatile aSPI0*) perip_SPI0.addr;

    /*Check register alignment: Print the register spaces*/
    printf("periph SPI0 physical address: %x\n", perip_SPI0.addr_p);
    printf("periph SPI0 virtual address: %x\n", perip_SPI0.addr);
    printf("register pSPI0 address: %x\n", pSPI0);
    printf("pSPI0 CS register: %x\n",&(pSPI0->CS.reg));
    printf("backward binary print of SPI0 CS register: ");
    printf_binary(pSPI0->CS.reg);
    printf("checking alignment of all registers:\n");
    printf("SPI0 CS register: %x\n", &(pSPI0->CS));
    printf("SPI0 FIFO register: %x\n", &(pSPI0->FIFO));
    printf("SPI0 CLK register: %x\n", &(pSPI0->CLK));
    printf("SPI0 DLEN register: %x\n", &(pSPI0->DLEN));
    printf("SPI0 LTOH register: %x\n", &(pSPI0->LTOH));
    printf("SPI0 DC register: %x\n", &(pSPI0->DC));

    /************** END MAPPING OF SPI REGISTERS ***********************/

    /************** MAPPING OF GPIO REGISTERS ***********************/

   /*Set peripheral GPIO structure address to virtual address
    The physical GPIO address is derived from the bus address  */
    perip_GPIO.addr_p = PHYSICAL_GPIO;
                                        printf("BUS_VIRTUAL_OFFSET: %x\n", PERIPH_BUS_PHYSICAL_OFFSET);
                                        printf("VITUAL_GPIO: %x\n", PHYSICAL_GPIO);
                                        printf("perip GPIO addr_p: %x\n", perip_GPIO.addr_p);
    /*Map the physical GPIO address space into virtual address space*/
    if(map_peripheral(&perip_GPIO,GPIO_BLOCK_SIZE) == -1) 
    {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }
    
    /*Map the retrieved virtual space address pointer into the register spaces*/
    printf("periph GPIO paddress: %x\n", perip_GPIO.addr);
    pGPIO = (volatile aGPIO*) perip_GPIO.addr;

    /*Check register alignment: Print the register spaces*/
    printf("periph GPIO physical address: %x\n", perip_GPIO.addr_p);
    printf("periph GPIO virtual address: %x\n", perip_GPIO.addr);
    printf("register pGPIO address: %x\n", pGPIO);
    printf("pGPIO GPIO_GPFSEL0 register: %x\n",&(pGPIO->GPIO_GPFSEL0.reg));
    printf("backward binary print of GPIO CS register: ");
    printf_binary(pGPIO->GPIO_GPFSEL0.reg);
    printf("checking alignment of all registers:\n");
    printf("GPIO GPIO_GPFSEL0 register: %x\n", &(pGPIO->GPIO_GPFSEL0));
    printf("GPIO GPIO_GPFSEL1 register: %x\n", &(pGPIO->GPIO_GPFSEL1));
    printf("GPIO GPIO_GPFSEL2 register: %x\n", &(pGPIO->GPIO_GPFSEL2));
    printf("GPIO GPIO_GPFSEL3 register: %x\n", &(pGPIO->GPIO_GPFSEL3));
    printf("GPIO GPIO_GPFSEL4 register: %x\n", &(pGPIO->GPIO_GPFSEL4));
    printf("GPIO GPIO_GPFSEL5 register: %x\n", &(pGPIO->GPIO_GPFSEL5));
    printf("GPIO GPIO_GPSET0 register: %x\n", &(pGPIO->GPIO_GPSET0));
    printf("GPIO GPIO_GPSET1 register: %x\n", &(pGPIO->GPIO_GPSET1));
    printf("GPIO GPIO_GPCLR0; register: %x\n", &(pGPIO->GPIO_GPCLR0));
    printf("GPIO GPIO_GPCLR1; register: %x\n", &(pGPIO->GPIO_GPCLR1));
    printf("GPIO GPIO_GPLEV0; register: %x\n", &(pGPIO->GPIO_GPLEV0));
    printf("GPIO GPIO_GPLEV0; register: %x\n", &(pGPIO->GPIO_GPLEV1));   
    printf("GPIO GPIO_GPEDS0; register: %x\n", &(pGPIO->GPIO_GPEDS0));
    printf("GPIO GPIO_GPEDS0; register: %x\n", &(pGPIO->GPIO_GPEDS1));   
    printf("GPIO GPIO_GPREN0; register: %x\n", &(pGPIO->GPIO_GPREN0));
    printf("GPIO GPIO_GPREN1; register: %x\n", &(pGPIO->GPIO_GPREN1));   
    printf("GPIO GPIO_GPFEN0; register: %x\n", &(pGPIO->GPIO_GPFEN0));
    printf("GPIO GPIO_GPFEN1; register: %x\n", &(pGPIO->GPIO_GPFEN1));      
    printf("GPIO GPIO_GPHEN0; register: %x\n", &(pGPIO->GPIO_GPHEN0));
    printf("GPIO GPIO_GPHEN1; register: %x\n", &(pGPIO->GPIO_GPHEN0));   
    printf("GPIO GPIO_GPPUD; register: %x\n", &(pGPIO->GPIO_GPPUD));
    printf("GPIO GPIO_GPPUDCLK0; register: %x\n", &(pGPIO->GPIO_GPPUDCLK0));   
    printf("GPIO GPIO_GPPUDCLK1; register: %x\n", &(pGPIO->GPIO_GPPUDCLK1));  

    printf("GPIO pin 7(CE1) FSEL0 register:\n");
    printf("GPIO pin 8(CE0) FSEL0 register:\n");
    printf("GPIO pin 9(MISO) FSEL0 register:\n");
    printf_binary(pGPIO->GPIO_GPFSEL0.reg);
    printf("GPIO pin 10(MOSI) FSEL1 register:\n");
    printf("GPIO pin 11(SCLK) FSEL1 register:\n");
    printf_binary(pGPIO->GPIO_GPFSEL1.reg);
    printf("GPIO GPLEV0 register:\n");
    printf_binary(pGPIO->GPIO_GPLEV0.reg);
    printf("GPIO GPLEV1 register:\n");
    printf_binary(pGPIO->GPIO_GPLEV1.reg);
    printf("GPIO GPEDS0 register:\n");
    printf_binary(pGPIO->GPIO_GPEDS0.reg);
    printf("GPIO GPEDS1 register:\n");
    printf_binary(pGPIO->GPIO_GPEDS1.reg);    
    printf("GPIO GPREN0 register:\n");
    printf_binary(pGPIO->GPIO_GPREN0.reg);
    printf("GPIO GPREN1 register:\n");
    printf_binary(pGPIO->GPIO_GPREN1.reg);  
    printf("GPIO GPFEN0 register:\n");
    printf_binary(pGPIO->GPIO_GPFEN0.reg);
    printf("GPIO GPFEN1 register:\n");
    printf_binary(pGPIO->GPIO_GPFEN1.reg); 
    printf("GPIO GPHEN0 register:\n");
    printf_binary(pGPIO->GPIO_GPHEN0.reg);  
    printf("GPIO GPHEN1 register:\n");
    printf_binary(pGPIO->GPIO_GPHEN1.reg);
    
    printf("GPIO GPLEN0 register:\n");
    printf_binary(pGPIO->GPIO_GPLEN0.reg);  
    printf("GPIO GPLEN1 register:\n");
    printf_binary(pGPIO->GPIO_GPLEN1.reg);  
    printf("GPIO GPAREN0 register:\n");
    printf_binary(pGPIO->GPIO_GPAREN0.reg);  
    printf("GPIO GPAREN1 register:\n");
    printf_binary(pGPIO->GPIO_GPAREN1.reg);  
    printf("GPIO GPPUD register:\n");
    printf_binary(pGPIO->GPIO_GPPUD.reg);
    printf("GPIO GPPUDCLK0 register:\n");
    printf_binary(pGPIO->GPIO_GPPUDCLK0.reg);
    printf("GPIO GPPUDCLK1 register:\n");
    printf_binary(pGPIO->GPIO_GPPUDCLK1.reg);
    /************** END MAPPING OF GPIO REGISTERS ***********************/

   /************** MAPPING OF DMA REGISTERS ***********************/

   /*Set peripheral GPIO structure address to virtual address
    The physical GPIO address is derived from the bus address  */
    perip_DMA.addr_p = PHYSICAL_DMA;
                                        printf("BUS_VIRTUAL_OFFSET: %x\n", PERIPH_BUS_PHYSICAL_OFFSET);
                                        printf("VITUAL_DMA: %x\n", PHYSICAL_DMA);
                                        printf("perip DMA addr_p: %x\n", perip_DMA.addr_p);
    /*Map the physical GPIO address space into virtual address space*/
    if(map_peripheral(&perip_DMA,DMA_BLOCK_SIZE) == -1) 
    {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }
    
    /*Map the retrieved virtual space address pointer into the register spaces*/
    printf("periph DMA paddress: %x\n", perip_DMA.addr);
    pDMA = (volatile aDMA*) perip_DMA.addr;

    /*Check register alignment: Print the register spaces*/
    printf("periph DMA paddress: %x\n", perip_DMA.addr_p);
    printf("periph DMA address: %x\n", perip_DMA.addr);
    printf("pDMA address: %x\n", pDMA);
    printf("DMA channel enabled: \n");
    printf("address DMA channel enabled: %x\n",&(pDMA->ENABLE.reg));
    printf_binary(pDMA->ENABLE.reg);
    printf("DMA channel enabled: %x\n", pDMA->ENABLE.reg);
    printf("DMA channel 0 address: %x\n", &(pDMA->CHANNEL_0));
    printf("DMA channel 4 address: %x\n", &(pDMA->CHANNEL_4));
    printf("DMA channel 12 address: %x\n", &(pDMA->CHANNEL_12));   
    printf("DMA INT STATUS address: %x\n", &(pDMA->INT_STATUS));
    printf("DMA ENABLE address: %x\n", &(pDMA->ENABLE));
    /************** END OF MAPPING DMA REGISTERS *********************/

    /************** START OF GPIO CONFIG *************************/

       /*SPI is master
    Pin 7 - SS1 : output
    Pin 8 - SS0: output
    Pin 9 - MISO: input
    Pin 10 - MOSI: output
    Pin 11 - SCLK: output
    define alt0 function 100 for those*/

    //clear the pin setting & set alt0 function
    //pin7 config
    //SetReg(pGPIO->GPIO_GPCLR0.reg,GPIO_GPCLR0_7,1,1);
    SetReg(pGPIO->GPIO_GPFSEL0.reg,GPIO_GPFSEL0_FSEL7,3,0b100);
        //no pull
        SetReg(pGPIO->GPIO_GPPUD.reg,GPIO_GPPUD_PUD,2,0b00);
        usleep(2);
        SetReg(pGPIO->GPIO_GPPUDCLK0.reg,GPIO_GPPUDCLK0_7,1,1);
        usleep(2);
        SetReg(pGPIO->GPIO_GPPUD.reg,GPIO_GPPUD_PUD,2,0b00);
        SetReg(pGPIO->GPIO_GPPUDCLK0.reg,GPIO_GPPUDCLK0_7,1,0);
    //pin8 config
    //SetReg(pGPIO->GPIO_GPCLR0.reg,GPIO_GPCLR0_8,1,1);
    SetReg(pGPIO->GPIO_GPFSEL0.reg,GPIO_GPFSEL0_FSEL8,3,0b100);
        //no pull
        SetReg(pGPIO->GPIO_GPPUD.reg,GPIO_GPPUD_PUD,2,0b00);
        usleep(2);
        SetReg(pGPIO->GPIO_GPPUDCLK0.reg,GPIO_GPPUDCLK0_8,1,1);
        usleep(2);
        SetReg(pGPIO->GPIO_GPPUD.reg,GPIO_GPPUD_PUD,2,0b00);
        SetReg(pGPIO->GPIO_GPPUDCLK0.reg,GPIO_GPPUDCLK0_8,1,0);
    //pin9 config
    //SetReg(pGPIO->GPIO_GPCLR0.reg,GPIO_GPCLR0_9,1,1);
    SetReg(pGPIO->GPIO_GPFSEL0.reg,GPIO_GPFSEL0_FSEL9,3,0b100);
        //pullup
        SetReg(pGPIO->GPIO_GPPUD.reg,GPIO_GPPUD_PUD,2,0b10);
        usleep(2);
        SetReg(pGPIO->GPIO_GPPUDCLK0.reg,GPIO_GPPUDCLK0_9,1,1);
        usleep(2);
        //SetReg(pGPIO->GPIO_GPPUD.reg,GPIO_GPPUD_PUD,2,0b00);
        //SetReg(pGPIO->GPIO_GPPUDCLK0.reg,GPIO_GPPUDCLK0_9,1,0);
    //pin10 config
    //SetReg(pGPIO->GPIO_GPCLR1.reg,GPIO_GPCLR0_10,1,1);
    SetReg(pGPIO->GPIO_GPFSEL1.reg,GPIO_GPFSEL1_FSEL10,3,0b100);
        //no pull
        SetReg(pGPIO->GPIO_GPPUD.reg,GPIO_GPPUD_PUD,2,0b00);
        usleep(2);
        SetReg(pGPIO->GPIO_GPPUDCLK1.reg,GPIO_GPPUDCLK0_10,1,1);
        usleep(2);
        SetReg(pGPIO->GPIO_GPPUD.reg,GPIO_GPPUD_PUD,2,0b00);
        SetReg(pGPIO->GPIO_GPPUDCLK0.reg,GPIO_GPPUDCLK0_10,1,0);
    //pin11 config
    //SetReg(pGPIO->GPIO_GPCLR1.reg,GPIO_GPCLR0_11,1,1);
    SetReg(pGPIO->GPIO_GPFSEL1.reg,GPIO_GPFSEL1_FSEL11,3,0b100);
        //no pull
        SetReg(pGPIO->GPIO_GPPUD.reg,GPIO_GPPUD_PUD,2,0b00);
        usleep(2);
        SetReg(pGPIO->GPIO_GPPUDCLK0.reg,GPIO_GPPUDCLK0_11,1,1);
        usleep(2);
        SetReg(pGPIO->GPIO_GPPUD.reg,GPIO_GPPUD_PUD,2,0b00);
        SetReg(pGPIO->GPIO_GPPUDCLK0.reg,GPIO_GPPUDCLK0_11,1,0);

    SetReg(pSPI0->CS.reg,0,32,0x30);
    //Not too slow!! otherwise will not work
    SetReg(pSPI0->CLK.reg,SPI0_CLK_CDIV,16,256);


    /************** END OF GPIO CONFIG *************************/

    /************** STARTING OF DMA ********************/

    /*Enable the DMA channels to ensure they are enabled*/
    printf("Enable the DMA channels\n");
    printf_binary(pDMA->ENABLE.reg);
    //Have bad experience with disabling the DMA's. Maybe with reset that issues would disappear. To test.
    //SetReg(pDMA->ENABLE.reg,11,1,0); //ch4 is free & ch12 is free
    //SetReg(pDMA->ENABLE.reg,11,1,1); //ch4 is free & ch12 is free
    //SetReg(pDMA->ENABLE.reg,12,1,0); //ch4 is free & ch12 is free
    //SetReg(pDMA->ENABLE.reg,12,1,1); //ch4 is free & ch12 is free
    SetReg(pDMA->CHANNEL_7.CS.reg,DMA_CS_RESET,1,1);
    SetReg(pDMA->CHANNEL_12.CS.reg,DMA_CS_RESET,1,1);
    
    /************** END OF STARTING OF DMA ********************/

    /************** START CONFIG OF SPI ***********************/

    //SetReg(pSPI0->CS.reg,SPI0_CS_INTD,1,0); //Dont generate done interupt
    //SetReg(pSPI0->CS.reg,SPI0_CS_INTR,1,0); //Dont generate rx interupt
    //SetReg(pSPI0->CS.reg,SPI0_CS_LEN,1,0); //The interface will behave as SPI master
    
    //SetReg(pSPI0->LTOH.reg,SPI0_LTOH_TOH,4,0); //As LoSSI setting out of scope nonetheless setting to no delay
    
    //Settings are important to time the DREQ signals right
    SetReg(pSPI0->DC.reg,SPI0_DC_TDREQ,8,32);    //Sends DREQ signal to TX DMA engine when FIFO level <=0x20 (default)
    SetReg(pSPI0->DC.reg,SPI0_DC_TPANIC,8,256);    //Sends Panic signal to TX DMA engine when FIFO level <=0x10 (default)
    SetReg(pSPI0->DC.reg,SPI0_DC_RDREQ,8,16);    //Sends DREQ signal to RX DMA engine when FIFO level >=0x20 (default)
    SetReg(pSPI0->DC.reg,SPI0_DC_RPANIC,8,128);    //Sends Panic signal to RX DMA engine when FIFO level >=0x30 (default)*/
    
    /************** END CONFIG OF SPI ***********************/

    /************** START SPI OPERATION *********************/

    /*10.6.3 DMA
        Note: In order to function correctly, each DMA channel must be set to perform 32-bit
        transfers when communicating with the SPI. Either the Source or the Destination Transfer
        Width field in the DMA TI register must be set to 0 (i.e. 32-bit words) depending upon
        whether the channel is reading or writing to the SPI.
        Two DMA channels are required, one to read from and one to write to the SPI.
        j) Enable DMA DREQ’s by setting the DMAEN bit and ADCS if required.
        k) Program two DMA control blocks, one for each DMA controller.
        l) DMA channel 1 control block should have its PER_MAP set to x and should be set to
        write ‘transfer length’ + 1 words to SPI_FIFO. The data should comprise:
            i) A word with the transfer length in bytes in the top sixteen bits, and the control
            register settings [7:0] in the bottom eight bits (i.e. TA = 1, CS, CPOL, CPHA as
            required.)
            ii) ‘Transfer length’ number in words of data to send.
        m) DMA channel 2 control block should have its PER_MAP set to y and should be set to
        read ‘transfer length’ words from SPI_FIFO.
        n) Point each DMA channel at its CB and set its ACTIVE bit to 1.
        o) On receipt of an interrupt from DMA channel 2, the transfer is complete. 
    */

        //config high level SPI functions
        SetReg(pSPI0->CS.reg,SPI0_CS_TA,1,0); //Stop SPI if would be active
        SetReg(pSPI0->CS.reg,SPI0_CS_LEN,1,0); //The interface will behave as SPI master
        SetReg(pSPI0->CS.reg,SPI0_CS_REN,1,0); /*NEEDS TO BE ZER0! BY DEFAULT 1! In bidirectional SPI master mode the same SPI standard is implemented except that a single
                                              /*  wire is used for the data (MIMO) instead of the two as in standard mode (MISO and MOSI).
                                                Bidirectional mode is used in a similar way to standard mode, the only difference is that
                                                before attempting to read data from the slave, you must set the read enable (SPI_REN) bit in
                                                the SPI control and status register (SPI_CS). This will turn the bus around, and when you
                                                write to the SPI_FIFO register (with junk) a read transaction will take place on the bus, and
                                                the read data will appear in the FIFO. */

        //SetReg(pSPI0->CS.reg,SPI0_CS_INTD,1,0);
        SetReg(pSPI0->CS.reg,SPI0_CS_ADCS,1,1); //Auto deassert CS at end
        
        /********  FILL Rx CONTROL BLOCK ************/
            SetReg(pCBRx->TI.reg,DMA_TI_WAIT_RESP,1,1);         //Yes wait for response KISS
            //SetReg(pCBRx->TI.reg,DMA_TI_DEST_WIDTH,1,0);        //Keep to 32-bit width write 
                                                                /*Note: In order to function correctly, each DMA channel must be set to perform 32-bit
                                                                transfers when communicating with the SPI. Either the Source or the Destination Transfer
                                                                Width field in the DMA TI register must be set to 0 (i.e. 32-bit words) depending upon
                                                                whether the channel is reading or writing to the SPI. */
            //SetReg(pCBRx->TI.reg,DMA_TI_DEST_IGNORE,1,0);       //Write to destination (Rx mode on)
            //SetReg(pCBRx->TI.reg,DMA_TI_SRC_WIDTH,1,0);         //No Tx mode
            //SetReg(pCBRx->TI.reg,DMA_TI_SRC_IGNORE,1,0);        //No Tx mode
            //SetReg(pCBRx->TI.reg,DMA_TI_BURST_LENGTH,4,0);      //This applies to read bursts (is not supported apart from wide bursts indicated below)
            //SetReg(pCBRx->TI.reg,DMA_TI_WAITS,5,0b0000);        //No Waits
            //SetReg(pCBRx->TI.reg,DMA_TI_NO_WIDE_BURSTS,1,0);    //Wide bursts as default efficiency standard

            SetReg(pCBRx->TI.reg,DMA_TI_PERMAP,5,7);            //Pacing is set by SPI Rx 
            SetReg(pCBRx->TI.reg,DMA_TI_SRC_DREQ,1,1);          //Periph generates DREQ's
            SetReg(pCBRx->TI.reg,DMA_TI_SRC_INC,1,0);           //No Tx mode
            SetReg(pCBRx->SOURCE_AD.reg,DMA_SOURCE_AD,32,(uint32_t) 0x7E204004);  //No sourcing of data as only Rx
            //SetReg(pCBRx->SOURCE_AD.reg,DMA_SOURCE_AD,32,(uint32_t) VC_Tx.bus_addr);
            printf("Src Address loaded: %x\n", pCBRx->SOURCE_AD.reg);
            SetReg(pCBRx->TI.reg,DMA_TI_DEST_DREQ,1,0);         //Periph generates DREQ's
            SetReg(pCBRx->TI.reg,DMA_TI_DEST_INC,1,1);          //Yes increase destination address writing
            SetReg(pCBRx->DEST_AD.reg,DMA_DEST_AD,32,(uint32_t) VC_Rx.bus_addr);      //Rx databuffer will be incremented automatically by one per receipt

            //SetReg(pCBRx->STRIDE.reg,DMA_STRIDE_S_STRIDE,16,0);                         //NA (2D mode)
            //SetReg(pCBRx->STRIDE.reg,DMA_STRIDE_D_STRIDE,16,0);                         //NA (2D mode)
            SetReg(pCBRx->TXFR_LEN.reg,DMA_TXFR_LEN_XLENGTH,32,4*SizeOfRxDatabuffer);     //as in linear mode, full 32 write in one ignoring YLENGTH
            SetReg(pCBRx->NEXTCONBK.reg,DMA_NEXTCONBK_ADDR,32,(uint32_t) 0);        //Starting with one receipt
            //SetReg(pCBRx->NEXTCONBK.reg,DMA_NEXTCONBK_ADDR,32,(uint32_t) &CBRx);        //Starting with one receipt

        /****** END OF Rx CONTROL BLOCK *********/

        /********  FILL Tx CONTROL BLOCK ************/
        /*DMA ch13: Memory (Src) ---->  SPI Tx (Dest) */
            
            //SetReg(pCBTx->TI.reg,DMA_TI_INTEN,1,0);             //no interupts
            //SetReg(pCBTx->TI.reg,DMA_TI_TDMODE,1,0);            //no 2D mode
            SetReg(pCBTx->TI.reg,DMA_TI_WAIT_RESP,1,1);         //Yes wait for response KISS
            //SetReg(pCBTx->TI.reg,DMA_TI_DEST_WIDTH,1,0);        //Dest = SPI 
            //SetReg(pCBTx->TI.reg,DMA_TI_DEST_IGNORE,1,0);       //No write to destination as peripheral (Tx mode off)
            //SetReg(pCBTx->TI.reg,DMA_TI_SRC_WIDTH,1,0);         //32-bit mode
                                                                /*Note: In order to function correctly, each DMA channel must be set to perform 32-bit
                                                                    transfers when communicating with the SPI. Either the Source or the Destination Transfer
                                                                    Width field in the DMA TI register must be set to 0 (i.e. 32-bit words) depending upon
                                                                    whether the channel is reading or writing to the SPI. */
            
            //SetReg(pCBTx->TI.reg,DMA_TI_SRC_IGNORE,1,0);        //Dont ignore, read from source and transmit
            //SetReg(pCBTx->TI.reg,DMA_TI_BURST_LENGTH,4,0);      //This applies to read bursts (is not supported apart from wide bursts indicated below)
            //SetReg(pCBTx->TI.reg,DMA_TI_WAITS,5,0b0);       //No Waits
            //SetReg(pCBTx->TI.reg,DMA_TI_NO_WIDE_BURSTS,1,0);    //Wide bursts as default efficiency standard

            SetReg(pCBTx->TI.reg,DMA_TI_PERMAP,5,6);            //Pacing is set by SPI Tx 
            SetReg(pCBTx->TI.reg,DMA_TI_DEST_DREQ,1,1);         //Use pace writing by Tx SPI
            SetReg(pCBTx->TI.reg,DMA_TI_DEST_INC,1,0);          //No as Dest = Peripheral
            SetReg(pCBTx->DEST_AD.reg,DMA_DEST_AD,32,(uint32_t)0x7E204004);  //No sourcing of data as only Rx
            //SetReg(pCBTx->DEST_AD.reg,DMA_DEST_AD,32,(uint32_t) VC_Rx.bus_addr);
            SetReg(pCBTx->TI.reg,DMA_TI_SRC_DREQ,1,0);          //No it's Tx mode
            SetReg(pCBTx->TI.reg,DMA_TI_SRC_INC,1,1);           //No Tx mode so no increment
            SetReg(pCBTx->SOURCE_AD.reg,DMA_SOURCE_AD,32,(uint32_t) VC_Tx.bus_addr);    //Set TxDataBuffer
            printf("CBTx SOURCE AD register: %x \n",(uint32_t) VC_Tx.bus_addr);

            SetReg(pCBTx->TXFR_LEN.reg,DMA_TXFR_LEN_XLENGTH,32,4*(SizeOfTxDatabuffer));   //as in linear mode, full 32 write in one ignoring YLENGTH
                                                                                    /*DMA channel 1 control block should have its PER_MAP set to x and should be set to
                                                                                    write ‘transfer length’ + 1 words to SPI_FIFO. The data should comprise:
                                                                                    i) A word with the transfer length in bytes in the top sixteen bits, and the control
                                                                                    register settings [7:0] in the bottom eight bits (i.e. TA = 1, CS, CPOL, CPHA as required.)
                                                                                    ii) ‘Transfer length’ number in words of data to send. */
            //SetReg(pCBTx->STRIDE.reg,DMA_STRIDE_S_STRIDE,16,0);     //NA (2D mode)
            //SetReg(pCBTx->STRIDE.reg,DMA_STRIDE_D_STRIDE,16,0);     //NA (2D mode)

            SetReg(pCBTx->NEXTCONBK.reg,DMA_NEXTCONBK_ADDR,32,(uint32_t) 0);   //Starting with one receipt
            //SetReg(pCBTx->NEXTCONBK.reg,DMA_NEXTCONBK_ADDR,32,(uint32_t) &CBTx);   //Starting with one receipt

            /********** END OF Tx CONTROL BLOCK ***********/

        //fill TxBuffer for testing, skip first word as technical filled
        int8_t z = 0x52;
        for (int i=0;i<SizeOfTxDatabuffer;i++) {
            
            pTxDatabuffer.b8[i]= z;
            (z==0x5A)?z=0x41:z++;
        }

        printf("CBRx address: %x\n", (uint32_t)VC_CBRx.bus_addr);
        SetReg(pDMA->CHANNEL_7.CONBLK_AD.reg, 0,32, (uint32_t) VC_CBRx.bus_addr);
            
        printf("CBTx address: %x\n", (uint32_t)VC_CBTx.bus_addr);
        SetReg(pDMA->CHANNEL_12.CONBLK_AD.reg, 0,32, (uint32_t) VC_CBTx.bus_addr);

        printf("DMA TI reg:\n");

        /*Activate DMA - start the process*/
        /*SetReg(pDMA->CHANNEL_7.CS.reg,DMA_CS_WAIT_FOR_OUTSTANDING_WRITES,1,1);
        SetReg(pDMA->CHANNEL_7.CS.reg,DMA_CS_PANIC_PRIORITY,1,15);
        SetReg(pDMA->CHANNEL_7.CS.reg,DMA_CS_PRIORITY,1,10);
        
        SetReg(pDMA->CHANNEL_12.CS.reg,DMA_CS_WAIT_FOR_OUTSTANDING_WRITES,1,1);
        SetReg(pDMA->CHANNEL_12.CS.reg,DMA_CS_PANIC_PRIORITY,1,15);
        SetReg(pDMA->CHANNEL_12.CS.reg,DMA_CS_PRIORITY,1,10);*/
        //SetReg(pDMA->CHANNEL_12.CS.reg,DMA_CS_ACTIVE,1,1);
        SetReg(pSPI0->DLEN.reg,0,16,SizeOfRxDatabuffer);
        SetReg(pSPI0->CS.reg,SPI0_CS_CSPOL,1,0);
        SetReg(pSPI0->CS.reg,14,1,0);
        SetReg(pSPI0->CS.reg,15,1,0);
        printf("pSPI0->CS.reg:\n");
        printf_binary(pSPI0->CS.reg);


        SetReg(pSPI0->CS.reg,SPI0_CS_CLEAR,2,0b11);
        SetReg(pDMA->CHANNEL_12.CS.reg,DMA_CS_END,1,1);
        SetReg(pDMA->CHANNEL_7.CS.reg,DMA_CS_END,1,1);
        SetReg(pDMA->CHANNEL_12.DEBUG.reg,0,3,7);
        SetReg(pDMA->CHANNEL_7.DEBUG.reg,0,3,7);
        SetReg(pDMA->CHANNEL_12.CS.reg,DMA_CS_ACTIVE,1,1);
        SetReg(pDMA->CHANNEL_7.CS.reg,DMA_CS_ACTIVE,1,1);
        SetReg(pSPI0->DLEN.reg,0,16,SizeOfRxDatabuffer);
        SetReg(pSPI0->CS.reg,0,9,0b110000000);
        printf("pSPI0->CS.reg:\n");
        printf_binary(pSPI0->CS.reg);
        
        
        printf("pSPI0->CS.reg:\n");


        //really necessary to set Sleep here!!!! 1 second is not enough!
        sleep(5);

        for (int i=0;i<2048;i++) printf("%c",pRxDatabuffer.b8[i]);
        printf("\n");
        printf_binary(pDMA->CHANNEL_12.CS.reg);
        SetReg(pDMA->CHANNEL_7.CS.reg,DMA_CS_ACTIVE,1,0);
        SetReg(pDMA->CHANNEL_12.CS.reg,DMA_CS_ACTIVE,1,0);

        printf("\n");
        printf_binary(pDMA->CHANNEL_7.CS.reg);
        printf_binary(pDMA->CHANNEL_7.DEBUG.reg);
        printf_binary(pDMA->CHANNEL_12.CS.reg);
        printf_binary(pDMA->CHANNEL_12.DEBUG.reg);

        unmap_peripheral(&perip_SPI0,SPI0_BLOCK_SIZE);
        unmap_peripheral(&perip_GPIO,GPIO_BLOCK_SIZE);
        unmap_peripheral(&perip_DMA,DMA_BLOCK_SIZE);
        VC_Rx.retMsg=unlock_vc_mem(VC_Rx.fd,VC_Rx.h);
        VC_Rx.retMsg=free_vc_mem(VC_Rx.fd,VC_Rx.h);
        VC_Tx.retMsg=unlock_vc_mem(VC_Tx.fd,VC_Tx.h);
        VC_Tx.retMsg=free_vc_mem(VC_Tx.fd,VC_Tx.h);
        unmap_segment(VC_Rx.phys_addr,SizeOfRxMemory);
        unmap_segment(VC_Tx.phys_addr,SizeOfTxMemory);
        close(VC_Rx.fd);
        close(VC_Tx.fd);
        VC_CBRx.retMsg=unlock_vc_mem(VC_CBRx.fd,VC_CBRx.h);
        VC_CBRx.retMsg=free_vc_mem(VC_CBRx.fd,VC_CBRx.h);
        VC_CBTx.retMsg=unlock_vc_mem(VC_CBTx.fd,VC_CBTx.h);
        VC_CBTx.retMsg=free_vc_mem(VC_CBTx.fd,VC_CBTx.h);
        unmap_segment(VC_CBRx.phys_addr,SizeOfCBRxMemory);
        unmap_segment(VC_CBTx.phys_addr,SizeOfCBTxMemory);
        close(VC_CBRx.fd);
        close(VC_CBTx.fd);

        printf("done\n");
        printf("done\n");
}