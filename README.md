# SPI-drivers
 SPI-drivers for ESP32 / Raspberry PI3 / Arduino M0/Mkr

This repository holds SPI drivers for 
1) ESP32 (DevkitC v4)
2) Raspberry PI3 (user space using own driver)
3) Arduino M0/Mkr - SAMD21G (Arduino Mkr 1010 Wifi)

These drivers have been developped to test the output capabilities for these boards.
More specifically a test setup was measuring the max speed achieved for an SPI bus created between the memory of each of them:
   
   1) ESP's PSRAM <---> ESP32 <------> Arduino Mkr <--x--> too little memory
      
      (ref post https://www.esp32.com/viewtopic.php?f=13&t=18047&sid=17b77fdbc56c3ed35d0481a16ee5b856)
   
   1) ESP's PSRAM <----> ESP32 <------> RPi3 <-----> RPi's memory

DMA was used for ESP32 & RPi3.
SPI drivers: 
 - ESP32 - driver provided by Espressif
 
 - RPi3 - own developped driver in user space using the VideoCore access to DMA and memory
 
 - Arduino Mkr - own developped driver
 
Speeds of 17 - 18 Mhz were achieved.

No duration tests were carried out.

The goal of this endavour is to find the fastest path from parallel or serial bus to memory (having ADC in mind).
- My other repositories are looking into finding the fastest path over USB to PC memory.
- This repository is looking into finding the fastest path over SPI to µP/µC's memory.

Future endavours will be investigating FPGA's capabilities handling parallel/SPI bus to memory using Xilinx Cora Z6S
Reason: 
- None of the µC's (real time processing) offer USB3 speeds for direct PC interfacing.
- Although ESP32 offers real easy solution, ie. high speed with (17Mhz) large memory (4MB) on FreeRTOS (therefore hinting real time processing), it is too little for real ADC processing. 17Mhz max would mean a Nyquist frequency of max 8Mhz. Furthermore more memory is required for higher speeds.

Looking forward having fun with the Xilinx Cora Z6S!

Keep making!





