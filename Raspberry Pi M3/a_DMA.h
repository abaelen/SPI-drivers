#include "a_GEN.h"


#define BUS_DMA                 0x7E007000
#define PHYSICAL_DMA            BUS_DMA - (PERIPH_BUS_PHYSICAL_OFFSET)
#define DMA_BLOCK_SIZE 		    (2*4096) //Block size of DMA register (ex ch15)

#define    DMA_CS_RESET                             31
#define    DMA_CS_ABORT                             30
#define    DMA_CS_DISDEBUG                          29
#define    DMA_CS_WAIT_FOR_OUTSTANDING_WRITES       28
#define    DMA_CS_PANIC_PRIORITY                    20
#define   DMA_CS_PRIORITY                          16
#define    DMA_CS_ERROR                             8
#define    DMA_CS_WAITING_FOR_OUTSTANDING_WRITES    6
#define    DMA_CS_DREQ_STOPS_DMA                    5
#define    DMA_CS_PAUSED                            4
#define    DMA_CS_DREQ                              3
#define    DMA_CS_INT                               2
#define    DMA_CS_END                               1
#define    DMA_CS_ACTIVE                            0
#define    DMA_CONBLK_AD                            0
#define    DMA_TI_NO_WIDE_BURSTS                    26
#define    DMA_TI_WAITS                             21
#define    DMA_TI_PERMAP                            16
#define    DMA_TI_BURST_LENGTH                      12
#define    DMA_TI_SRC_IGNORE                        11
#define    DMA_TI_SRC_DREQ                          10
#define    DMA_TI_SRC_WIDTH                         9
#define    DMA_TI_SRC_INC                           8
#define    DMA_TI_DEST_IGNORE                       7
#define    DMA_TI_DEST_DREQ                         6
#define    DMA_TI_DEST_WIDTH                        5
#define    DMA_TI_DEST_INC                          4
#define    DMA_TI_WAIT_RESP                         3
#define    DMA_TI_TDMODE                            1
#define    DMA_TI_INTEN                             0
#define    DMA_SOURCE_AD                            0
#define    DMA_DEST_AD                              0
#define    DMA_TXFR_LEN_XLENGTH                     0
#define    DMA_TXFR_LEN_YLENGTH                     16
#define    DMA_STRIDE_S_STRIDE                      0
#define    DMA_STRIDE_D_STRIDE                      16
#define    DMA_NEXTCONBK_ADDR                       0
#define    DMA_DEBUG_READ_LAST_NOT_SET_ERROR        0
#define    DMA_DEBUG_FIFO_ERROR                     1
#define    DMA_DEBUG_READ_ERROR                     2
#define    DMA_DEBUG_OUTSTANDING_WRITES             4
#define    DMA_DEBUG_DMA_ID                         8
#define    DMA_DEBUG_DMA_STATE                      16
#define    DMA_DEBUG_VERSION                        25
#define    DMA_DEBUG_LITE                           28


