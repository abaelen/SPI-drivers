#pragma once

/*References:
1) https://iosoft.blog/2020/05/25/raspberry-pi-dma-programming/
2) Hendric @ https://www.raspberrypi.org/forums/viewtopic.php?t=88965
3) https://www.pieter-jan.com/node/15
4) https://medium.com/@jscdroiddev/c-code-development-on-a-virtual-raspberry-pi-desktop-4d3d326c08ec
5) https://github.com/jbentham/rpi
6) https://archive.fosdem.org/2017/schedule/event/programming_rpi3/attachments/slides/1475/export/events/attachments/programming_rpi3/slides/1475/bare_metal_rpi3.pdf

*/


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>


#define PERIPH_BUS_PHYSICAL_OFFSET       0x7E000000 - 0x3F000000
#define AllOne                           (uint32_t)(((uint64_t)1<<33)-1);
#define SetReg(var,pos,size,val)         var = (var & (((~0)<<(pos+size)) | ((1<<pos)-1))) | ((val)<<pos); //printf_binary(var);
//#define ClearReg(p,a)           p |=~(0<<a);


// IO Acces
struct bcm2835_peripheral {
    uint32_t addr_p;           // Address in physical map that we want this memory block to expose
    int mem_fd;                // File descriptor to physical memory virtual file '/dev/mem'
    void *map;
    volatile uint32_t *addr;
};

// Get virtual memory segment for peripheral regs or physical mem
int8_t map_peripheral(struct bcm2835_peripheral *p,size_t block_size)
{
   // Open /dev/mem
   if ((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("Failed to open /dev/mem, try checking permissions.\n");
      return -1;
   }
 
   p->map = mmap(
      NULL,
      block_size,
      PROT_READ|PROT_WRITE,
      MAP_SHARED,
      p->mem_fd,      // File descriptor to physical memory virtual file '/dev/mem'
      p->addr_p       // Address in physical map that we want this memory block to expose
   );
 
   if (p->map == MAP_FAILED) {
        perror("mmap");
        return -1;
   }
 
   p->addr = (volatile uint32_t *)p->map;

   return 0;
}
// unmap virtual memory segment for peripheral regs or physical mem
void unmap_peripheral(struct bcm2835_peripheral *p,size_t block_size) {
 
    munmap(p->map, block_size);
    close(p->mem_fd);
}
// print binary (/*in reverse*/)
void printf_binary(uint32_t n)
{
   uint8_t m=0;
   for (int i=31;i>=0;i--) 
   {
       m = (n >> i) & 0x01;
       if (m==1) printf("1");
       else printf("0");
   }
    
 /*   while (n) {
    if (n & 1)
        printf("1");
    else
        printf("0");

    n >>= 1;
}*/
printf("\n");
}

