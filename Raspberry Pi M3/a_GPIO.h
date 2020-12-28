#include "a_GEN.h"

/*GPIO Register Map
There are 54 general-purpose I/O (GPIO) lines split into two banks. All GPIO pins have at
least two alternative functions within BCM. The alternate functions are usually peripheral IO
and a single peripheral may appear in each bank to allow flexibility on the choice of IO
voltage. Details of alternative functions are given in section 6.2. Alternative Function
Assignments. 

The GPIO peripheral has three dedicated interrupt lines. These lines are triggered by the
setting of bits in the event detect status register. Each bank has its’ own interrupt line with the
third line shared between all bits.
The Alternate function table also has the pull state (pull-up/pull-down) which is applied after
a power down. 

*/

#define BUS_GPIO                0x7E200000 
#define BUS_CM                  0x7E101070 
#define PHYSICAL_GPIO           BUS_GPIO - (PERIPH_BUS_PHYSICAL_OFFSET)
#define GPIO_BLOCK_SIZE 		(1*1024)

#define GPIO_GPFSEL0_FSEL0      0
#define GPIO_GPFSEL0_FSEL1      3
#define GPIO_GPFSEL0_FSEL2      6
#define GPIO_GPFSEL0_FSEL3      9
#define GPIO_GPFSEL0_FSEL4      12
#define GPIO_GPFSEL0_FSEL5      15
#define GPIO_GPFSEL0_FSEL6      18
#define GPIO_GPFSEL0_FSEL7      21
#define GPIO_GPFSEL0_FSEL8      24
#define GPIO_GPFSEL0_FSEL9      27
#define GPIO_GPFSEL1_FSEL10      0
#define GPIO_GPFSEL1_FSEL11      3
#define GPIO_GPFSEL1_FSEL12      6
#define GPIO_GPFSEL1_FSEL13      9
#define GPIO_GPFSEL1_FSEL14      12
#define GPIO_GPFSEL1_FSEL15      15
#define GPIO_GPFSEL1_FSEL16      18
#define GPIO_GPFSEL1_FSEL17      21
#define GPIO_GPFSEL1_FSEL18      24
#define GPIO_GPFSEL1_FSEL19      27
#define GPIO_GPFSEL2_FSEL20      0
#define GPIO_GPFSEL2_FSEL21      3
#define GPIO_GPFSEL2_FSEL22      6
#define GPIO_GPFSEL2_FSEL23      9
#define GPIO_GPFSEL2_FSEL24      12
#define GPIO_GPFSEL2_FSEL25      15
#define GPIO_GPFSEL2_FSEL26      18
#define GPIO_GPFSEL2_FSEL27      21
#define GPIO_GPFSEL2_FSEL28      24
#define GPIO_GPFSEL2_FSEL29      27
#define GPIO_GPFSEL3_FSEL30      0
#define GPIO_GPFSEL3_FSEL31      3
#define GPIO_GPFSEL3_FSEL32      6
#define GPIO_GPFSEL3_FSEL33      9
#define GPIO_GPFSEL3_FSEL34      12
#define GPIO_GPFSEL3_FSEL35      15
#define GPIO_GPFSEL3_FSEL36      18
#define GPIO_GPFSEL3_FSEL37      21
#define GPIO_GPFSEL3_FSEL38      24
#define GPIO_GPFSEL3_FSEL39      27
#define GPIO_GPFSEL4_FSEL40      0
#define GPIO_GPFSEL4_FSEL41      3
#define GPIO_GPFSEL4_FSEL42      6
#define GPIO_GPFSEL4_FSEL43      9
#define GPIO_GPFSEL4_FSEL44      12
#define GPIO_GPFSEL4_FSEL45      15
#define GPIO_GPFSEL4_FSEL46      18
#define GPIO_GPFSEL4_FSEL47      21
#define GPIO_GPFSEL4_FSEL48      24
#define GPIO_GPFSEL4_FSEL49      27
#define GPIO_GPFSEL5_FSEL50      0
#define GPIO_GPFSEL5_FSEL51      3
#define GPIO_GPFSEL5_FSEL52      6
#define GPIO_GPFSEL5_FSEL53      9

#define GPIO_GPSET0_0      0
#define GPIO_GPSET0_1      1
#define GPIO_GPSET0_2      2
#define GPIO_GPSET0_3      3
#define GPIO_GPSET0_4      4
#define GPIO_GPSET0_5      5
#define GPIO_GPSET0_6      6
#define GPIO_GPSET0_7      7
#define GPIO_GPSET0_8      8
#define GPIO_GPSET0_9      9
#define GPIO_GPSET0_10     10
#define GPIO_GPSET0_11     11
#define GPIO_GPSET0_12     12
#define GPIO_GPSET0_13     13
#define GPIO_GPSET0_14     14
#define GPIO_GPSET0_15     15
#define GPIO_GPSET0_16     16
#define GPIO_GPSET0_17     17
#define GPIO_GPSET0_18     18
#define GPIO_GPSET0_19     19
#define GPIO_GPSET0_20     20
#define GPIO_GPSET0_21     21
#define GPIO_GPSET0_22     22
#define GPIO_GPSET0_23     23
#define GPIO_GPSET0_24     24
#define GPIO_GPSET0_25     25
#define GPIO_GPSET0_26     26
#define GPIO_GPSET0_27     27
#define GPIO_GPSET0_28     28
#define GPIO_GPSET0_29     29
#define GPIO_GPSET0_30     30
#define GPIO_GPSET0_31     31
#define GPIO_GPSET1_32     0
#define GPIO_GPSET1_33     1
#define GPIO_GPSET1_34     2
#define GPIO_GPSET1_35     3
#define GPIO_GPSET1_36     4
#define GPIO_GPSET1_37     5
#define GPIO_GPSET1_38     6
#define GPIO_GPSET1_39     7
#define GPIO_GPSET1_40     8
#define GPIO_GPSET1_41     9
#define GPIO_GPSET1_42     10
#define GPIO_GPSET1_43     11
#define GPIO_GPSET1_44     12
#define GPIO_GPSET1_45     13
#define GPIO_GPSET1_46     14
#define GPIO_GPSET1_47     15
#define GPIO_GPSET1_48     16
#define GPIO_GPSET1_49     17
#define GPIO_GPSET1_50     18
#define GPIO_GPSET1_51     19
#define GPIO_GPSET1_52     20
#define GPIO_GPSET1_53     21

#define GPIO_GPCLR0_0      0
#define GPIO_GPCLR0_1      1
#define GPIO_GPCLR0_2      2
#define GPIO_GPCLR0_3      3
#define GPIO_GPCLR0_4      4
#define GPIO_GPCLR0_5      5
#define GPIO_GPCLR0_6      6
#define GPIO_GPCLR0_7      7
#define GPIO_GPCLR0_8      8
#define GPIO_GPCLR0_9      9
#define GPIO_GPCLR0_10     10
#define GPIO_GPCLR0_11     11
#define GPIO_GPCLR0_12     12
#define GPIO_GPCLR0_13     13
#define GPIO_GPCLR0_14     14
#define GPIO_GPCLR0_15     15
#define GPIO_GPCLR0_16     16
#define GPIO_GPCLR0_17     17
#define GPIO_GPCLR0_18     18
#define GPIO_GPCLR0_19     19
#define GPIO_GPCLR0_20     20
#define GPIO_GPCLR0_21     21
#define GPIO_GPCLR0_22     22
#define GPIO_GPCLR0_23     23
#define GPIO_GPCLR0_24     24
#define GPIO_GPCLR0_25     25
#define GPIO_GPCLR0_26     26
#define GPIO_GPCLR0_27     27
#define GPIO_GPCLR0_28     28
#define GPIO_GPCLR0_29     29
#define GPIO_GPCLR0_30     30
#define GPIO_GPCLR0_31     31
#define GPIO_GPCLR1_32     0
#define GPIO_GPCLR1_33     1
#define GPIO_GPCLR1_34     2
#define GPIO_GPCLR1_35     3
#define GPIO_GPCLR1_36     4
#define GPIO_GPCLR1_37     5
#define GPIO_GPCLR1_38     6
#define GPIO_GPCLR1_39     7
#define GPIO_GPCLR1_40     8
#define GPIO_GPCLR1_41     9
#define GPIO_GPCLR1_42     10
#define GPIO_GPCLR1_43     11
#define GPIO_GPCLR1_44     12
#define GPIO_GPCLR1_45     13
#define GPIO_GPCLR1_46     14
#define GPIO_GPCLR1_47     15
#define GPIO_GPCLR1_48     16
#define GPIO_GPCLR1_49     17
#define GPIO_GPCLR1_50     18
#define GPIO_GPCLR1_51     19
#define GPIO_GPCLR1_52     20
#define GPIO_GPCLR1_53     21

#define GPIO_GPLEV0_0      0
#define GPIO_GPLEV0_1      1
#define GPIO_GPLEV0_2      2
#define GPIO_GPLEV0_3      3
#define GPIO_GPLEV0_4      4
#define GPIO_GPLEV0_5      5
#define GPIO_GPLEV0_6      6
#define GPIO_GPLEV0_7      7
#define GPIO_GPLEV0_8      8
#define GPIO_GPLEV0_9      9
#define GPIO_GPLEV0_10     10
#define GPIO_GPLEV0_11     11
#define GPIO_GPLEV0_12     12
#define GPIO_GPLEV0_13     13
#define GPIO_GPLEV0_14     14
#define GPIO_GPLEV0_15     15
#define GPIO_GPLEV0_16     16
#define GPIO_GPLEV0_17     17
#define GPIO_GPLEV0_18     18
#define GPIO_GPLEV0_19     19
#define GPIO_GPLEV0_20     20
#define GPIO_GPLEV0_21     21
#define GPIO_GPLEV0_22     22
#define GPIO_GPLEV0_23     23
#define GPIO_GPLEV0_24     24
#define GPIO_GPLEV0_25     25
#define GPIO_GPLEV0_26     26
#define GPIO_GPLEV0_27     27
#define GPIO_GPLEV0_28     28
#define GPIO_GPLEV0_29     29
#define GPIO_GPLEV0_30     30
#define GPIO_GPLEV0_31     31
#define GPIO_GPLEV1_32     0
#define GPIO_GPLEV1_33     1
#define GPIO_GPLEV1_34     2
#define GPIO_GPLEV1_35     3
#define GPIO_GPLEV1_36     4
#define GPIO_GPLEV1_37     5
#define GPIO_GPLEV1_38     6
#define GPIO_GPLEV1_39     7
#define GPIO_GPLEV1_40     8
#define GPIO_GPLEV1_41     9
#define GPIO_GPLEV1_42     10
#define GPIO_GPLEV1_43     11
#define GPIO_GPLEV1_44     12
#define GPIO_GPLEV1_45     13
#define GPIO_GPLEV1_46     14
#define GPIO_GPLEV1_47     15
#define GPIO_GPLEV1_48     16
#define GPIO_GPLEV1_49     17
#define GPIO_GPLEV1_50     18
#define GPIO_GPLEV1_51     19
#define GPIO_GPLEV1_52     20
#define GPIO_GPLEV1_53     21