/*Synopsis DMA Control And Status register contains the main control and status bits for this DMA channel.*/
typedef union 
{
    struct
    {
        /*Activate the DMA
        This bit enables the DMA. The DMA will start if this bit is set and the CB_ADDR is
        non zero. The DMA transfer can be paused and resumed by clearing, then
        setting it again. This bit is automatically cleared at the
        end of the complete DMA transfer, ie. after a NEXTCONBK = 0x0000_0000 has
        been loaded.
        RW 0x0*/
        uint32_t ACTIVE:1;
        /*DMA End Flag
        Set when the transfer described by the current control block is complete. 
        Write 1 to clear.
        W1C 0x0*/
        uint32_t END:1;
        /*Interrupt Status
        This is set when the transfer for the CB ends and INTEN is set to 1. Once set it
        must be manually cleared down, even if the next CB has INTEN = 0.
        Write 1 to clear.
        W1C 0x0*/
        uint32_t INT:1;
        /*DREQ State
        Indicates the state of the selected DREQ (Data Request) signal, ie. the DREQ
        selected by the PERMAP field of the transfer info.
        1 = Requesting data. This will only be valid once the DMA has started and the
        PERMAP field has been loaded from the CB. It will remain valid, indicating the
        selected DREQ signal, until a new CB is loaded. 
        If PERMAP is set to zero (unpaced transfer) then this bit will read back as 1.
        0 = No data request.
        RO 0x0*/
        uint32_t DREQ:1;
        /*DMA Paused State
        Indicates if the DMA is currently paused and not transferring data. This will occur
        if: the active bit has been cleared, if the DMA is currently executing wait cycles
        or if the debug_pause signal has been set by the debug block, or the number of
        outstanding writes has exceeded the max count.
        1 = DMA channel is paused.
        0 = DMA channel is running.
        RO 0x0*/
        uint32_t PAUSED:1;
        /*DMA Paused by DREQ State
        Indicates if the DMA is currently paused and not transferring data due to the
        DREQ being inactive..
        1 = DMA channel is paused.
        0 = DMA channel is running.
        RO 0x0*/
        uint32_t DREQ_STOPS_DMA:1;
        /*DMA is Waiting for the Last Write to be Received
        Indicates if the DMA is currently waiting for any outstanding writes to be
        received, and is not transferring data.
        1 = DMA channel is waiting.
        RO 0x0*/
        uint32_t WAITING_FOR_OUTSTANDING_WRITES:1;
        /*Reserved - Write as 0, read as don't care*/
        uint32_t RESERVED1:1;
        /*DMA Error
        Indicates if the DMA has detected an error. The error flags are available in the
        debug register, and have to be cleared by writing to that register.
        1 = DMA channel has an error flag set.
        0 = DMA channel is ok.
        RO 0x0*/
        uint32_t ERROR:1;
        /*Reserved - Write as 0, read as don't care*/
        uint32_t RESERVED2:7;
        /*AXI Priority Level
        Sets the priority of normal AXI bus transactions. This value is used when the
        panic bit of the selected peripheral channel is zero.
        Zero is the lowest priority.
        RW 0x0*/
        uint32_t PRIORITY:4;
        /*AXI Panic Priority Level
        Sets the priority of panicking AXI bus  transactions. This value is used when the
        panic bit of the selected peripheral channel is 1.
        Zero is the lowest priority.
        RW 0x0*/
        uint32_t PANIC_PRIORITY:4;
        /*Reserved - Write as 0, read as don't care*/        
        uint32_t RESERVED3:4;
        /*Wait for outstanding writes
        When set to 1, the DMA will keep a tally of the AXI writes going out and the write
        responses coming in. At the very end of the current DMA transfer it will wait
        until the last outstanding write response has been received before indicating the
        transfer is complete. Whilst waiting it will load the next CB address (but will
        not fetch the CB), clear the active flag (if the next CB address = zero), and it will
        defer setting the END flag or the INT flag until the last outstanding write response
        has been received. In this mode, the DMA will pause if it has
        more than 13 outstanding writes at any one time.
        RW 0x0*/
        uint32_t WAIT_FOR_OUTSTANDING_WRITES:1;
        /*Disable debug pause signal
        When set to 1, the DMA will not stop when the debug pause signal is asserted.
        RW 0x0*/
        uint32_t DISDEBUG:1;
        /*Abort DMA
        Writing a 1 to this bit will abort the current DMA CB. The DMA will load the
        next CB and attempt to continue. The bit cannot be read, and will self clear.
        W1SC 0x0*/
        uint32_t ABORT:1;
        /*DMA Channel Reset
        Writing a 1 to this bit will reset the DMA. 
        The bit cannot be read, and will self
        clear.
        W1SC 0x0*/
        uint32_t RESET:1;
    }bit;
    uint32_t reg;
}DMA_CS_Type;

/*Synopsis DMA Control Block Address register*/
typedef union 
{
    struct 
    {
        /*Control Block Address
        This tells the DMA where to find a Control Block stored in memory. When the ACTIVE bit is set and this
        address is non zero, the DMA will begin its transfer by loading the contents of the addressed CB into the
        relevant DMA channel registers.
        At the end of the transfer this register will be updated with the ADDR field of the NEXTCONBK control block
        register. If this field is zero, the DMA will stop. Reading this register will return the address of the
        currently active CB (in the linked list of CB s). The address must be 256 bit aligned, so the bottom 5 bits
        of the address must be zero.
        RW 0x0
        */
       uint32_t SCB_ADDR:32;
    }bit;
    uint32_t reg;
}DMA_CONBLK_AD_Type;

