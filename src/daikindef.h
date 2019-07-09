#ifndef daikindef_h
#define daikindef_h

#include "driver/rmt.h"

const int wakedatalength = 5;
const int headerlength = 1;
const int trailerlength = 1;
const int frame1bytelength = 8;
const int frame2bytelength = 19;      // Daikin format data length 8byte + 19byte
const int endframelength = 1;


const rmt_channel_t channel =   RMT_CHANNEL_0;
const gpio_num_t irPin =        GPIO_NUM_26;

#define CARRIER_FREQ            38000
#define DUTY_PERCENT            33

#define DAIKIN_HDR_MARK	        3600
#define DAIKIN_HDR_SPACE        1880
#define DAIKIN_ONE_MARK	        348
#define DAIKIN_ONE_SPACE	1386
#define DAIKIN_ZERO_MARK	348
#define DAIKIN_ZERO_SPACE       490
#define DAIKIN_ZERO_TRAILER1    25000
#define DAIKIN_ZERO_TRAILER2    30000

unsigned char daikin[]     = {
        0x11, 0xDA, 0x27, 0x00, 0x02, 0x00, 0x00, 0x00,
//        0     1     2    3     4     5      6    7
        0x00, 0x0e, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
//        8     9    10    11    12     13    14    15 
        0x00, 0x00, 0x00, 0x00, 0x11, 0xDA, 0x27, 0x00, 
//        16    17    18    19    20    21    22    23 
        0x00, 0x39, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x06, 
//        24    25    26    27    28    29    30    31 
        0x60, 0x00, 0x00, 0xc3, 0x00, 0x00, 0x00
//        32    33    34    35    36    37    38
		};

#define DAIKIN_FRAME1_LENGTH 20 //8
#define DAIKIN_FRAME1_OFFSET 0
#define DAIKIN_FRAME2_LENGTH 19
#define DAIKIN_FRAME2_OFFSET DAIKIN_FRAME1_LENGTH

unsigned char vFanTable[7] = { 0x30,0x40,0x50,0x60,0x70,0xa0,0xb0};
//                        0 FAN 1 COOL 2 DRY 3 HEAT
unsigned char vModeTable[5] = { 0x6,0x3,0x2,0x4,0x00};

unsigned char vprocessstatus[18] = {
        0x02, //stop
        0x03, //change temp.
        0x06, //change wind direction
        0x07, //change wind power
        0x0c, //dismiss timer setting
        0x0d, //comfort auto
        0x0e, //cool
        0x0f, //dry
        0x10, //warm
        0x16, //change wind direction horizontally
        0x18, //clean inside the aircon
        0x1a, //fan
        0x1c, //laundry mode
        0x1e, //change the status for on timer 
        0x1f, //change the status for off timer
        0x2f, //set a sleep timer
        0x34, //set "kaze-nai-su" mode
        0x35  //set strema
};

#endif