#define GPIO_GPEDS0_0      0
#define GPIO_GPEDS0_1      1
#define GPIO_GPEDS0_2      2
#define GPIO_GPEDS0_3      3
#define GPIO_GPEDS0_4      4
#define GPIO_GPEDS0_5      5
#define GPIO_GPEDS0_6      6
#define GPIO_GPEDS0_7      7
#define GPIO_GPEDS0_8      8
#define GPIO_GPEDS0_9      9
#define GPIO_GPEDS0_10     10
#define GPIO_GPEDS0_11     11
#define GPIO_GPEDS0_12     12
#define GPIO_GPEDS0_13     13
#define GPIO_GPEDS0_14     14
#define GPIO_GPEDS0_15     15
#define GPIO_GPEDS0_16     16
#define GPIO_GPEDS0_17     17
#define GPIO_GPEDS0_18     18
#define GPIO_GPEDS0_19     19
#define GPIO_GPEDS0_20     20
#define GPIO_GPEDS0_21     21
#define GPIO_GPEDS0_22     22
#define GPIO_GPEDS0_23     23
#define GPIO_GPEDS0_24     24
#define GPIO_GPEDS0_25     25
#define GPIO_GPEDS0_26     26
#define GPIO_GPEDS0_27     27
#define GPIO_GPEDS0_28     28
#define GPIO_GPEDS0_29     29
#define GPIO_GPEDS0_30     30
#define GPIO_GPEDS0_31     31
#define GPIO_GPEDS1_32     0
#define GPIO_GPEDS1_33     1
#define GPIO_GPEDS1_34     2
#define GPIO_GPEDS1_35     3
#define GPIO_GPEDS1_36     4
#define GPIO_GPEDS1_37     5
#define GPIO_GPEDS1_38     6
#define GPIO_GPEDS1_39     7
#define GPIO_GPEDS1_40     8
#define GPIO_GPEDS1_41     9
#define GPIO_GPEDS1_42     10
#define GPIO_GPEDS1_43     11
#define GPIO_GPEDS1_44     12
#define GPIO_GPEDS1_45     13
#define GPIO_GPEDS1_46     14
#define GPIO_GPEDS1_47     15
#define GPIO_GPEDS1_48     16
#define GPIO_GPEDS1_49     17
#define GPIO_GPEDS1_50     18
#define GPIO_GPEDS1_51     19
#define GPIO_GPEDS1_52     20
#define GPIO_GPEDS1_53     21

#define GPIO_GPREN0_0      0
#define GPIO_GPREN0_1      1
#define GPIO_GPREN0_2      2
#define GPIO_GPREN0_3      3
#define GPIO_GPREN0_4      4
#define GPIO_GPREN0_5      5
#define GPIO_GPREN0_6      6
#define GPIO_GPREN0_7      7
#define GPIO_GPREN0_8      8
#define GPIO_GPREN0_9      9
#define GPIO_GPREN0_10     10
#define GPIO_GPREN0_11     11
#define GPIO_GPREN0_12     12
#define GPIO_GPREN0_13     13
#define GPIO_GPREN0_14     14
#define GPIO_GPREN0_15     15
#define GPIO_GPREN0_16     16
#define GPIO_GPREN0_17     17
#define GPIO_GPREN0_18     18
#define GPIO_GPREN0_19     19
#define GPIO_GPREN0_20     20
#define GPIO_GPREN0_21     21
#define GPIO_GPREN0_22     22
#define GPIO_GPREN0_23     23
#define GPIO_GPREN0_24     24
#define GPIO_GPREN0_25     25
#define GPIO_GPREN0_26     26
#define GPIO_GPREN0_27     27
#define GPIO_GPREN0_28     28
#define GPIO_GPREN0_29     29
#define GPIO_GPREN0_30     30
#define GPIO_GPREN0_31     31
#define GPIO_GPREN1_32     0
#define GPIO_GPREN1_33     1
#define GPIO_GPREN1_34     2
#define GPIO_GPREN1_35     3
#define GPIO_GPREN1_36     4
#define GPIO_GPREN1_37     5
#define GPIO_GPREN1_38     6
#define GPIO_GPREN1_39     7
#define GPIO_GPREN1_40     8
#define GPIO_GPREN1_41     9
#define GPIO_GPREN1_42     10
#define GPIO_GPREN1_43     11
#define GPIO_GPREN1_44     12
#define GPIO_GPREN1_45     13
#define GPIO_GPREN1_46     14
#define GPIO_GPREN1_47     15
#define GPIO_GPREN1_48     16
#define GPIO_GPREN1_49     17
#define GPIO_GPREN1_50     18
#define GPIO_GPREN1_51     19
#define GPIO_GPREN1_52     20
#define GPIO_GPREN1_53     21

#define GPIO_GPFEN0_0      0
#define GPIO_GPFEN0_1      1
#define GPIO_GPFEN0_2      2
#define GPIO_GPFEN0_3      3
#define GPIO_GPFEN0_4      4
#define GPIO_GPFEN0_5      5
#define GPIO_GPFEN0_6      6
#define GPIO_GPFEN0_7      7
#define GPIO_GPFEN0_8      8
#define GPIO_GPFEN0_9      9
#define GPIO_GPFEN0_10     10
#define GPIO_GPFEN0_11     11
#define GPIO_GPFEN0_12     12
#define GPIO_GPFEN0_13     13
#define GPIO_GPFEN0_14     14
#define GPIO_GPFEN0_15     15
#define GPIO_GPFEN0_16     16
#define GPIO_GPFEN0_17     17
#define GPIO_GPFEN0_18     18
#define GPIO_GPFEN0_19     19
#define GPIO_GPFEN0_20     20
#define GPIO_GPFEN0_21     21
#define GPIO_GPFEN0_22     22
#define GPIO_GPFEN0_23     23
#define GPIO_GPFEN0_24     24
#define GPIO_GPFEN0_25     25
#define GPIO_GPFEN0_26     26
#define GPIO_GPFEN0_27     27
#define GPIO_GPFEN0_28     28
#define GPIO_GPFEN0_29     29
#define GPIO_GPFEN0_30     30
#define GPIO_GPFEN0_31     31
#define GPIO_GPFEN1_32     0
#define GPIO_GPFEN1_33     1
#define GPIO_GPFEN1_34     2
#define GPIO_GPFEN1_35     3
#define GPIO_GPFEN1_36     4
#define GPIO_GPFEN1_37     5
#define GPIO_GPFEN1_38     6
#define GPIO_GPFEN1_39     7
#define GPIO_GPFEN1_40     8
#define GPIO_GPFEN1_41     9
#define GPIO_GPFEN1_42     10
#define GPIO_GPFEN1_43     11
#define GPIO_GPFEN1_44     12
#define GPIO_GPFEN1_45     13
#define GPIO_GPFEN1_46     14
#define GPIO_GPFEN1_47     15
#define GPIO_GPFEN1_48     16
#define GPIO_GPFEN1_49     17
#define GPIO_GPFEN1_50     18
#define GPIO_GPFEN1_51     19
#define GPIO_GPFEN1_52     20
#define GPIO_GPFEN1_53     21

#define GPIO_GPHEN0_0      0
#define GPIO_GPHEN0_1      1
#define GPIO_GPHEN0_2      2
#define GPIO_GPHEN0_3      3
#define GPIO_GPHEN0_4      4
#define GPIO_GPHEN0_5      5
#define GPIO_GPHEN0_6      6
#define GPIO_GPHEN0_7      7
#define GPIO_GPHEN0_8      8
#define GPIO_GPHEN0_9      9
#define GPIO_GPHEN0_10     10
#define GPIO_GPHEN0_11     11
#define GPIO_GPHEN0_12     12
#define GPIO_GPHEN0_13     13
#define GPIO_GPHEN0_14     14
#define GPIO_GPHEN0_15     15
#define GPIO_GPHEN0_16     16
#define GPIO_GPHEN0_17     17
#define GPIO_GPHEN0_18     18
#define GPIO_GPHEN0_19     19
#define GPIO_GPHEN0_20     20
#define GPIO_GPHEN0_21     21
#define GPIO_GPHEN0_22     22
#define GPIO_GPHEN0_23     23
#define GPIO_GPHEN0_24     24
#define GPIO_GPHEN0_25     25
#define GPIO_GPHEN0_26     26
#define GPIO_GPHEN0_27     27
#define GPIO_GPHEN0_28     28
#define GPIO_GPHEN0_29     29
#define GPIO_GPHEN0_30     30
#define GPIO_GPHEN0_31     31
#define GPIO_GPHEN1_32     0
#define GPIO_GPHEN1_33     1
#define GPIO_GPHEN1_34     2
#define GPIO_GPHEN1_35     3
#define GPIO_GPHEN1_36     4
#define GPIO_GPHEN1_37     5
#define GPIO_GPHEN1_38     6
#define GPIO_GPHEN1_39     7
#define GPIO_GPHEN1_40     8
#define GPIO_GPHEN1_41     9
#define GPIO_GPHEN1_42     10
#define GPIO_GPHEN1_43     11
#define GPIO_GPHEN1_44     12
#define GPIO_GPHEN1_45     13
#define GPIO_GPHEN1_46     14
#define GPIO_GPHEN1_47     15
#define GPIO_GPHEN1_48     16
#define GPIO_GPHEN1_49     17
#define GPIO_GPHEN1_50     18
#define GPIO_GPHEN1_51     19
#define GPIO_GPHEN1_52     20
#define GPIO_GPHEN1_53     21

#define GPIO_GPLEN0_0      0
#define GPIO_GPLEN0_1      1
#define GPIO_GPLEN0_2      2
#define GPIO_GPLEN0_3      3
#define GPIO_GPLEN0_4      4
#define GPIO_GPLEN0_5      5
#define GPIO_GPLEN0_6      6
#define GPIO_GPLEN0_7      7
#define GPIO_GPLEN0_8      8
#define GPIO_GPLEN0_9      9
#define GPIO_GPLEN0_10     10
#define GPIO_GPLEN0_11     11
#define GPIO_GPLEN0_12     12
#define GPIO_GPLEN0_13     13
#define GPIO_GPLEN0_14     14
#define GPIO_GPLEN0_15     15
#define GPIO_GPLEN0_16     16
#define GPIO_GPLEN0_17     17
#define GPIO_GPLEN0_18     18
#define GPIO_GPLEN0_19     19
#define GPIO_GPLEN0_20     20
#define GPIO_GPLEN0_21     21
#define GPIO_GPLEN0_22     22
#define GPIO_GPLEN0_23     23
#define GPIO_GPLEN0_24     24
#define GPIO_GPLEN0_25     25
#define GPIO_GPLEN0_26     26
#define GPIO_GPLEN0_27     27
#define GPIO_GPLEN0_28     28
#define GPIO_GPLEN0_29     29
#define GPIO_GPLEN0_30     30
#define GPIO_GPLEN0_31     31
#define GPIO_GPLEN1_32     0
#define GPIO_GPLEN1_33     1
#define GPIO_GPLEN1_34     2
#define GPIO_GPLEN1_35     3
#define GPIO_GPLEN1_36     4
#define GPIO_GPLEN1_37     5
#define GPIO_GPLEN1_38     6
#define GPIO_GPLEN1_39     7
#define GPIO_GPLEN1_40     8
#define GPIO_GPLEN1_41     9
#define GPIO_GPLEN1_42     10
#define GPIO_GPLEN1_43     11
#define GPIO_GPLEN1_44     12
#define GPIO_GPLEN1_45     13
#define GPIO_GPLEN1_46     14
#define GPIO_GPLEN1_47     15
#define GPIO_GPLEN1_48     16
#define GPIO_GPLEN1_49     17
#define GPIO_GPLEN1_50     18
#define GPIO_GPLEN1_51     19
#define GPIO_GPLEN1_52     20
#define GPIO_GPLEN1_53     21