/*Synopsis DMA Transfer Information.*/
typedef union 
{
    struct
    {
        /*Interrupt Enable
        1 = Generate an interrupt when the transfer described by the current Control Block completes.
        0 = Do not generate an interrupt.
        RW 0x0*/
        uint32_t INTEN:1;
        /*2D Mode - Only applicable for channels 0 to 6
        1 = 2D mode interpret the TXFR_LEN register as YLENGTH number of transfers each of XLENGTH, and
        add the strides to the address after each transfer.
        0 = Linear mode interpret the TXFR register as a single transfer of total length {YLENGTH ,XLENGTH}.
        RW 0x0*/
        uint32_t TDMODE:1;
        /*Reserved - Write as 0, read as don't care*/
        uint32_t RESERVE1:1;
        /*Wait for a Write Response
        When set this makes the DMA wait until it receives the AXI write response for each write. This ensures
        that multiple writes cannot get stacked in the AXI bus pipeline.
        1= Wait for the write response to be received before proceeding.
        0 = Don t wait; continue as soon as the write data is sent. RW 0x0*/
        uint32_t WAIT_RESP:1;
        /*Destination Address Increment
        1 = Destination address increments after each write. 
        The address will increment by 4, if DEST_WIDTH=0 else by 32.
        0 = Destination address does not change.
        RW 0x0*/
        uint32_t DEST_INC:1;
        /*Destination Transfer Width
        1 = Use 128-bit destination write width.
        0 = Use 32-bit destination write width.
        RW 0x0*/
        uint32_t DEST_WIDTH:1;
        /*Control Destination Writes with DREQ
        1 = The DREQ selected by PERMAP will gate the destination writes.
        0 = DREQ has no effect.
        RW 0x0 */
        uint32_t DEST_DREQ:1;
        /*Ignore Writes
        1 = Do not perform destination writes.
        0 = Write data to destination.
        RW 0x0*/
        uint32_t DEST_IGNORE:1;
        /*Source Address Increment
        1 = Source address increments after each read. The address will increment by 4, if S_WIDTH=0 else by 32.
        0 = Source address does not change.
        RW 0x0*/
        uint32_t SRC_INC:1;
        /*Source Transfer Width
        1 = Use 128-bit source read width.
        0 = Use 32-bit source read width.
        RW 0x0*/
        uint32_t SRC_WIDTH:1;
        /*Control Source Reads with DREQ
        1 = The DREQ selected by PER_MAP will gate the source reads.
        0 = DREQ has no effect.
        RW 0x0*/
        uint32_t SRC_DREQ:1;
        /*Ignore Reads
        1 = Do not perform source reads. In addition, destination writes will zero all the write strobes. This is used for fast cache fill operations.
        0 = Perform source reads..
        RW 0x0*/
        uint32_t SRC_IGNORE:1;
        /*Burst Transfer Length
        Indicates the burst length of the DMA transfers. The DMA will attempt to transfer data as bursts of this number of words. 
        A value of zero will produce a single transfer. Bursts are only produced for specific conditions, see main text.
        RW 0x0 */
        uint32_t BURST_LENGTH:4;
        /*Peripheral Mapping
        Indicates the peripheral number (1-31) whose ready signal shall be used to control the rate of the
        transfers, and whose panic signals will be output on the DMA AXI bus. Set to 0 for a continuous un-paced transfer.
        RW 0x0
            DREQ Peripheral
            0 DREQ = 1
              This is always on so use this
              channel if no DREQ is required.
            1 DSI
            2 PCM TX
            3 PCM RX
            4 SMI
            5 PWM
            6 SPI TX
            7 SPI RX
            8 BSC/SPI Slave TX
            9 BSC/SPI Slave RX
            10 unused
            11 e.MMC
            12 UART TX
            13 SD HOST
            14 UART RX.
            15 DSI
            16 SLIMBUS MCTX.
            17 HDMI
            18 SLIMBUS MCRX
            19 SLIMBUS DC0
            20 SLIMBUS DC1
            21 SLIMBUS DC2
            22 SLIMBUS DC3
            23 SLIMBUS DC4
            24 Scaler FIFO 0 & SMI *
            25 Scaler FIFO 1 & SMI *
            26 Scaler FIFO 2 & SMI *
            27 SLIMBUS DC5
            28 SLIMBUS DC6
            29 SLIMBUS DC7
            30 SLIMBUS DC8
            31 SLIMBUS DC9
        */
        uint32_t PERMAP:5;
        /*Add Wait Cycles
        This slows down the DMA throughput by setting the number of dummy cycles burnt after each DMA read
        or write operation is completed. A value of 0 means that no wait cycles are to be added.
        RW 0x0*/
        uint32_t WAITS:5;
        /*Don t Do wide writes as a 2 beat burst - Only applicable for channels 0 to 6
        This prevents the DMA from issuing wide writes as 2 beat AXI bursts. 
        This is an inefficient access mode, so the default is to use the bursts.
        RW 0x0*/
        uint32_t NO_WIDE_BURSTS:1;
        /*Reserved - Write as 0, read as don't care*/
        uint32_t RESERVED2:5;
    }bit;
    uint32_t reg;
}DMA_TI_Type;

