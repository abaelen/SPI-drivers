/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

#include <SPI.h>
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

#define PacketSize  2048

int x=0;
const int slaveSelectPin=7;
const int handshakePin=6;
char Msg[129];

void setup() {
  // put your setup code here, to run once:
//  SerialUSB.begin(115200);
  SPI.begin();
  pinMode(slaveSelectPin,OUTPUT);
  pinMode(handshakePin,INPUT);
//  while(!SerialUSB);
//Bind RTC clock to 8MHz source and start it
      RTC->MODE0.COUNT.bit.COUNT=0x00;
      while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY );
      uint8_t *clkctrlID; clkctrlID = (uint8_t *) (0x40000C02UL); *clkctrlID=(int8_t) 4; //set clckctrl register to 30 - ADC
      GCLK->CLKCTRL.bit.GEN = 0x0; //Change to 8MHzclock generator 3
      while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY );
      GCLK->CLKCTRL.bit.CLKEN = 1;
//Before enabling initialize the RTC mode
      RTC->MODE0.CTRL.bit.MODE=0x0; //32-bit count value
      //CTRL.CLKREP not valid in Mode0
      RTC->MODE0.CTRL.bit.PRESCALER=0x0; //DIV 1 we allow 8MHz
      RTC->MODE1.READREQ.bit.RCONT=1; //Continuous reading 

//Enable the clock
      RTC->MODE0.CTRL.bit.ENABLE=0x00;
      while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY );

      SPI.beginTransaction(SPISettings(15000000, MSBFIRST, SPI_MODE0));
      //Override the conservative 12Mhz setting on Arduino's kernel
      SERCOM1->SPI.CTRLA.bit.ENABLE = 0;
      while(SERCOM1->SPI.SYNCBUSY.bit.ENABLE); 
        SERCOM1->SPI.BAUD.reg = 0x00;
      SERCOM1->SPI.CTRLA.bit.ENABLE = 1;
      while(SERCOM1->SPI.SYNCBUSY.bit.ENABLE);
      
}

void loop() {
  // put your main code here, to run repeatedly:
  delayMicroseconds(5); //required to sync the devices  
  while (digitalRead(handshakePin)==LOW);

  
  for (int i=0;i<PacketSize;i++){
    //Inspired from the code of Baxsie:
    //There is a 1-byte buffer in front of the
    //the SPI transmit shift register. If that
    //register is empty, we can write.
    //Speedup: write directly to the register

   SERCOM1->SPI.DATA.reg = 0x42;//MsgIn[i]; // Writing data into Data register
   while( (SERCOM1->SPI.INTFLAG.bit.DRE) == 0)
   {
     // Waiting Complete Reception
   }

    //x++;
  }
 
}