#define GPIO_GPAREN0_0      0
#define GPIO_GPAREN0_1      1
#define GPIO_GPAREN0_2      2
#define GPIO_GPAREN0_3      3
#define GPIO_GPAREN0_4      4
#define GPIO_GPAREN0_5      5
#define GPIO_GPAREN0_6      6
#define GPIO_GPAREN0_7      7
#define GPIO_GPAREN0_8      8
#define GPIO_GPAREN0_9      9
#define GPIO_GPAREN0_10     10
#define GPIO_GPAREN0_11     11
#define GPIO_GPAREN0_12     12
#define GPIO_GPAREN0_13     13
#define GPIO_GPAREN0_14     14
#define GPIO_GPAREN0_15     15
#define GPIO_GPAREN0_16     16
#define GPIO_GPAREN0_17     17
#define GPIO_GPAREN0_18     18
#define GPIO_GPAREN0_19     19
#define GPIO_GPAREN0_20     20
#define GPIO_GPAREN0_21     21
#define GPIO_GPAREN0_22     22
#define GPIO_GPAREN0_23     23
#define GPIO_GPAREN0_24     24
#define GPIO_GPAREN0_25     25
#define GPIO_GPAREN0_26     26
#define GPIO_GPAREN0_27     27
#define GPIO_GPAREN0_28     28
#define GPIO_GPAREN0_29     29
#define GPIO_GPAREN0_30     30
#define GPIO_GPAREN0_31     31
#define GPIO_GPAREN1_32     0
#define GPIO_GPAREN1_33     1
#define GPIO_GPAREN1_34     2
#define GPIO_GPAREN1_35     3
#define GPIO_GPAREN1_36     4
#define GPIO_GPAREN1_37     5
#define GPIO_GPAREN1_38     6
#define GPIO_GPAREN1_39     7
#define GPIO_GPAREN1_40     8
#define GPIO_GPAREN1_41     9
#define GPIO_GPAREN1_42     10
#define GPIO_GPAREN1_43     11
#define GPIO_GPAREN1_44     12
#define GPIO_GPAREN1_45     13
#define GPIO_GPAREN1_46     14
#define GPIO_GPAREN1_47     15
#define GPIO_GPAREN1_48     16
#define GPIO_GPAREN1_49     17
#define GPIO_GPAREN1_50     18
#define GPIO_GPAREN1_51     19
#define GPIO_GPAREN1_52     20
#define GPIO_GPAREN1_53     21

#define GPIO_GPAFEN0_0      0
#define GPIO_GPAFEN0_1      1
#define GPIO_GPAFEN0_2      2
#define GPIO_GPAFEN0_3      3
#define GPIO_GPAFEN0_4      4
#define GPIO_GPAFEN0_5      5
#define GPIO_GPAFEN0_6      6
#define GPIO_GPAFEN0_7      7
#define GPIO_GPAFEN0_8      8
#define GPIO_GPAFEN0_9      9
#define GPIO_GPAFEN0_10     10
#define GPIO_GPAFEN0_11     11
#define GPIO_GPAFEN0_12     12
#define GPIO_GPAFEN0_13     13
#define GPIO_GPAFEN0_14     14
#define GPIO_GPAFEN0_15     15
#define GPIO_GPAFEN0_16     16
#define GPIO_GPAFEN0_17     17
#define GPIO_GPAFEN0_18     18
#define GPIO_GPAFEN0_19     19
#define GPIO_GPAFEN0_20     20
#define GPIO_GPAFEN0_21     21
#define GPIO_GPAFEN0_22     22
#define GPIO_GPAFEN0_23     23
#define GPIO_GPAFEN0_24     24
#define GPIO_GPAFEN0_25     25
#define GPIO_GPAFEN0_26     26
#define GPIO_GPAFEN0_27     27
#define GPIO_GPAFEN0_28     28
#define GPIO_GPAFEN0_29     29
#define GPIO_GPAFEN0_30     30
#define GPIO_GPAFEN0_31     31
#define GPIO_GPAFEN1_32     0
#define GPIO_GPAFEN1_33     1
#define GPIO_GPAFEN1_34     2
#define GPIO_GPAFEN1_35     3
#define GPIO_GPAFEN1_36     4
#define GPIO_GPAFEN1_37     5
#define GPIO_GPAFEN1_38     6
#define GPIO_GPAFEN1_39     7
#define GPIO_GPAFEN1_40     8
#define GPIO_GPAFEN1_41     9
#define GPIO_GPAFEN1_42     10
#define GPIO_GPAFEN1_43     11
#define GPIO_GPAFEN1_44     12
#define GPIO_GPAFEN1_45     13
#define GPIO_GPAFEN1_46     14
#define GPIO_GPAFEN1_47     15
#define GPIO_GPAFEN1_48     16
#define GPIO_GPAFEN1_49     17
#define GPIO_GPAFEN1_50     18
#define GPIO_GPAFEN1_51     19
#define GPIO_GPAFEN1_52     20
#define GPIO_GPAFEN1_53     21

#define GPIO_GPPUD_PUD             0

#define GPIO_GPPUDCLK0_0      0
#define GPIO_GPPUDCLK0_1      1
#define GPIO_GPPUDCLK0_2      2
#define GPIO_GPPUDCLK0_3      3
#define GPIO_GPPUDCLK0_4      4
#define GPIO_GPPUDCLK0_5      5
#define GPIO_GPPUDCLK0_6      6
#define GPIO_GPPUDCLK0_7      7
#define GPIO_GPPUDCLK0_8      8
#define GPIO_GPPUDCLK0_9      9
#define GPIO_GPPUDCLK0_10     10
#define GPIO_GPPUDCLK0_11     11
#define GPIO_GPPUDCLK0_12     12
#define GPIO_GPPUDCLK0_13     13
#define GPIO_GPPUDCLK0_14     14
#define GPIO_GPPUDCLK0_15     15
#define GPIO_GPPUDCLK0_16     16
#define GPIO_GPPUDCLK0_17     17
#define GPIO_GPPUDCLK0_18     18
#define GPIO_GPPUDCLK0_19     19
#define GPIO_GPPUDCLK0_20     20
#define GPIO_GPPUDCLK0_21     21
#define GPIO_GPPUDCLK0_22     22
#define GPIO_GPPUDCLK0_23     23
#define GPIO_GPPUDCLK0_24     24
#define GPIO_GPPUDCLK0_25     25
#define GPIO_GPPUDCLK0_26     26
#define GPIO_GPPUDCLK0_27     27
#define GPIO_GPPUDCLK0_28     28
#define GPIO_GPPUDCLK0_29     29
#define GPIO_GPPUDCLK0_30     30
#define GPIO_GPPUDCLK0_31     31
#define GPIO_GPPUDCLK1_32     0
#define GPIO_GPPUDCLK1_33     1
#define GPIO_GPPUDCLK1_34     2
#define GPIO_GPPUDCLK1_35     3
#define GPIO_GPPUDCLK1_36     4
#define GPIO_GPPUDCLK1_37     5
#define GPIO_GPPUDCLK1_38     6
#define GPIO_GPPUDCLK1_39     7
#define GPIO_GPPUDCLK1_40     8
#define GPIO_GPPUDCLK1_41     9
#define GPIO_GPPUDCLK1_42     10
#define GPIO_GPPUDCLK1_43     11
#define GPIO_GPPUDCLK1_44     12
#define GPIO_GPPUDCLK1_45     13
#define GPIO_GPPUDCLK1_46     14
#define GPIO_GPPUDCLK1_47     15
#define GPIO_GPPUDCLK1_48     16
#define GPIO_GPPUDCLK1_49     17
#define GPIO_GPPUDCLK1_50     18
#define GPIO_GPPUDCLK1_51     19
#define GPIO_GPPUDCLK1_52     20
#define GPIO_GPPUDCLK1_53     21




//GPIO Function Select Registers (GPFSELn) 
/*The function select registers are used to define the operation of the general-purpose I/O
pins. Each of the 54 GPIO pins has at least two alternative functions as defined in section
16.2. The FSEL{n} field determines the functionality of the nth GPIO pin. All unused
alternative function lines are tied to ground and will output a “0” if selected. All pins reset
to normal GPIO input operation.*/
typedef union 
{
    struct
    {
        /* FSEL0 - Function Select 0
        000 = GPIO Pin 0 is an input
        001 = GPIO Pin 0 is an output
        100 = GPIO Pin 0 takes alternate function 0
        101 = GPIO Pin 0 takes alternate function 1
        110 = GPIO Pin 0 takes alternate function 2
        111 = GPIO Pin 0 takes alternate function 3
        011 = GPIO Pin 0 takes alternate function 4
        010 = GPIO Pin 0 takes alternate function 5 */
        uint32_t FSEL0:3;
        /* FSEL1 - Function Select 1
        000 = GPIO Pin 1 is an input
        001 = GPIO Pin 1 is an output
        100 = GPIO Pin 1 takes alternate function 0
        101 = GPIO Pin 1 takes alternate function 1
        110 = GPIO Pin 1 takes alternate function 2
        111 = GPIO Pin 1 takes alternate function 3
        011 = GPIO Pin 1 takes alternate function 4
        010 = GPIO Pin 1 takes alternate function 5 */
        uint32_t FSEL1:3;        
        /*FSEL2 - Function Select 2
        000 = GPIO Pin 2 is an input
        001 = GPIO Pin 2 is an output
        100 = GPIO Pin 2 takes alternate function 0
        101 = GPIO Pin 2 takes alternate function 1
        110 = GPIO Pin 2 takes alternate function 2
        111 = GPIO Pin 2 takes alternate function 3
        011 = GPIO Pin 2 takes alternate function 4
        010 = GPIO Pin 2 takes alternate function 5 */
        uint32_t FSEL2:3;        
        /*FSEL3 - Function Select 3
        000 = GPIO Pin 3 is an input
        001 = GPIO Pin 3 is an output
        100 = GPIO Pin 3 takes alternate function 0
        101 = GPIO Pin 3 takes alternate function 1
        110 = GPIO Pin 3 takes alternate function 2
        111 = GPIO Pin 3 takes alternate function 3
        011 = GPIO Pin 3 takes alternate function 4
        010 = GPIO Pin 3 takes alternate function 5 */
        uint32_t FSEL3:3;        
        /*FSEL14 - Function Select 14
        000 = GPIO Pin 4 is an input
        001 = GPIO Pin 4 is an output
        100 = GPIO Pin 4 takes alternate function 0
        101 = GPIO Pin 4 takes alternate function 1
        110 = GPIO Pin 4 takes alternate function 2
        111 = GPIO Pin 4 takes alternate function 3
        011 = GPIO Pin 4 takes alternate function 4
        010 = GPIO Pin 4 takes alternate function 5 */
        uint32_t FSEL4:3;        
        /*FSEL5 - Function Select 5
        000 = GPIO Pin 5 is an input
        001 = GPIO Pin 5 is an output
        100 = GPIO Pin 5 takes alternate function 0
        101 = GPIO Pin 5 takes alternate function 1
        110 = GPIO Pin 5 takes alternate function 2
        111 = GPIO Pin 5 takes alternate function 3
        011 = GPIO Pin 5 takes alternate function 4
        010 = GPIO Pin 5 takes alternate function 5 */
        uint32_t FSEL5:3;
        /*FSEL6 - Function Select 6
        000 = GPIO Pin 6 is an input
        001 = GPIO Pin 6 is an output
        100 = GPIO Pin 6 takes alternate function 0
        101 = GPIO Pin 6 takes alternate function 1
        110 = GPIO Pin 6 takes alternate function 2
        111 = GPIO Pin 6 takes alternate function 3
        011 = GPIO Pin 6 takes alternate function 4
        010 = GPIO Pin 6 takes alternate function 5 */     
        uint32_t FSEL6:3;
        /*FSEL7 - Function Select 7
        000 = GPIO Pin 7 is an input
        001 = GPIO Pin 7 is an output
        100 = GPIO Pin 7 takes alternate function 0
        101 = GPIO Pin 7 takes alternate function 1
        110 = GPIO Pin 7 takes alternate function 2
        111 = GPIO Pin 7 takes alternate function 3
        011 = GPIO Pin 7 takes alternate function 4
        010 = GPIO Pin 7 takes alternate function 5 */     
        uint32_t FSEL7:3;
        /*FSEL8 - Function Select 8
        000 = GPIO Pin 8 is an input
        001 = GPIO Pin 8 is an output
        100 = GPIO Pin 8 takes alternate function 0
        101 = GPIO Pin 8 takes alternate function 1
        110 = GPIO Pin 8 takes alternate function 2
        111 = GPIO Pin 8 takes alternate function 3
        011 = GPIO Pin 8 takes alternate function 4
        010 = GPIO Pin 8 takes alternate function 5 */     
        uint32_t FSEL8:3;
        /*FSEL9 - Function Select 9
        000 = GPIO Pin 9 is an input
        001 = GPIO Pin 9 is an output
        100 = GPIO Pin 9 takes alternate function 0
        101 = GPIO Pin 9 takes alternate function 1
        110 = GPIO Pin 9 takes alternate function 2
        111 = GPIO Pin 9 takes alternate function 3
        011 = GPIO Pin 9 takes alternate function 4
        010 = GPIO Pin 9 takes alternate function 5 */     
        uint32_t FSEL9:3;
        /*Reserved  */   
        uint32_t Reserved1:2;  
    }bit;
    uint32_t reg;
}GPIO_GPFSEL0_Type;

