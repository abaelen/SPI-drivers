#include "a_GEN.h"

/*SPI Register Map
The BCM2835 devices has only one SPI interface of this type. It is referred to in all the
documentation as SPI0. It has two additional mini SPI interfaces (SPI1 and SPI2). The
specifiation of those can be found under 2.3 Universal SPI Master (2x).
The base address of this SPI0 interface is 0x7E204000. 
*/

#define BUS_SPI0                0x7E204000
#define PHYSICAL_SPI0           BUS_SPI0 - (PERIPH_BUS_PHYSICAL_OFFSET)
#define SPI0_BLOCK_SIZE 		(5*1024)

#define SPI0_DC_TDREQ           0
#define SPI0_DC_TPANIC          8
#define SPI0_DC_RDREQ           16
#define SPI0_DC_RPANIC          24
#define SPI0_LTOH_TOH           0
#define SPI0_DLEN_LEN           0
#define SPI0_CLK_CDIV           0
#define SPI0_FIFO               0
#define SPI0_CS_CS              0
#define SPI0_CS_CPHA            2
#define SPI0_CS_CPOL            3
#define SPI0_CS_CLEAR           4
#define SPI0_CS_CSPOL           6
#define SPI0_CS_TA              7
#define SPI0_CS_DMAEN           8
#define SPI0_CS_INTD            9
#define SPI0_CS_INTR            10
#define SPI0_CS_ADCS            11
#define SPI0_CS_REN             12
#define SPI0_CS_LEN             13
#define SPI0_CS_LMONO           14
#define SPI0_CS_TE_EN           15
#define SPI0_CS_DONE            16
#define SPI0_CS_RXD             17
#define SPI0_CS_TXD             18
#define SPI0_CS_RXR             19
#define SPI0_CS_RXF             20
#define SPI0_CS_CSPOL0          21
#define SPI0_CS_CSPOL1          22
#define SPI0_CS_CSPOL2          23
#define SPI0_CS_DMA_LEN         24
#define SPI0_CS_LEN_LONG        25