/*Synopsis DMA Source Address*/
typedef union 
{
    struct
    {
        /*DMA Source Address
        Source address for the DMA operation. Updated by the DMA engine as the transfer progresses.
        RW 0x0*/
        uint32_t S_ADDR;
    }bit;
    uint32_t reg;
}DMA_SOURCE_AD_Type;

/*Synopsis DMA Destination Address*/
typedef union 
{
    struct
    {
        /*DMA Destination Address
        Destination address for the DMA operation. Updated by the DMA engine as the transfer progresses.
        RW 0x0*/
        uint32_t S_ADDR;
    }bit;
    uint32_t reg;
}DMA_DEST_AD_Type;

/*Synopsis DMA Transfer Length. This specifies the amount of data to be transferred in bytes.
In normal (non 2D) mode this specifies the amount of bytes to be transferred.
In 2D mode it is interpreted as an X and a Y length, and the DMA will perform Y transfers, each
of length X bytes and add the strides onto the addresses after each X leg of the transfer.
The length register is updated by the DMA engine as the transfer progresses, so it will indicate
the data left to transfer. */
typedef union 
{
    struct
    {
        /*Reserved - Write as 0, read as don't care*/
        uint32_t RESERVED:2;
        /*Only applicable for channels 0 to 6
        When in 2D mode, This is the Y transfer length, indicating how many xlength transfers are performed.
        When in normal linear mode this becomes the top bits of the XLENGTH
        RW 0x0*/
        uint32_t YLENGTH:14;
        /*Transfer Length in bytes. RW 0x0*/
        uint32_t XLENGTH:16;
    }bit;
    uint32_t reg;
}DMA_TXFR_LEN_Type;

/*Synopsis DMA 2D Stride*/
typedef union 
{
    struct
    {
        /*Source Stride (2D Mode)
        Signed (2 s complement) byte increment to apply to the source address at the end of each row in 2D mode.
        RW 0x0*/
        uint32_t S_STRIDE:16;
        /*Destination Stride (2D Mode)
        Signed (2 s complement) byte increment to apply to the destination address at the end of each row in 2D mode.
        RW 0x0*/
        uint32_t D_STRIDE:16;
    }bit;
    uint32_t reg;
}DMA_STRIDE_Type;

/*DMA Next Control Block Address
The value loaded into this register can be overwritten so that the linked list of Control Block data
structures can be altered. However it is only safe to do this when the DMA is paused. The
address must be 256 bit aligned and so the bottom 5 bits cannot be set and will read back as
zero. */
typedef union 
{
    struct
    {
        /*Address of next CB for chained DMA operations. RW 0x0*/
        uint32_t ADDR;
    }bit;
    uint32_t reg;
}DMA_NEXTCONBK_Type;