//GPIO Function Select Registers (GPFSELn) 
/*The function select registers are used to define the operation of the general-purpose I/O
pins. Each of the 54 GPIO pins has at least two alternative functions as defined in section
16.2. The FSEL{n} field determines the functionality of the nth GPIO pin. All unused
alternative function lines are tied to ground and will output a “0” if selected. All pins reset
to normal GPIO input operation.*/
typedef union 
{
    struct
    {
        /* FSEL10 - Function Select 10
        000 = GPIO Pin 10 is an input
        001 = GPIO Pin 10 is an output
        100 = GPIO Pin 10 takes alternate function 0
        101 = GPIO Pin 10 takes alternate function 1
        110 = GPIO Pin 10 takes alternate function 2
        111 = GPIO Pin 10 takes alternate function 3
        011 = GPIO Pin 10 takes alternate function 4
        010 = GPIO Pin 10 takes alternate function 5 */
        uint32_t FSEL10:3;
        /* FSEL11 - Function Select 11
        000 = GPIO Pin 11 is an input
        001 = GPIO Pin 11 is an output
        100 = GPIO Pin 11 takes alternate function 0
        101 = GPIO Pin 11 takes alternate function 1
        110 = GPIO Pin 11 takes alternate function 2
        111 = GPIO Pin 11 takes alternate function 3
        011 = GPIO Pin 11 takes alternate function 4
        010 = GPIO Pin 11 takes alternate function 5 */
        uint32_t FSEL11:3;        
        /*FSEL12 - Function Select 12
        000 = GPIO Pin 12 is an input
        001 = GPIO Pin 12 is an output
        100 = GPIO Pin 12 takes alternate function 0
        101 = GPIO Pin 12 takes alternate function 1
        110 = GPIO Pin 12 takes alternate function 2
        111 = GPIO Pin 12 takes alternate function 3
        011 = GPIO Pin 12 takes alternate function 4
        010 = GPIO Pin 12 takes alternate function 5 */
        uint32_t FSEL12:3;        
        /*FSEL13 - Function Select 13
        000 = GPIO Pin 13 is an input
        001 = GPIO Pin 13 is an output
        100 = GPIO Pin 13 takes alternate function 0
        101 = GPIO Pin 13 takes alternate function 1
        110 = GPIO Pin 13 takes alternate function 2
        111 = GPIO Pin 13 takes alternate function 3
        011 = GPIO Pin 13 takes alternate function 4
        010 = GPIO Pin 13 takes alternate function 5 */
        uint32_t FSEL13:3;        
        /*FSEL4 - Function Select 4
        000 = GPIO Pin 14 is an input
        001 = GPIO Pin 14 is an output
        100 = GPIO Pin 14 takes alternate function 0
        101 = GPIO Pin 14 takes alternate function 1
        110 = GPIO Pin 14 takes alternate function 2
        111 = GPIO Pin 14 takes alternate function 3
        011 = GPIO Pin 14 takes alternate function 4
        010 = GPIO Pin 14 takes alternate function 5 */
        uint32_t FSEL14:3;        
        /*FSEL5 - Function Select 5
        000 = GPIO Pin 15 is an input
        001 = GPIO Pin 15 is an output
        100 = GPIO Pin 15 takes alternate function 0
        101 = GPIO Pin 15 takes alternate function 1
        110 = GPIO Pin 15 takes alternate function 2
        111 = GPIO Pin 15 takes alternate function 3
        011 = GPIO Pin 15 takes alternate function 4
        010 = GPIO Pin 15 takes alternate function 5 */
        uint32_t FSEL15:3;
        /*FSEL16 - Function Select 16
        000 = GPIO Pin 16 is an input
        001 = GPIO Pin 16 is an output
        100 = GPIO Pin 16 takes alternate function 0
        101 = GPIO Pin 16 takes alternate function 1
        110 = GPIO Pin 16 takes alternate function 2
        111 = GPIO Pin 16 takes alternate function 3
        011 = GPIO Pin 16 takes alternate function 4
        010 = GPIO Pin 16 takes alternate function 5 */     
        uint32_t FSEL16:3;
        /*FSEL17 - Function Select 17
        000 = GPIO Pin 7 is an input
        001 = GPIO Pin 7 is an output
        100 = GPIO Pin 7 takes alternate function 0
        101 = GPIO Pin 7 takes alternate function 1
        110 = GPIO Pin 7 takes alternate function 2
        111 = GPIO Pin 7 takes alternate function 3
        011 = GPIO Pin 7 takes alternate function 4
        010 = GPIO Pin 7 takes alternate function 5 */     
        uint32_t FSEL17:3;
        /*FSEL18 - Function Select 18
        000 = GPIO Pin 18 is an input
        001 = GPIO Pin 18 is an output
        100 = GPIO Pin 18 takes alternate function 0
        101 = GPIO Pin 18 takes alternate function 1
        110 = GPIO Pin 18 takes alternate function 2
        111 = GPIO Pin 18 takes alternate function 3
        011 = GPIO Pin 18 takes alternate function 4
        010 = GPIO Pin 18 takes alternate function 5 */     
        uint32_t FSEL18:3;
        /*FSEL19 - Function Select 19
        000 = GPIO Pin 19 is an input
        001 = GPIO Pin 19 is an output
        100 = GPIO Pin 19 takes alternate function 0
        101 = GPIO Pin 19 takes alternate function 1
        110 = GPIO Pin 19 takes alternate function 2
        111 = GPIO Pin 19 takes alternate function 3
        011 = GPIO Pin 19 takes alternate function 4
        010 = GPIO Pin 19 takes alternate function 5 */     
        uint32_t FSEL19:3;
        /*Reserved  */   
        uint32_t Reserved1:2;  
    }bit;
    uint32_t reg;
}GPIO_GPFSEL1_Type;

//GPIO Function Select Registers (GPFSELn) 
/*The function select registers are used to define the operation of the general-purpose I/O
pins. Each of the 54 GPIO pins has at least two alternative functions as defined in section
16.2. The FSEL{n} field determines the functionality of the nth GPIO pin. All unused
alternative function lines are tied to ground and will output a “0” if selected. All pins reset
to normal GPIO input operation.*/
typedef union 
{
    struct
    {
        /* FSEL20 - Function Select 20
        000 = GPIO Pin 20 is an input
        001 = GPIO Pin 20 is an output
        100 = GPIO Pin 20 takes alternate function 0
        101 = GPIO Pin 20 takes alternate function 1
        110 = GPIO Pin 20 takes alternate function 2
        111 = GPIO Pin 20 takes alternate function 3
        011 = GPIO Pin 20 takes alternate function 4
        010 = GPIO Pin 20 takes alternate function 5 */
        uint32_t FSEL20:3;
        /* FSEL21 - Function Select 21
        000 = GPIO Pin 21 is an input
        001 = GPIO Pin 21 is an output
        100 = GPIO Pin 21 takes alternate function 0
        101 = GPIO Pin 21 takes alternate function 1
        110 = GPIO Pin 21 takes alternate function 2
        111 = GPIO Pin 21 takes alternate function 3
        011 = GPIO Pin 21 takes alternate function 4
        010 = GPIO Pin 21 takes alternate function 5 */
        uint32_t FSEL21:3;        
        /*FSEL22 - Function Select 22
        000 = GPIO Pin 22 is an input
        001 = GPIO Pin 22 is an output
        100 = GPIO Pin 22 takes alternate function 0
        101 = GPIO Pin 22 takes alternate function 1
        110 = GPIO Pin 22 takes alternate function 2
        111 = GPIO Pin 22 takes alternate function 3
        011 = GPIO Pin 22 takes alternate function 4
        010 = GPIO Pin 22 takes alternate function 5 */
        uint32_t FSEL22:3;        
        /*FSEL23 - Function Select 23
        000 = GPIO Pin 23 is an input
        001 = GPIO Pin 23 is an output
        100 = GPIO Pin 23 takes alternate function 0
        101 = GPIO Pin 23 takes alternate function 1
        110 = GPIO Pin 23 takes alternate function 2
        111 = GPIO Pin 23 takes alternate function 3
        011 = GPIO Pin 23 takes alternate function 4
        010 = GPIO Pin 23 takes alternate function 5 */
        uint32_t FSEL23:3;        
        /*FSEL24 - Function Select 24
        000 = GPIO Pin 24 is an input
        001 = GPIO Pin 24 is an output
        100 = GPIO Pin 24 takes alternate function 0
        101 = GPIO Pin 24 takes alternate function 1
        110 = GPIO Pin 24 takes alternate function 2
        111 = GPIO Pin 24 takes alternate function 3
        011 = GPIO Pin 24 takes alternate function 4
        010 = GPIO Pin 24 takes alternate function 5 */
        uint32_t FSEL24:3;        
        /*FSEL25 - Function Select 25
        000 = GPIO Pin 25 is an input
        001 = GPIO Pin 25 is an output
        100 = GPIO Pin 25 takes alternate function 0
        101 = GPIO Pin 25 takes alternate function 1
        110 = GPIO Pin 25 takes alternate function 2
        111 = GPIO Pin 25 takes alternate function 3
        011 = GPIO Pin 25 takes alternate function 4
        010 = GPIO Pin 25 takes alternate function 5 */
        uint32_t FSEL25:3;
        /*FSEL26 - Function Select 26
        000 = GPIO Pin 26 is an input
        001 = GPIO Pin 26 is an output
        100 = GPIO Pin 26 takes alternate function 0
        101 = GPIO Pin 26 takes alternate function 1
        110 = GPIO Pin 26 takes alternate function 2
        111 = GPIO Pin 26 takes alternate function 3
        011 = GPIO Pin 26 takes alternate function 4
        010 = GPIO Pin 26 takes alternate function 5 */     
        uint32_t FSEL26:3;
        /*FSEL27 - Function Select 27
        000 = GPIO Pin 7 is an input
        001 = GPIO Pin 7 is an output
        100 = GPIO Pin 7 takes alternate function 0
        101 = GPIO Pin 7 takes alternate function 1
        110 = GPIO Pin 7 takes alternate function 2
        111 = GPIO Pin 7 takes alternate function 3
        011 = GPIO Pin 7 takes alternate function 4
        010 = GPIO Pin 7 takes alternate function 5 */     
        uint32_t FSEL27:3;
        /*FSEL28 - Function Select 28
        000 = GPIO Pin 28 is an input
        001 = GPIO Pin 28 is an output
        100 = GPIO Pin 28 takes alternate function 0
        101 = GPIO Pin 28 takes alternate function 1
        110 = GPIO Pin 28 takes alternate function 2
        111 = GPIO Pin 28 takes alternate function 3
        011 = GPIO Pin 28 takes alternate function 4
        010 = GPIO Pin 28 takes alternate function 5 */     
        uint32_t FSEL28:3;
        /*FSEL29 - Function Select 29
        000 = GPIO Pin 29 is an input
        001 = GPIO Pin 29 is an output
        100 = GPIO Pin 29 takes alternate function 0
        101 = GPIO Pin 29 takes alternate function 1
        110 = GPIO Pin 29 takes alternate function 2
        111 = GPIO Pin 29 takes alternate function 3
        011 = GPIO Pin 29 takes alternate function 4
        010 = GPIO Pin 29 takes alternate function 5 */     
        uint32_t FSEL29:3;
        /*Reserved  */   
        uint32_t Reserved1:2;  
    }bit;
    uint32_t reg;
}GPIO_GPFSEL2_Type;