//CS Register
/*Synopsis This register contains the main control and status bits for the SPI.*/
typedef union 
{
    struct
    {
            /*Chip Select
            00 = Chip select 0
            01 = Chip select 1
            10 = Chip select 2
            11 = Reserved
            RW 0x0 */
        uint32_t CS:2;
            /*Clock Phase
            0 = First SCLK transition at middle of data bit.
            1 = First SCLK transition at beginning of data bit.
            RW 0x0*/
        uint32_t CPHA:1;
            /*Clock Polarity
            0 = Rest state of clock = low.
            1 = Rest state of clock = high.
            RW 0x0*/
        uint32_t CPOL:1;
            /*CLEAR FIFO Clear
            00 = No action.
            x1 = Clear TX FIFO. One shot operation.
            1x = Clear RX FIFO. One shot operation.
            If CLEAR and TA are both set in the same
            operation, the FIFOs are cleared before the new
            frame is started. Read back as 0.
            RW 0x0 */
        uint32_t CLEAR:2;
            /*Chip Select Polarity
            0 = Chip select lines are active low
            1 = Chip select lines are active high
            RW 0x0 */
        uint32_t CSPOL:1;
            /*Transfer Active
            0 = Transfer not active./CS lines are all high
            (assuming CSPOL = 0). RXR and DONE are 0.
            Writes to SPIFIFO write data into bits -0 of
            SPICS allowing DMA data blocks to set mode
            before sending data.
            1 = Transfer active. /CS lines are set according
            to CS bits and CSPOL. Writes to SPIFIFO write
            data to TX FIFO.TA is cleared by a
            dma_frame_end pulse from the DMA controller.
            RW 0x0 */
        uint32_t TA:1;
            /*DMAEN DMA Enable
            0 = No DMA requests will be issued.
            1 = Enable DMA operation.
            Peripheral generates data requests. These will
            be taken in four-byte words until the SPIDLEN
            has been reached.
            RW 0x0 */
        uint32_t DMAEN:1;
            /*INTD Interrupt on Done
            0 = Don t generate interrupt on transfer
            complete.
            1 = Generate interrupt when DONE = 1.
            RW 0x0  */
        uint32_t INTD:1;
           /*INTR Interrupt on RXR
            0 = Don t generate interrupts on RX FIFO
            condition.
            1 = Generate interrupt while RXR = 1.
            RW 0x0 */
        uint32_t INTR:1;
             /*ADCS Automatically Deassert Chip Select
            0 = Don t automatically deassert chip select at
            the end of a DMA transfer chip select is
            manually controlled by software.
            1 = Automatically deassert chip select at the end
            of a DMA transfer (as determined by SPIDLEN)
            RW 0x0 */
        uint32_t ADCS:1;
            /*REN Read Enable
            read enable if you are using bidirectional mode.
            If this bit is set, the SPI peripheral will be able to
            send data to this device.
            0 = We intend to write to the SPI peripheral.
            1 = We intend to read from the SPI peripheral.
            RW 0x1 */
        uint32_t REN:1;
            /*LEN LoSSI enable
            The serial interface is configured as a LoSSI
            master.
            0 = The serial interface will behave as an SPI
            master.
            1 = The serial interface will behave as a LoSSI
            master.
            RW 0x0 */
        uint32_t LEN:1;
            /*Unused RW 0x0 */
        uint32_t LMONO:1;
            /*Unused RW 0x0 */
        uint32_t TE_EN:1;
            /*Done transfer Done
            0 = Transfer is in progress (or not active TA = 0).
            1 = Transfer is complete. Cleared by writing
            more data to the TX FIFO or setting TA to 0.
            RO 0x0 */
        uint32_t DONE:1;
            /*RXD RX FIFO contains Data
            0 = RX FIFO is empty.
            1 = RX FIFO contains at least 1 byte.
            RO 0x0 */
        uint32_t RXD:1;
            /*TXD TX FIFO can accept Data
            0 = TX FIFO is full and so cannot accept more
            data.
            1 = TX FIFO has space for at least 1 byte.
            RO 0x1 */
        uint32_t TXD:1;
            /*RXR RX FIFO needs Reading ( full)
            0 = RX FIFO is less than full (or not active TA =
            0).
            1 = RX FIFO is or more full. Cleared by reading
            sufficient data from the RX FIFO or setting TA to
            0.
            RO 0x0 */
        uint32_t RXR:1;
            /*RXF - RX FIFO Full
            0 = RXFIFO is not full.
            1 = RX FIFO is full. No further serial data will be
            sent/ received until data is read from FIFO.
            RO 0x0 */
        uint32_t RXF:1;
            /*Chip Select 0 Polarity
            0= Chip select is active low.
            1= Chip select is active high.
            RW 0x0 */
        uint32_t CSPOL0:1;
            /*Chip Select 1 Polarity
            0= Chip select is active low.
            1= Chip select is active high.
            RW 0x0 */
        uint32_t CSPOL1:1;
            /*Chip Select 2 Polarity
            0= Chip select is active low.
            1= Chip select is active high.
            RW 0x0 */
        uint32_t CSPOL2:1;
            /*Enable DMA mode in Lossi mode RW 0x0 */
        uint32_t DMALEN:1;
            /*Enable Long data word in Lossi mode if
            DMA_LEN is set
            0= writing to the FIFO will write a single byte
            1= wrirng to the FIFO will write a 32 bit word
            RW 0x0 */
        uint32_t LEN_LONG:1;
            /*Reserved - Write as 0, read as don't care*/
        uint32_t reserved:6;
    }bit;
    uint32_t reg;
}SPI0_CS_Type;