/*Synopsis DMA Debug register. */
typedef union 
{
    struct
    {
        /*Read Last Not Set Error
        If the AXI read last signal was not set when expected, then this error bit will be set. It can be
        cleared by writing a 1.
        RW 0x0*/
        uint32_t READ_LAST_NOT_SET_ERROR:1;
        /*Fifo Error
        Set if the optional read Fifo records an error condition. It can be cleared by writing a 1,
        RW 0x0 */
        uint32_t FIFO_ERROR:1;
        /*Slave Read Response Error
        Set if the read operation returned an error value on the read response bus. It can be cleared by writing a 1,
        RW 0x0*/
        uint32_t READ_ERROR:1;
        /*Reserved - Write as 0, read as don't care*/
        uint32_t RESERVED1:1;
        /*DMA Outstanding Writes Counter
        Returns the number of write responses that have not yet been received.
        This count is reset at the start of each new DMA transfer or with a DMA reset.
        RO 0x0*/
        uint32_t OUTSTANDING_WRITES:4;
        /*DMA ID
        Returns the DMA AXI ID of this DMA channel. RO 0x0*/
        uint32_t DMA_ID:8;
        /*DMA State Machine State
        Returns the value of the DMA engines state machine for this channel. 
        RO 0x0*/
        uint32_t DMA_STATE:9;
        /*DMA Version
        DMA version number, indicating control bit filed changes.
        RO 0x2*/
        uint32_t VERSION:3;
        /*DMA Lite
        Set if the DMA is a reduced performance LITE engine.
        RO 0x0*/
        uint32_t LITE:1;
        /*Reserved - Write as 0, read as don't care*/
        uint32_t RESERVED3:3;
    }bit;
    uint32_t reg;
}DMA_DEBUG_Type;

/*Synopsis Interrupt status of each DMA engine*/
typedef union 
{
    struct
    {
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT0:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT1:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT2:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT3:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT4:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT5:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT6:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT7:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT8:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT9:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT10:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT11:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT12:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT13:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT14:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t INT15:1;
        /*Reserved - Write as 0, read as don't care*/
        uint32_t RESERVED:16;
    }bit;
    uint32_t reg;
}DMA_INT_STATUS_Type;

/*Synopsis Global enable bits for each channel
Avoid channels 0, 1, 2, 3, 6, 7. The GPU uses 1, 3, 6, 7. The frame buffer uses 0 and the SD card uses 2. (Joan)*/
typedef union 
{
    struct
    {
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN0:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN1:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN2:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN3:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN4:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN5:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN6:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN7:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN8:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN9:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN10:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN11:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN12:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN13:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t EN14:1;
        /*Interrupt status of DMA engine 0 RW 0x0*/
        uint32_t RESERVED:17;
    }bit;
    uint32_t reg;
}DMA_ENABLE_Type;

/*Register Map 
Each DMA channel has an identical register map, only the base address of each channel is
different.
There is a global enable register at the top of the Address map that can disable each DMA for
powersaving.
Only three registers in each channels register set are directly writeable (CS, CONBLK_AD
and DEBUG). The other registers (TI, SOURCE_AD, DEST_AD, TXFR_LEN, STRIDE &
NEXTCONBK), are automatically loaded from a Control Block data structure held in
external memory. */
typedef struct 
{
    DMA_CS_Type                 CS;          //Control and Status            RW
    DMA_CONBLK_AD_Type          CONBLK_AD;   //Control Block Address         RW
    DMA_TI_Type                 TI;          //Transfer Information          RO
    DMA_SOURCE_AD_Type          SOURCE_AD;   //Source Address                RO
    DMA_DEST_AD_Type            DEST_AD;     //Destination Address           RO
    DMA_TXFR_LEN_Type           TXFR_LEN;   //Transfer Length               RO
    DMA_STRIDE_Type             STRIDE;      //2D Mode Stride                RO
                                            /*The value loaded into the NEXTCONBK register can be
                                            overwritten so that the linked list of Control Block data structures can be dynamically altered.
                                            However it is only safe to do this when the DMA is paused*/
    DMA_NEXTCONBK_Type          NEXTCONBK;   //Next Control Block Address    RO/RW?
    DMA_DEBUG_Type              DEBUG;       // Debug                        RW     
}DMA_CHANNEL_Type;