//GPIO Function Select Registers (GPFSELn) 
/*The function select registers are used to define the operation of the general-purpose I/O
pins. Each of the 54 GPIO pins has at least two alternative functions as defined in section
16.2. The FSEL{n} field determines the functionality of the nth GPIO pin. All unused
alternative function lines are tied to ground and will output a “0” if selected. All pins reset
to normal GPIO input operation.*/
typedef union 
{
    struct
    {
        /* FSEL30 - Function Select 30
        000 = GPIO Pin 30 is an input
        001 = GPIO Pin 30 is an output
        100 = GPIO Pin 30 takes alternate function 0
        101 = GPIO Pin 30 takes alternate function 1
        110 = GPIO Pin 30 takes alternate function 2
        111 = GPIO Pin 30 takes alternate function 3
        011 = GPIO Pin 30 takes alternate function 4
        010 = GPIO Pin 30 takes alternate function 5 */
        uint32_t FSEL30:3;
        /* FSEL31 - Function Select 31
        000 = GPIO Pin 31 is an input
        001 = GPIO Pin 31 is an output
        100 = GPIO Pin 31 takes alternate function 0
        101 = GPIO Pin 31 takes alternate function 1
        110 = GPIO Pin 31 takes alternate function 2
        111 = GPIO Pin 31 takes alternate function 3
        011 = GPIO Pin 31 takes alternate function 4
        010 = GPIO Pin 31 takes alternate function 5 */
        uint32_t FSEL31:3;        
        /*FSEL32 - Function Select 32
        000 = GPIO Pin 32 is an input
        001 = GPIO Pin 32 is an output
        100 = GPIO Pin 32 takes alternate function 0
        101 = GPIO Pin 32 takes alternate function 1
        110 = GPIO Pin 32 takes alternate function 2
        111 = GPIO Pin 32 takes alternate function 3
        011 = GPIO Pin 32 takes alternate function 4
        010 = GPIO Pin 32 takes alternate function 5 */
        uint32_t FSEL32:3;        
        /*FSEL33 - Function Select 33
        000 = GPIO Pin 33 is an input
        001 = GPIO Pin 33 is an output
        100 = GPIO Pin 33 takes alternate function 0
        101 = GPIO Pin 33 takes alternate function 1
        110 = GPIO Pin 33 takes alternate function 2
        111 = GPIO Pin 33 takes alternate function 3
        011 = GPIO Pin 33 takes alternate function 4
        010 = GPIO Pin 33 takes alternate function 5 */
        uint32_t FSEL33:3;        
        /*FSEL34 - Function Select 34
        000 = GPIO Pin 34 is an input
        001 = GPIO Pin 34 is an output
        100 = GPIO Pin 34 takes alternate function 0
        101 = GPIO Pin 34 takes alternate function 1
        110 = GPIO Pin 34 takes alternate function 2
        111 = GPIO Pin 34 takes alternate function 3
        011 = GPIO Pin 34 takes alternate function 4
        010 = GPIO Pin 34 takes alternate function 5 */
        uint32_t FSEL34:3;        
        /*FSEL35 - Function Select 35
        000 = GPIO Pin 35 is an input
        001 = GPIO Pin 35 is an output
        100 = GPIO Pin 35 takes alternate function 0
        101 = GPIO Pin 35 takes alternate function 1
        110 = GPIO Pin 35 takes alternate function 2
        111 = GPIO Pin 35 takes alternate function 3
        011 = GPIO Pin 35 takes alternate function 4
        010 = GPIO Pin 35 takes alternate function 5 */
        uint32_t FSEL35:3;
        /*FSEL36 - Function Select 36
        000 = GPIO Pin 36 is an input
        001 = GPIO Pin 36 is an output
        100 = GPIO Pin 36 takes alternate function 0
        101 = GPIO Pin 36 takes alternate function 1
        110 = GPIO Pin 36 takes alternate function 2
        111 = GPIO Pin 36 takes alternate function 3
        011 = GPIO Pin 36 takes alternate function 4
        010 = GPIO Pin 36 takes alternate function 5 */     
        uint32_t FSEL36:3;
        /*FSEL37 - Function Select 37
        000 = GPIO Pin 7 is an input
        001 = GPIO Pin 7 is an output
        100 = GPIO Pin 7 takes alternate function 0
        101 = GPIO Pin 7 takes alternate function 1
        110 = GPIO Pin 7 takes alternate function 2
        111 = GPIO Pin 7 takes alternate function 3
        011 = GPIO Pin 7 takes alternate function 4
        010 = GPIO Pin 7 takes alternate function 5 */     
        uint32_t FSEL37:3;
        /*FSEL38 - Function Select 38
        000 = GPIO Pin 38 is an input
        001 = GPIO Pin 38 is an output
        100 = GPIO Pin 38 takes alternate function 0
        101 = GPIO Pin 38 takes alternate function 1
        110 = GPIO Pin 38 takes alternate function 2
        111 = GPIO Pin 38 takes alternate function 3
        011 = GPIO Pin 38 takes alternate function 4
        010 = GPIO Pin 38 takes alternate function 5 */     
        uint32_t FSEL38:3;
        /*FSEL39 - Function Select 39
        000 = GPIO Pin 39 is an input
        001 = GPIO Pin 39 is an output
        100 = GPIO Pin 39 takes alternate function 0
        101 = GPIO Pin 39 takes alternate function 1
        110 = GPIO Pin 39 takes alternate function 2
        111 = GPIO Pin 39 takes alternate function 3
        011 = GPIO Pin 39 takes alternate function 4
        010 = GPIO Pin 39 takes alternate function 5 */     
        uint32_t FSEL39:3;
        /*Reserved  */   
        uint32_t Reserved1:2;  
    }bit;
    uint32_t reg;
}GPIO_GPFSEL3_Type;

//GPIO Function Select Registers (GPFSELn) 
/*The function select registers are used to define the operation of the general-purpose I/O
pins. Each of the 54 GPIO pins has at least two alternative functions as defined in section
16.2. The FSEL{n} field determines the functionality of the nth GPIO pin. All unused
alternative function lines are tied to ground and will output a “0” if selected. All pins reset
to normal GPIO input operation.*/
typedef union 
{
    struct
    {
        /* FSEL40 - Function Select 40
        000 = GPIO Pin 40 is an input
        001 = GPIO Pin 40 is an output
        100 = GPIO Pin 40 takes alternate function 0
        101 = GPIO Pin 40 takes alternate function 1
        110 = GPIO Pin 40 takes alternate function 2
        111 = GPIO Pin 40 takes alternate function 3
        011 = GPIO Pin 40 takes alternate function 4
        010 = GPIO Pin 40 takes alternate function 5 */
        uint32_t FSEL40:3;
        /* FSEL41 - Function Select 41
        000 = GPIO Pin 41 is an input
        001 = GPIO Pin 41 is an output
        100 = GPIO Pin 41 takes alternate function 0
        101 = GPIO Pin 41 takes alternate function 1
        110 = GPIO Pin 41 takes alternate function 2
        111 = GPIO Pin 41 takes alternate function 3
        011 = GPIO Pin 41 takes alternate function 4
        010 = GPIO Pin 41 takes alternate function 5 */
        uint32_t FSEL41:3;        
        /*FSEL42 - Function Select 42
        000 = GPIO Pin 42 is an input
        001 = GPIO Pin 42 is an output
        100 = GPIO Pin 42 takes alternate function 0
        101 = GPIO Pin 42 takes alternate function 1
        110 = GPIO Pin 42 takes alternate function 2
        111 = GPIO Pin 42 takes alternate function 3
        011 = GPIO Pin 42 takes alternate function 4
        010 = GPIO Pin 42 takes alternate function 5 */
        uint32_t FSEL42:3;        
        /*FSEL43 - Function Select 43
        000 = GPIO Pin 43 is an input
        001 = GPIO Pin 43 is an output
        100 = GPIO Pin 43 takes alternate function 0
        101 = GPIO Pin 43 takes alternate function 1
        110 = GPIO Pin 43 takes alternate function 2
        111 = GPIO Pin 43 takes alternate function 3
        011 = GPIO Pin 43 takes alternate function 4
        010 = GPIO Pin 43 takes alternate function 5 */
        uint32_t FSEL43:3;        
        /*FSEL44 - Function Select 44
        000 = GPIO Pin 44 is an input
        001 = GPIO Pin 44 is an output
        100 = GPIO Pin 44 takes alternate function 0
        101 = GPIO Pin 44 takes alternate function 1
        110 = GPIO Pin 44 takes alternate function 2
        111 = GPIO Pin 44 takes alternate function 3
        011 = GPIO Pin 44 takes alternate function 4
        010 = GPIO Pin 44 takes alternate function 5 */
        uint32_t FSEL44:3;        
        /*FSEL45 - Function Select 45
        000 = GPIO Pin 45 is an input
        001 = GPIO Pin 45 is an output
        100 = GPIO Pin 45 takes alternate function 0
        101 = GPIO Pin 45 takes alternate function 1
        110 = GPIO Pin 45 takes alternate function 2
        111 = GPIO Pin 45 takes alternate function 3
        011 = GPIO Pin 45 takes alternate function 4
        010 = GPIO Pin 45 takes alternate function 5 */
        uint32_t FSEL45:3;
        /*FSEL46 - Function Select 46
        000 = GPIO Pin 46 is an input
        001 = GPIO Pin 46 is an output
        100 = GPIO Pin 46 takes alternate function 0
        101 = GPIO Pin 46 takes alternate function 1
        110 = GPIO Pin 46 takes alternate function 2
        111 = GPIO Pin 46 takes alternate function 3
        011 = GPIO Pin 46 takes alternate function 4
        010 = GPIO Pin 46 takes alternate function 5 */     
        uint32_t FSEL46:3;
        /*FSEL47 - Function Select 47
        000 = GPIO Pin 7 is an input
        001 = GPIO Pin 7 is an output
        100 = GPIO Pin 7 takes alternate function 0
        101 = GPIO Pin 7 takes alternate function 1
        110 = GPIO Pin 7 takes alternate function 2
        111 = GPIO Pin 7 takes alternate function 3
        011 = GPIO Pin 7 takes alternate function 4
        010 = GPIO Pin 7 takes alternate function 5 */     
        uint32_t FSEL47:3;
        /*FSEL48 - Function Select 48
        000 = GPIO Pin 48 is an input
        001 = GPIO Pin 48 is an output
        100 = GPIO Pin 48 takes alternate function 0
        101 = GPIO Pin 48 takes alternate function 1
        110 = GPIO Pin 48 takes alternate function 2
        111 = GPIO Pin 48 takes alternate function 3
        011 = GPIO Pin 48 takes alternate function 4
        010 = GPIO Pin 48 takes alternate function 5 */     
        uint32_t FSEL48:3;
        /*FSEL49 - Function Select 49
        000 = GPIO Pin 49 is an input
        001 = GPIO Pin 49 is an output
        100 = GPIO Pin 49 takes alternate function 0
        101 = GPIO Pin 49 takes alternate function 1
        110 = GPIO Pin 49 takes alternate function 2
        111 = GPIO Pin 49 takes alternate function 3
        011 = GPIO Pin 49 takes alternate function 4
        010 = GPIO Pin 49 takes alternate function 5 */     
        uint32_t FSEL49:3;
        /*Reserved  */   
        uint32_t Reserved1:2;  
    }bit;
    uint32_t reg;
}GPIO_GPFSEL4_Type;

