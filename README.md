# SPI-drivers
 SPI-drivers for ESP32 / Raspberry PI3 / Arduino M0/Mkr

This repositry holds SPI drivers for 
1) ESP32 (DevkitC v4)
2) Raspberry PI3 (user space using own driver)
3) Arduino M0/Mkr - SAMD21G (Arduino Mkr 1010 Wifi)

These drivers have been developped to test the output capabilities for these boards.
More specifically a test setup was measuring the max speed achieved for an SPI bus created between the memory of each of them:
   
   ESP's PSRAM <---> ESP32 <------> Arduino Mkr 
      
      (ref post https://www.esp32.com/viewtopic.php?f=13&t=18047&sid=17b77fdbc56c3ed35d0481a16ee5b856)
   
   ESP's PSRAM <----> ESP32 <------> RPi3

DMA was used for ESP32 & RPi3.
SPI drivers: 
 ESP32 - driver provided by Espressif
 
 RPi3 - own developped driver in user space using the VideoCore access to DMA and memory
 
 Arduino Mkr - own developped driver
 
I appreciated ESP32 most, as the proprietary driver was used at 17Mhz speeds.
The RPi3 could follow the speed, but required own developped driver.

No duration tests were carried out.