/*Each DMA channel has an identical register map, only the base address of each channel is
different.

There is a global enable register at the top of the Address map that can disable each DMA for
powersaving.

Only three registers in each channels register set are directly writeable (CS, CONBLK_AD
and DEBUG). The other registers (TI, SOURCE_AD, DEST_AD, TXFR_LEN, STRIDE &
NEXTCONBK), are automatically loaded from a Control Block data structure held in
external memory. 
The DMA Controller is comprised of several identical DMA Channels depending upon the
required configuration. Each individual DMA channel has an identical register map (although
LITE channels have less functionality and hence less registers).
DMA Channel 0 is located at the address of 0x7E007000, Channel 1 at 0x7E007100, Channel
2 at 0x7E007200 and so on. Thus adjacent DMA Channels are offset by 0x100.
DMA Channel 15 however, is physically removed from the other DMA Channels and so has
a different address base of 0x7EE05000.  */
typedef struct 
{
    DMA_CHANNEL_Type             CHANNEL_0;    uint8_t RESERVED1[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_1;    uint8_t RESERVED2[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_2;    uint8_t RESERVED3[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_3;    uint8_t RESERVED4[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_4;    uint8_t RESERVED5[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_5;    uint8_t RESERVED6[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_6;    uint8_t RESERVED7[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_7;    uint8_t RESERVED8[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_8;    uint8_t RESERVED9[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_9;    uint8_t RESERVED10[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_10;   uint8_t RESERVED11[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_11;   uint8_t RESERVED12[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_12;   uint8_t RESERVED13[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_13;   uint8_t RESERVED14[218]; //offset register to 0x100 multiple
    DMA_CHANNEL_Type             CHANNEL_14;   uint8_t RESERVED15[444]; //offset register to 0x100 multiple
    DMA_INT_STATUS_Type          INT_STATUS; //Interrupt status of each DMA channel
                                                uint8_t RESERVED16[12]; //offset register to 0x100 multiple
    DMA_ENABLE_Type              ENABLE;     //Global enable bits for each DMA channel
}aDMA;


/*Control Blocks (CB) are 8 words (256 bits) in length and must start at a 256-bit aligned
address. The format of the CB data structure in memory, is shown below.
Each 32 bit word of the control block is automatically loaded into the corresponding 32 bit
DMA control block register at the start of a DMA transfer. The descriptions of these registers
also defines the corresponding bit locations in the CB data structure in memory. 

The DMA is started by writing the address of a CB structure into the CONBLK_AD register
and then setting the ACTIVE bit. The DMA will fetch the CB from the address set in the
SCB_ADDR field of this reg and it will load it into the read-only registers described below.
It will then begin a DMA transfer according to the information in the CB.
When it has completed the current DMA transfer (length => 0) the DMA will update the
CONBLK_AD register with the contents of the NEXTCONBK register, fetch a new CB from
that address, and start the whole procedure once again.
The DMA will stop (and clear the ACTIVE bit) when it has completed a DMA transfer and
the NEXTCONBK register is set to 0x0000_0000. It will load this value into the
CONBLK_AD reg and then stop. 

Most of the DMA control block registers cannot be written to directly as they loaded automatically
from memory. They can be read to provide status information, and to indicate the progress of
the current DMA transfer. The value loaded into the NEXTCONBK register can be
overwritten so that the linked list of Control Block data structures can be dynamically altered.
However it is only safe to do this when the DMA is paused. */
typedef struct 
{
    DMA_TI_Type              TI;          //Transfer Information
    DMA_SOURCE_AD_Type       SOURCE_AD;   //Source Address
    DMA_DEST_AD_Type         DEST_AD;     //Destination Address
    DMA_TXFR_LEN_Type        TXFR_LEN;    //Transfer Length
    DMA_STRIDE_Type          STRIDE;      //2D Mode Stride
    DMA_NEXTCONBK_Type       NEXTCONBK;   //Next Control Block Address
    uint32_t                 RESERVED1;   //Reserved - set to zero
    uint32_t                 RESERVED2;   //Reserved - set to zero
}aCB __attribute__ ((aligned (32)));;