//GPIO Function Select Registers (GPFSELn) 
/*The function select registers are used to define the operation of the general-purpose I/O
pins. Each of the 54 GPIO pins has at least two alternative functions as defined in section
16.2. The FSEL{n} field determines the functionality of the nth GPIO pin. All unused
alternative function lines are tied to ground and will output a “0” if selected. All pins reset
to normal GPIO input operation.*/
typedef union 
{
    struct
    {
        /* FSEL50 - Function Select 50
        000 = GPIO Pin 50 is an input
        001 = GPIO Pin 50 is an output
        100 = GPIO Pin 50 takes alternate function 0
        101 = GPIO Pin 50 takes alternate function 1
        110 = GPIO Pin 50 takes alternate function 2
        111 = GPIO Pin 50 takes alternate function 3
        011 = GPIO Pin 50 takes alternate function 4
        010 = GPIO Pin 50 takes alternate function 5 */
        uint32_t FSEL50:3;
        /* FSEL51 - Function Select 51
        000 = GPIO Pin 51 is an input
        001 = GPIO Pin 51 is an output
        100 = GPIO Pin 51 takes alternate function 0
        101 = GPIO Pin 51 takes alternate function 1
        110 = GPIO Pin 51 takes alternate function 2
        111 = GPIO Pin 51 takes alternate function 3
        011 = GPIO Pin 51 takes alternate function 4
        010 = GPIO Pin 51 takes alternate function 5 */
        uint32_t FSEL51:3;        
        /*FSEL52 - Function Select 52
        000 = GPIO Pin 52 is an input
        001 = GPIO Pin 52 is an output
        100 = GPIO Pin 52 takes alternate function 0
        101 = GPIO Pin 52 takes alternate function 1
        110 = GPIO Pin 52 takes alternate function 2
        111 = GPIO Pin 52 takes alternate function 3
        011 = GPIO Pin 52 takes alternate function 4
        010 = GPIO Pin 52 takes alternate function 5 */
        uint32_t FSEL52:3;        
        /*FSEL53 - Function Select 53
        000 = GPIO Pin 53 is an input
        001 = GPIO Pin 53 is an output
        100 = GPIO Pin 53 takes alternate function 0
        101 = GPIO Pin 53 takes alternate function 1
        110 = GPIO Pin 53 takes alternate function 2
        111 = GPIO Pin 53 takes alternate function 3
        011 = GPIO Pin 53 takes alternate function 4
        010 = GPIO Pin 53 takes alternate function 5 */
        uint32_t FSEL53:3;        
        /*Reserved  */   
        uint32_t Reserved1:20;  
    }bit;
    uint32_t reg;
}GPIO_GPFSEL5_Type;


/*GPIO Pin Output Set Registers (GPSETn) 
SYNOPSIS The output set registers are used to set a GPIO pin. The SET{n} field defines the
respective GPIO pin to set, writing a “0” to the field has no effect. If the GPIO pin is
being used as in input (by default) then the value in the SET{n} field is ignored.
However, if the pin is subsequently defined as an output then the bit will be set
according to the last set/clear operation. Separating the set and clear functions
removes the need for read-modify-write operations  */
typedef union 
{
    struct
    {
        /*SETn (n=0..31) 
        0 = No effect
        1 = Set GPIO pin n
        R/W 0 */
        uint32_t SET:32;
    }bit;
    uint32_t reg;
}GPIO_GPSET0_Type;


/*GPIO Pin Output Set Registers (GPSETn) 
SYNOPSIS The output set registers are used to set a GPIO pin. The SET{n} field defines the
respective GPIO pin to set, writing a “0” to the field has no effect. If the GPIO pin is
being used as in input (by default) then the value in the SET{n} field is ignored.
However, if the pin is subsequently defined as an output then the bit will be set
according to the last set/clear operation. Separating the set and clear functions
removes the need for read-modify-write operations  */
typedef union 
{
    struct
    {
        /*SETn (n=32..53)
        0 = No effect
        1 = Set GPIO pin n.
        R/W 0  */
        uint32_t SET:22;
        /*Reserved*/
        uint32_t Reserved1:10;
    }bit;
    uint32_t reg;
}GPIO_GPSET1_Type;

/*GPIO Pin Output Clear Registers (GPCLRn) 
SYNOPSIS The output clear registers) are used to clear a GPIO pin. The CLR{n} field defines
the respective GPIO pin to clear, writing a “0” to the field has no effect. If the GPIO
pin is being used as in input (by default) then the value in the CLR{n} field is
ignored. However, if the pin is subsequently defined as an output then the bit will
be set according to the last set/clear operation. Separating the set and clear
functions removes the need for read-modify-write operations.   */
typedef union 
{
    struct
    {
        /*CLRn (n=0..31) 0 = No effect
                        1 = Clear GPIO pin n
        R/W 0 */
        uint32_t CLR:32;
    }bit;
    uint32_t reg;
}GPIO_GPCLR0_Type;

/*GPIO Pin Output Clear Registers (GPCLRn) 
SYNOPSIS The output clear registers) are used to clear a GPIO pin. The CLR{n} field defines
the respective GPIO pin to clear, writing a “0” to the field has no effect. If the GPIO
pin is being used as in input (by default) then the value in the CLR{n} field is
ignored. However, if the pin is subsequently defined as an output then the bit will
be set according to the last set/clear operation. Separating the set and clear
functions removes the need for read-modify-write operations.   */
typedef union 
{
    struct
    {
        /*CLRn (n=0..31) 0 = No effect
                        1 = Clear GPIO pin n
        R/W 0 */
        uint32_t CLR:22;
        /*Reserved*/
        uint32_t Reserved1:10;
    }bit;
    uint32_t reg;
}GPIO_GPCLR1_Type;

/*GPIO Pin Level Registers (GPLEVn) 
SYNOPSIS The pin level registers return the actual value of the pin. The LEV{n} field gives the
value of the respective GPIO pin.   */
typedef union 
{
    struct
    {
        /*LEVn (n=0..31)
        R/W 0  */
        uint32_t LEV:32;
    }bit;
    uint32_t reg;
}GPIO_GPLEV0_Type;

/*GPIO Pin Level Registers (GPLEVn) 
SYNOPSIS The pin level registers return the actual value of the pin. The LEV{n} field gives the
value of the respective GPIO pin.   */
typedef union 
{
    struct
    {
        /*LEVn (n=32..53)
        R/W 0 */
        uint32_t LEV:22;
        /*Reserved*/
        uint32_t Reserved1:10;
    }bit;
    uint32_t reg;
}GPIO_GPLEV1_Type;

/*GPIO Event Detect Status Registers (GPEDSn) 
SYNOPSIS The event detect status registers are used to record level and edge events on the
GPIO pins. The relevant bit in the event detect status registers is set whenever: 1)
an edge is detected that matches the type of edge programmed in the rising/falling
edge detect enable registers, or 2) a level is detected that matches the type of level
programmed in the high/low level detect enable registers. The bit is cleared by
writing a “1” to the relevant bit.
The interrupt controller can be programmed to interrupt the processor when any of
the status bits are set. The GPIO peripheral has three dedicated interrupt lines.
Each GPIO bank can generate an independent interrupt. The third line generates a
single interrupt whenever any bit is set.    */
typedef union 
{
    struct
    {
        /*EDSn (n=0..31) 0 = Event not detected on GPIO pin n
        1 = Event detected on GPIO pin n
        R/W 0 */
        uint32_t EDS:32;
    }bit;
    uint32_t reg;
}GPIO_GPEDS0_Type;

/*GPIO Event Detect Status Registers (GPEDSn) 
SYNOPSIS The event detect status registers are used to record level and edge events on the
GPIO pins. The relevant bit in the event detect status registers is set whenever: 1)
an edge is detected that matches the type of edge programmed in the rising/falling
edge detect enable registers, or 2) a level is detected that matches the type of level
programmed in the high/low level detect enable registers. The bit is cleared by
writing a “1” to the relevant bit.
The interrupt controller can be programmed to interrupt the processor when any of
the status bits are set. The GPIO peripheral has three dedicated interrupt lines.
Each GPIO bank can generate an independent interrupt. The third line generates a
single interrupt whenever any bit is set.    */
typedef union 
{
    struct
    {
        /*EDSn (n=0..31) 0 = Event not detected on GPIO pin n
        1 = Event detected on GPIO pin n
        R/W 0 */
        uint32_t EDS:22;
        /*Reserved*/
        uint32_t Reserved1:10;
    }bit;
    uint32_t reg;
}GPIO_GPEDS1_Type;

/*GPIO Rising Edge Detect Enable Registers (GPRENn)  
SYNOPSIS The rising edge detect enable registers define the pins for which a rising edge
transition sets a bit in the event detect status registers (GPEDSn). When the
relevant bits are set in both the GPRENn and GPFENn registers, any transition (1
to 0 and 0 to 1) will set a bit in the GPEDSn registers. The GPRENn registers use
synchronous edge detection. This means the input signal is sampled using the
system clock and then it is looking for a “011” pattern on the sampled signal. This
has the effect of suppressing glitches.*/
typedef union 
{
    struct
    {
        /*EDSn (n=0..31) 0 = Event not detected on GPIO pin n
        1 = Event detected on GPIO pin n
        R/W 0 */
        uint32_t REN:32;
    }bit;
    uint32_t reg;
}GPIO_GPREN0_Type;

/*GPIO Rising Edge Detect Enable Registers (GPRENn)  
SYNOPSIS The rising edge detect enable registers define the pins for which a rising edge
transition sets a bit in the event detect status registers (GPEDSn). When the
relevant bits are set in both the GPRENn and GPFENn registers, any transition (1
to 0 and 0 to 1) will set a bit in the GPEDSn registers. The GPRENn registers use
synchronous edge detection. This means the input signal is sampled using the
system clock and then it is looking for a “011” pattern on the sampled signal. This
has the effect of suppressing glitches.*/
typedef union 
{
    struct
    {
        /*EDSn (n=0..31) 0 = Event not detected on GPIO pin n
        1 = Event detected on GPIO pin n
        R/W 0 */
        uint32_t REN:22;
        /*Reserved*/
        uint32_t Reserved1:10;
    }bit;
    uint32_t reg;
}GPIO_GPREN1_Type;

/*GPIO Falling Edge Detect Enable Registers (GPRENn) 
SYNOPSIS The falling edge detect enable registers define the pins for which a falling edge
transition sets a bit in the event detect status registers (GPEDSn). When the relevant
bits are set in both the GPRENn and GPFENn registers, any transition (1 to 0 and 0
to 1) will set a bit in the GPEDSn registers. The GPFENn registers use synchronous
edge detection. This means the input signal is sampled using the system clock and
then it is looking for a “100” pattern on the sampled signal. This has the effect of
suppressing glitches. */
typedef union 
{
    struct
    {
        /*FENn (n=0..31) 0 = Falling edge detect disabled on GPIO pin n.
        1 = Falling edge on GPIO pin n sets corresponding bit in EDSn.
        R/W 0 */
        uint32_t FEN:32;
    }bit;
    uint32_t reg;
}GPIO_GPFEN0_Type;