/*Synopsis This register allows TX data to be written to the TX FIFO and RX data to be read from
the RX FIFO. */
typedef union 
{
    struct
    {
        /*DMA Mode (DMAEN set)
        If TA is clear, the first 32-bit write to this register
        will control SPIDLEN and SPICS. Subsequent
        reads and writes will be taken as four-byte data
        words to be read/written to the FIFOs
        Poll/Interrupt Mode (DMAEN clear, TA set)
        Writes to the register write bytes to TX FIFO.
        Reads from register read bytes from the RX
        FIFO
        RW 0x0 */
        uint32_t DATA:32;
    }bit;
    uint32_t reg;
}SPI0_FIFO_Type;


/*Synopsis This register allows the SPI clock rate to be set. */
typedef union  
{
    struct
    {
        /*Clock Divider
        SCLK = Core Clock / CDIV
        If CDIV is set to 0, the divisor is 65536. The
        divisor must be a power of 2. Odd numbers
        rounded down. The maximum SPI clock rate is
        of the APB clock.
        RW 0x0 */
        uint32_t CDIV:32;
    }bit;
    uint32_t reg;
}SPI0_CLK_Type;

/*Synopsis This register allows the SPI data length rate to be set*/
typedef union  
{
    struct 
    {
        /*Data Length
        The number of bytes to transfer.
        This field is only valid for DMA mode (DMAEN
        set) and controls how many bytes to transmit
        (and therefore receive).
        RW 0x0*/
        uint32_t LEN:16;
        /*Reserved - Write as 0, read as don't care*/
        uint32_t Reserved:16;
    }bit;
    uint32_t reg;
}SPI0_DLEN_Type;

/*Synopsis This register allows the LoSSI output hold delay to be set. */
typedef union 
{
    struct
    {
        /*This sets the Output Hold delay in APB clocks. A
        value of 0 causes a 1 clock delay.
        RW 0x1 */
        uint32_t TOH:4;
        /*Reserved - Write as 0, read as don't care*/
        uint32_t Reserved:28;
    }bit;
    uint32_t reg;
}SPI0_LTOH_Type;

/*Synopsis This register controls the generation of the DREQ and Panic signals to an external
DMA engine The DREQ signals are generated when the FIFOs reach their defined
levels and need servicing. The Panic signals instruct the external DMA engine to raise
the priority of its AXI requests. */
typedef union 
{
    struct
    {
       /*DMA Write Request Threshold.
        Generate a DREQ signal to the TX DMA engine
        whenever the TX FIFO level is less than or equal
        to this amount.
        RW 0x20  */
        uint32_t TDREQ:8;
        /*DMA Write Panic Threshold.
        Generate the Panic signal to the TX DMA engine
        whenever the TX FIFO level is less than or equal
        to this amount.
        RW 0x10 */
        uint32_t TPANIC:8;
        /*DMA Read Request Threshold.
        Generate A DREQ to the RX DMA engine
        whenever the RX FIFO level is greater than this
        amount, (RX DREQ is also generated if the
        transfer has finished but the RXFIFO isn t
        empty).
        RW 0x20*/
        uint32_t RDREQ:8;
        /*DMA Read Panic Threshold.
        Generate the Panic signal to the RX DMA
        engine whenever the RX FIFO level is greater
        than this amount.
        RW 0x30 */
        uint32_t RPANIC:8;
    }bit;
    uint32_t reg;
}SPI0_DC_Type;

/*The BCM2835 devices has only one SPI interface of this type. It is referred to in all the
documentation as SPI0. It has two additional mini SPI interfaces (SPI1 and SPI2). The
specifiation of those can be found under 2.3 Universal SPI Master (2x).
The base address of this SPI0 interface is 0x7E204000. */
typedef struct 
{
    SPI0_CS_Type    CS;        //SPI Master Control and Status
    SPI0_FIFO_Type  FIFO;      //SPI Master TX and RX FIFOs
    SPI0_CLK_Type   CLK;       //SPI Master Clock Divider
    SPI0_DLEN_Type  DLEN;      //SPI Master Data Length 
    SPI0_LTOH_Type  LTOH;      //SPI LOSSI mode TOH
    SPI0_DC_Type    DC;        ///SPI DMA DREQ Controls
}aSPI0;