/*GPIO Falling Edge Detect Enable Registers (GPRENn) 
SYNOPSIS The falling edge detect enable registers define the pins for which a falling edge
transition sets a bit in the event detect status registers (GPEDSn). When the relevant
bits are set in both the GPRENn and GPFENn registers, any transition (1 to 0 and 0
to 1) will set a bit in the GPEDSn registers. The GPFENn registers use synchronous
edge detection. This means the input signal is sampled using the system clock and
then it is looking for a “100” pattern on the sampled signal. This has the effect of
suppressing glitches. */
typedef union 
{
    struct
    {
        /*FENn (n=0..31) 0 = Falling edge detect disabled on GPIO pin n.
        1 = Falling edge on GPIO pin n sets corresponding bit in EDSn. R/W 0 */
        uint32_t FEN:22;
        /*Reserved*/
        uint32_t Reserved1:10;
    }bit;
    uint32_t reg;
}GPIO_GPFEN1_Type;

/*GPIO High Detect Enable Registers (GPHENn) 
SYNOPSIS The high level detect enable registers define the pins for which a high level sets a bit in
the event detect status register (GPEDSn). If the pin is still high when an attempt is
made to clear the status bit in GPEDSn then the status bit will remain set.  */
typedef union 
{
    struct
    {
        /*HENn (n=0..31) 0 = High detect disabled on GPIO pin n
        1 = High on GPIO pin n sets corresponding bit in GPEDS
        R/W 0 */
        uint32_t HEN:32;
    }bit;
    uint32_t reg;
}GPIO_GPHEN0_Type;

/*GPIO High Detect Enable Registers (GPHENn) 
SYNOPSIS The high level detect enable registers define the pins for which a high level sets a bit in
the event detect status register (GPEDSn). If the pin is still high when an attempt is
made to clear the status bit in GPEDSn then the status bit will remain set.  */
typedef union 
{
    struct
    {
        /*HENn (n=0..31) 0 = High detect disabled on GPIO pin n
        1 = High on GPIO pin n sets corresponding bit in GPEDS
        R/W 0 */
        uint32_t HEN:22;
        /*Reserved*/
        uint32_t Reserved1:10;
    }bit;
    uint32_t reg;
}GPIO_GPHEN1_Type;

/*GPIO Low Detect Enable Registers (GPLENn) 
SYNOPSIS The low level detect enable registers define the pins for which a low level sets a bit in
the event detect status register (GPEDSn). If the pin is still low when an attempt is
made to clear the status bit in GPEDSn then the status bit will remain set.   */
typedef union 
{
    struct
    {
        /*LENn (n=0..31) 0 = Low detect disabled on GPIO pin n
        1 = Low on GPIO pin n sets corresponding bit in GPEDS
        R/W 0 */
        uint32_t LEN:32;
    }bit;
    uint32_t reg;
}GPIO_GPLEN0_Type;

/*GPIO Low Detect Enable Registers (GPLENn) 
SYNOPSIS The low level detect enable registers define the pins for which a low level sets a bit in
the event detect status register (GPEDSn). If the pin is still low when an attempt is
made to clear the status bit in GPEDSn then the status bit will remain set.   */
typedef union 
{
    struct
    {
         /*LENn (n=0..31) 0 = Low detect disabled on GPIO pin n
        1 = Low on GPIO pin n sets corresponding bit in GPEDS
        R/W 0 */
        uint32_t LEN:22;
        /*Reserved*/
        uint32_t Reserved1:10;
    }bit;
    uint32_t reg;
}GPIO_GPLEN1_Type;


/*GPIO Asynchronous rising Edge Detect Enable Registers (GPARENn) 
SYNOPSIS The asynchronous rising edge detect enable registers define the pins for which a
asynchronous rising edge transition sets a bit in the event detect status registers
(GPEDSn).
Asynchronous means the incoming signal is not sampled by the system clock. As such
rising edges of very short duration can be detected. */
typedef union 
{
    struct
    {
        /*ARENn (n=0..31) 0 = Asynchronous rising edge detect disabled on GPIO pin n.
                        1 = Asynchronous rising edge on GPIO pin n sets corresponding bit in EDSn.
        R/W 0*/
        uint32_t LEN:32;
    }bit;
    uint32_t reg;
}GPIO_GPAREN0_Type;

/*GPIO Asynchronous rising Edge Detect Enable Registers (GPARENn) 
SYNOPSIS The asynchronous rising edge detect enable registers define the pins for which a
asynchronous rising edge transition sets a bit in the event detect status registers
(GPEDSn).
Asynchronous means the incoming signal is not sampled by the system clock. As such
rising edges of very short duration can be detected. */
typedef union 
{
    struct
    {
        /*ARENn (n=0..31) 0 = Asynchronous rising edge detect disabled on GPIO pin n.
                        1 = Asynchronous rising edge on GPIO pin n sets corresponding bit in EDSn.
        R/W 0*/
        uint32_t LEN:22;
        /*Reserved*/
        uint32_t Reserved1:10;
    }bit;
    uint32_t reg;
}GPIO_GPAREN1_Type;

/*GPIO Asynchronous Falling Edge Detect Enable Registers (GPAFENn) 
SYNOPSIS The asynchronous falling edge detect enable registers define the pins for which a
asynchronous falling edge transition sets a bit in the event detect status registers
(GPEDSn). Asynchronous means the incoming signal is not sampled by the system
clock. As such falling edges of very short duration can be detected.  */
typedef union 
{
    struct
    {
        /*AFENn (n=0..31)   0 = Asynchronous falling edge detect disabled on GPIO pin n.
                            1 = Asynchronous falling edge on GPIO pin n sets corresponding bit in EDSn.
        R/W 0 */
        uint32_t AFEN:32;
    }bit;
    uint32_t reg;
}GPIO_GPAFEN0_Type;

/*GPIO Asynchronous Falling Edge Detect Enable Registers (GPAFENn) 
SYNOPSIS The asynchronous falling edge detect enable registers define the pins for which a
asynchronous falling edge transition sets a bit in the event detect status registers
(GPEDSn). Asynchronous means the incoming signal is not sampled by the system
clock. As such falling edges of very short duration can be detected.  */
typedef union 
{
    struct
    {
        /*AFENn (n=0..31)   0 = Asynchronous falling edge detect disabled on GPIO pin n.
                            1 = Asynchronous falling edge on GPIO pin n sets corresponding bit in EDSn.
        R/W 0 */
        uint32_t AFEN:22;
        /*Reserved*/
        uint32_t Reserved1:10;
    }bit;
    uint32_t reg;
}GPIO_GPAFEN1_Type;

/*GPIO Pull-up/down Register (GPPUD) 
SYNOPSIS The GPIO Pull-up/down Register controls the actuation of the internal pull-up/down
control line to ALL the GPIO pins. This register must be used in conjunction with the 2
GPPUDCLKn registers.
Note that it is not possible to read back the current Pull-up/down settings and so it is the
users’ responsibility to ‘remember’ which pull-up/downs are active. The reason for this is
that GPIO pull-ups are maintained even in power-down mode when the core is off, when
all register contents is lost.
The Alternate function table also has the pull state which is applied after a power down.   */
typedef union 
{
    struct
    {
        /*PUD PUD - GPIO Pin Pull-up/down
        00 = Off – disable pull-up/down
        01 = Enable Pull Down control
        10 = Enable Pull Up control
        11 = Reserved
        *Use in conjunction with GPPUDCLK0/1/2
        R/W 0 */
        uint32_t PUD:2;
        /*Reserved*/
        uint32_t Reserved1:30;
    }bit;
    uint32_t reg;
}GPIO_GPPUD_Type;

/*GPIO Pull-up/down Clock Registers (GPPUDCLKn) 
SYNOPSIS The GPIO Pull-up/down Clock Registers control the actuation of internal pull-downs on
the respective GPIO pins. These registers must be used in conjunction with the GPPUD
register to effect GPIO Pull-up/down changes. The following sequence of events is
required:
1. Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither
to remove the current Pull-up/down)
2. Wait 150 cycles – this provides the required set-up time for the control signal
3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
modify – NOTE only the pads which receive a clock will be modified, all others will
retain their previous state.
4. Wait 150 cycles – this provides the required hold time for the control signal
5. Write to GPPUD to remove the control signal
6. Write to GPPUDCLK0/1 to remove the clock    */
typedef union 
{
    struct
    {
        /*PUDCLKn (n=32..53) 0 = No Effect
        1 = Assert Clock on line (n)
        *Must be used in conjunction with GPPUD
        R/W 0 */
        uint32_t PUDCLK:32;
    }bit;
    uint32_t reg;
}GPIO_GPPUDCLK0_Type;

/*GPIO Pull-up/down Clock Registers (GPPUDCLKn) 
SYNOPSIS The GPIO Pull-up/down Clock Registers control the actuation of internal pull-downs on
the respective GPIO pins. These registers must be used in conjunction with the GPPUD
register to effect GPIO Pull-up/down changes. The following sequence of events is
required:
1. Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither
to remove the current Pull-up/down)
2. Wait 150 cycles – this provides the required set-up time for the control signal
3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
modify – NOTE only the pads which receive a clock will be modified, all others will
retain their previous state.
4. Wait 150 cycles – this provides the required hold time for the control signal
5. Write to GPPUD to remove the control signal
6. Write to GPPUDCLK0/1 to remove the clock    */
typedef union 
{
    struct
    {
        /*PUDCLKn (n=32..53) 0 = No Effect
        1 = Assert Clock on line (n)
        *Must be used in conjunction with GPPUD
        R/W 0 */
        uint32_t PUDCLK:22;
        /*Reserved*/
        uint32_t Reserved1:10;
    }bit;
    uint32_t reg;
}GPIO_GPPUDCLK1_Type;


/*Register View
The GPIO has 41 registers. All accesses are assumed to be 32-bit. */

typedef struct
{
    GPIO_GPFSEL0_Type GPIO_GPFSEL0;
    GPIO_GPFSEL1_Type GPIO_GPFSEL1;
    GPIO_GPFSEL2_Type GPIO_GPFSEL2;
    GPIO_GPFSEL3_Type GPIO_GPFSEL3;
    GPIO_GPFSEL4_Type GPIO_GPFSEL4;
    GPIO_GPFSEL5_Type GPIO_GPFSEL5;
    uint32_t Reserved1;
    GPIO_GPSET0_Type GPIO_GPSET0;
    GPIO_GPSET1_Type GPIO_GPSET1;
    uint32_t Reserved2;
    GPIO_GPCLR0_Type GPIO_GPCLR0;
    GPIO_GPCLR1_Type GPIO_GPCLR1;
    uint32_t Reserved3;
    GPIO_GPCLR0_Type GPIO_GPLEV0;
    GPIO_GPCLR1_Type GPIO_GPLEV1;
    uint32_t Reserved4;
    GPIO_GPCLR0_Type GPIO_GPEDS0;
    GPIO_GPCLR1_Type GPIO_GPEDS1;
    uint32_t Reserved5;
    GPIO_GPCLR0_Type GPIO_GPREN0;
    GPIO_GPCLR1_Type GPIO_GPREN1;
    uint32_t Reserved6;
    GPIO_GPCLR0_Type GPIO_GPFEN0;
    GPIO_GPCLR1_Type GPIO_GPFEN1;
    uint32_t Reserved7;
    GPIO_GPCLR0_Type GPIO_GPHEN0;
    GPIO_GPCLR1_Type GPIO_GPHEN1;
    uint32_t Reserved8;
    GPIO_GPCLR0_Type GPIO_GPLEN0;
    GPIO_GPCLR1_Type GPIO_GPLEN1;
    uint32_t Reserved9;
    GPIO_GPCLR0_Type GPIO_GPAREN0;
    GPIO_GPCLR1_Type GPIO_GPAREN1;
    uint32_t Reserved10;
    GPIO_GPCLR0_Type GPIO_GPAFEN0;
    GPIO_GPCLR1_Type GPIO_GPAFEN1;
    uint32_t Reserved11;
    GPIO_GPPUD_Type GPIO_GPPUD;
    GPIO_GPPUDCLK0_Type GPIO_GPPUDCLK0;
    GPIO_GPPUDCLK1_Type GPIO_GPPUDCLK1;
}aGPIO;



/* Alternative Function Assignments
Every GPIO pin can carry an alternate function. Up to 6 alternate function are available but
not every pin has that many alternate functions. The table below gives a quick over view. 

Pull ALT0 ALT1 ALT2 ALT3 ALT4 ALT5
GPIO0 High SDA0 SA5 <reserved>
GPIO1 High SCL0 SA4 <reserved>
GPIO2 High SDA1 SA3 <reserved>
GPIO3 High SCL1 SA2 <reserved>
GPIO4 High GPCLK0 SA1 <reserved> ARM_TDI
GPIO5 High GPCLK1 SA0 <reserved> ARM_TDO
GPIO6 High GPCLK2 SOE_N / SE <reserved> ARM_RTCK
GPIO7 High SPI0_CE1_N SWE_N / SRW_N <reserved>
GPIO8 High SPI0_CE0_N SD0 <reserved>
GPIO9 Low SPI0_MISO SD1 <reserved>
GPIO10 Low SPI0_MOSI SD2 <reserved>
GPIO11 Low SPI0_SCLK SD3 <reserved>
GPIO12 Low PWM0 SD4 <reserved> ARM_TMS
GPIO13 Low PWM1 SD5 <reserved> ARM_TCK
GPIO14 Low TXD0 SD6 <reserved> TXD1
GPIO15 Low RXD0 SD7 <reserved> RXD1
GPIO16 Low <reserved> SD8 <reserved> CTS0 SPI1_CE2_N CTS1
GPIO17 Low <reserved> SD9 <reserved> RTS0 SPI1_CE1_N RTS1
GPIO18 Low PCM_CLK SD10 <reserved> BSCSL SDA / MOSI SPI1_CE0_N PWM0
GPIO19 Low PCM_FS SD11 <reserved> BSCSL SCL / SCLK SPI1_MISO PWM1
GPIO20 Low PCM_DIN SD12 <reserved> BSCSL / MISO SPI1_MOSI GPCLK0
GPIO21 Low PCM_DOUT SD13 <reserved> BSCSL / CE_N SPI1_SCLK GPCLK1
GPIO22 Low <reserved> SD14 <reserved> SD1_CLK ARM_TRST
GPIO23 Low <reserved> SD15 <reserved> SD1_CMD ARM_RTCK
GPIO24 Low <reserved> SD16 <reserved> SD1_DAT0 ARM_TDO
GPIO25 Low <reserved> SD17 <reserved> SD1_DAT1 ARM_TCK
GPIO26 Low <reserved> <reserved> <reserved> SD1_DAT2 ARM_TDI
GPIO27 Low <reserved> <reserved> <reserved> SD1_DAT3 ARM_TMS
GPIO28 - SDA0 SA5 PCM_CLK <reserved>
GPIO29 - SCL0 SA4 PCM_FS <reserved>
GPIO30 Low <reserved> SA3 PCM_DIN CTS0 CTS1
GPIO31 Low <reserved> SA2 PCM_DOUT RTS0 RTS1
GPIO32 Low GPCLK0 SA1 <reserved> TXD0 TXD1
GPIO33 Low <reserved> SA0 <reserved> RXD0 RXD1
GPIO34 High GPCLK0 SOE_N / SE <reserved> <reserved>
GPIO35 High SPI0_CE1_N SWE_N / SRW_N <reserved>
GPIO36 High SPI0_CE0_N SD0 TXD0 <reserved>
GPIO37 Low SPI0_MISO SD1 RXD0 <reserved>
GPIO38 Low SPI0_MOSI SD2 RTS0 <reserved>
GPIO39 Low SPI0_SCLK SD3 CTS0 <reserved>
GPIO40 Low PWM0 SD4 <reserved> SPI2_MISO TXD1 
GPIO41 Low PWM1 SD5 <reserved> <reserved> SPI2_MOSI RXD1
GPIO42 Low GPCLK1 SD6 <reserved> <reserved> SPI2_SCLK RTS1
GPIO43 Low GPCLK2 SD7 <reserved> <reserved> SPI2_CE0_N CTS1
GPIO44 - GPCLK1 SDA0 SDA1 <reserved> SPI2_CE1_N
GPIO45 - PWM1 SCL0 SCL1 <reserved> SPI2_CE2_N
GPIO46 High <Internal>
GPIO47 High <Internal>
GPIO48 High <Internal>
GPIO49 High <Internal>
GPIO50 High <Internal>
GPIO51 High <Internal>
GPIO52 High <Internal>
GPIO53 High <Internal>

Special function legend:
Name Function See section
SDA0 BSC6
 master 0 data line BSC
SCL0 BSC master 0 clock line BSC
SDA1 BSC master 1 data line BSC
SCL1 BSC master 1 clock line BSC
GPCLK0 General purpose Clock 0 <TBD>
GPCLK1 General purpose Clock 1 <TBD>
GPCLK2 General purpose Clock 2 <TBD>
SPI0_CE1_N SPI0 Chip select 1 SPI
SPI0_CE0_N SPI0 Chip select 0 SPI
SPI0_MISO SPI0 MISO SPI
SPI0_MOSI SPI0 MOSI SPI
SPI0_SCLK SPI0 Serial clock SPI
PWMx Pulse Width Modulator 0..1 Pulse Width Modulator
TXD0 UART 0 Transmit Data UART
RXD0 UART 0 Receive Data UART
CTS0 UART 0 Clear To Send UART
RTS0 UART 0 Request To Send UART
PCM_CLK PCM clock PCM Audio
PCM_FS PCM Frame Sync PCM Audio
PCM_DIN PCM Data in PCM Audio
PCM_DOUT PCM data out PCM Audio
SAx Secondary mem Address bus Secondary Memory Interface
SOE_N / SE Secondary mem. Controls Secondary Memory Interface
SWE_N / SRW_N Secondary mem. Controls Secondary Memory Interface
SDx Secondary mem. data bus Secondary Memory Interface
BSCSL SDA / MOSI BSC slave Data, SPI salve MOSI BSC ISP slave
BSCSL SCL / SCLK BSC slave Clock, SPI slave clock BSC ISP slave
BSCSL - / MISO BSC <not used>,SPI MISO BSC ISP slave
BSCSL - / CE_N BSC <not used>, SPI CSn BSC ISP slave 
SPI1_CEx_N SPI1 Chip select 0-2 Auxiliary I/O
SPI1_MISO SPI1 MISO Auxiliary I/O
SPI1_MOSI SPI1 MOSI Auxiliary I/O
SPI1_SCLK SPI1 Serial clock Auxiliary I/O
TXD0 UART 1 Transmit Data Auxiliary I/O
RXD0 UART 1 Receive Data Auxiliary I/O
CTS0 UART 1 Clear To Send Auxiliary I/O
RTS0 UART 1 Request To Send Auxiliary I/O
SPI2_CEx_N SPI2 Chip select 0-2 Auxiliary I/O
SPI2_MISO SPI2 MISO Auxiliary I/O
SPI2_MOSI SPI2 MOSI Auxiliary I/O
SPI2_SCLK SPI2 Serial clock Auxiliary I/O
ARM_TRST ARM JTAG reset <TBD>
ARM_RTCK ARM JTAG return clock <TBD>
ARM_TDO ARM JTAG Data out <TBD>
ARM_TCK ARM JTAG Clock <TBD>
ARM_TDI ARM JTAG Data in <TBD>
ARM_TMS ARM JTAG Mode select <TBD> 
*/

#define CM_GPCTL_SRC         0
#define CM_GPCTL_ENAB        4
#define CM_GPCTL_KILL        5
#define CM_GPCTL_BUSY        7
#define CM_GPCTL_FLIP        8
#define CM_GPCTL_MASH        9
#define CM_GPCTL_PASSWD      24

typedef union 
{
    struct
    {
        /*Clock source
        0 = GND
        1 = oscillator
        2 = testdebug0
        3 = testdebug1
        4 = PLLA per
        5 = PLLC per
        6 = PLLD per
        7 = HDMI auxiliary
        8-15 = GND
        To avoid lock-ups and glitches do not change this
        control while BUSY=1 and do not change this control
        at the same time as asserting ENAB.
        R/W 0 */
        uint32_t SRC:4;
        /*Enable the clock generator
        This requests the clock to start or stop without
        glitches. The output clock will not stop immediately
        because the cycle must be allowed to complete to
        avoid glitches. The BUSY flag will go low when the
        final cycle is completed.
        R/W 0 */
        uint32_t ENAB:1;
        /*Kill the clock generator
        0 = no action
        1 = stop and reset the clock generator
        This is intended for test/debug only. Using this control
        may cause a glitch on the clock generator output.
        R/W 0 */
        uint32_t KILL:1;
        /*Unused*/
        uint32_t Reserved1:1;
        /*BUSY Clock generator is running
        Indicates the clock generator is running. To avoid
        glitches and lock-ups, clock sources and setups must
        not be changed while this flag is set.
        R 0 */
        uint32_t BUSY:1;
        /*Invert the clock generator output
        This is intended for use in test/debug only. Switching
        this control will generate an edge on the clock
        generator output. To avoid output glitches do not
        switch this control while BUSY=1.
        R/W 0*/
        uint32_t FLIP:1;
        /*MASH control
        0 = integer division
        1 = 1-stage MASH (equivalent to non-MASH dividers)
        2 = 2-stage MASH
        3 = 3-stage MASH
        To avoid lock-ups and glitches do not change this
        control while BUSY=1 and do not change this control
        at the same time as asserting ENAB.
        R/W 0*/
        uint32_t MASH:2;
        /*Unused*/
        uint32_t Reserved2:13;
        /*Clock Manager password “5a” */
        uint32_t PASSWD:8;
    }bit;
    uint32_t reg;
}CM_GPCTL_Type;

typedef union 
{
    struct
    {
        /*Fractional part of divisor
        To avoid lock-ups and glitches do not change this
        control while BUSY=1.
        R/W 0 */
        uint32_t DIVF:12;
        /*Integer part of divisor
        This value has a minimum limit determined by the
        MASH setting. See text for details. To avoid lock-ups
        and glitches do not change this control while BUSY=1.
        R/W 0*/
        uint32_t DIVI:12;
        /*Clock Manager password “5a”*/
        uint32_t PASSWD:8;
    }bit;
    uint32_t reg;
}CM_GPDIV_Type;



/*Clock Manager General Purpose Clocks Control (CM_GP0CTL, GP1CTL &
GP2CTL) 
Address 
0x 7e10 1070 CM_GP0CTL
0x 7e10 1078 CM_GP1CTL
0x 7e10 1080 CM_GP2CTL 
Clock Manager General Purpose Clock Divisors (CM_GP0DIV, CM_GP1DIV &
CM_GP2DIV) 
Address 
0x 7e10 1074 CM_GP0DIV
0x 7e10 107c CM_GP1DIV
0x 7e10 1084 CM_GP2DIV */
typedef struct
{
    CM_GPCTL_Type  CM_GP0CTL;
    CM_GPDIV_Type  CM_GP0DIV;
    CM_GPCTL_Type  CM_GP1CTL;
    CM_GPDIV_Type  CM_GP1DIV;
    CM_GPCTL_Type  CM_GP2CTL;
    CM_GPDIV_Type  CM_GP2DIV;
}aCM_Type;



