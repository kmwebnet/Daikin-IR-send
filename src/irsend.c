#include "irsend.h"
#include "daikindef.h"
#include "driver/rmt.h"
#include "stdio.h"

esp_err_t ret;
rmt_item32_t *rmtdata;

void txsetup (void)
{
  printf("txsetup started.\n");

  rmt_config_t rmtConfig;
 
  rmtConfig.rmt_mode = RMT_MODE_TX;  // transmit mode
  rmtConfig.channel = channel;  // channel to use 0 - 7
  rmtConfig.clk_div = 80;  // clock divider 1 - 255. source clock is 80MHz -> 80MHz/80 = 1MHz -> 1 tick = 1 us
  rmtConfig.gpio_num = irPin; // pin to use
  rmtConfig.mem_block_num = 7; // memory block size
   
  rmtConfig.tx_config.loop_en = 0; // no loop
  rmtConfig.tx_config.carrier_freq_hz = CARRIER_FREQ;  // IR remote controller uses 38kHz carrier frequency
  rmtConfig.tx_config.carrier_duty_percent = DUTY_PERCENT; // duty 
  rmtConfig.tx_config.carrier_level =  RMT_CARRIER_LEVEL_HIGH; // carrier level
  rmtConfig.tx_config.carrier_en = 1;  // carrier enable
  rmtConfig.tx_config.idle_level =  RMT_IDLE_LEVEL_LOW ; // signal level at idle
  rmtConfig.tx_config.idle_output_en = 1;  // output if idle
 
  ret = rmt_set_source_clk(channel, RMT_BASECLK_APB);
  if (ret != ESP_OK )
  {
      printf ("rmt_set_source_clk failed%d\n", ret);
      return;
  }
  ret = rmt_config(&rmtConfig);
  if (ret != ESP_OK )
  {
      printf ("rmt_config failed%d\n", ret);
      return;
  }

  ret = rmt_driver_install(rmtConfig.channel, 0, 0);
  if (ret != ESP_OK )
  {
      printf ("rmt_driver_install failed%d\n", ret);
      return;
  }


printf ("txsetup finish\n");

return; 
}

void on()
{
	daikin[25] |= 0x01;
    switch (daikin[25] >> 4)
    {
        case 2: //dry
            daikin[9] = vprocessstatus[7];
        break;
        case 3: //cool
            daikin[9] = vprocessstatus[6];
        break;
        case 4: //warm
            daikin[9] = vprocessstatus[8];
        break;
        case 6: //fan
            daikin[9] = vprocessstatus[11];
        break;
    }
    daikin[11] =  0x00;
	checksum();
}

void off()
{
	daikin[25] &= 0xFE;
    daikin[9] = vprocessstatus[0];
    daikin[11] =  0x80;
	checksum();
}

void setPower(uint8_t state)
{
	if (state == 0) {
		off();
	}else {
		on();
	}
}

void setSwing_on()
{
	daikin[28] |=0x0f;
	checksum();
}

void setSwing_off()
{
	daikin[28] &=0xf0;
	checksum();
}

void setSwing(uint8_t state)
{
	if (state == 0) {
		setSwing_off();
	}else {
		setSwing_on();
	}
}

void setSwingLR_on()
{
	daikin[29] = daikin[29] | 0x0F;
	checksum();
}

void setSwingLR_off()
{
	daikin[29] = daikin[29] & 0xF0;
	checksum();
}

void setSwingLR(uint8_t state)
{
    if (state == 0) {
        setSwingLR_off();
	}else {
        setSwingLR_on();
	}
}

uint8_t getPower()
{
	return (daikin[25] & 0x01);
}

void setMode(uint8_t mode)
{
	uint8_t trmode = vModeTable[mode];
	if (mode>=0 && mode <=4)
	{
		daikin[25] = ((daikin[25] & 0x0f) | trmode << 4) | getPower();
		checksum();
	}
}

// 0~4 speed,5 auto,6 moon
void setFan(uint8_t speed)
{
	uint8_t fan = vFanTable[speed];
	if (speed>=0 && speed <=6)
	{
		daikin[28] &= 0x0f;
		daikin[28] |= fan;
		checksum();
	}
}

void setTemp(uint8_t temp)
{
	if (temp >= 18 && temp<=32)
	{
		daikin[26] = (temp)*2;
		checksum();
	}
}

void checksum()
{
	uint8_t sum = 0;
	uint8_t i;


	for(i = 0; i <= 18; i++){
		sum += daikin[i];
	}

        daikin[19] = sum &0xFF;

        sum=0;
	for(i = 20; i <= 37; i++){
		sum += daikin[i];
        }

        daikin[38] = sum &0xFF;
}

void sendframe() {

printf("sendframe start\n");

// malloc necessary memory

  rmtdata = (rmt_item32_t *)malloc(
    (
    wakedatalength + 
    trailerlength  + 
    headerlength   + 
    frame1bytelength * 8 +
    trailerlength  + 
    headerlength   +
    frame2bytelength * 8 +        
    endframelength 
    ) * sizeof(rmt_item32_t));


//  set frame 1 checksum at the end of frame
    uint8_t sum = 0;
	for(uint8_t i = DAIKIN_FRAME1_OFFSET; i < DAIKIN_FRAME1_LENGTH - 1 ; i++){
		sum += daikin[i];
	}
        daikin[DAIKIN_FRAME1_LENGTH - 1] = sum &0xFF;


//  set frame 2 checksum at the end of frame
    sum = 0;
	for(uint8_t i = DAIKIN_FRAME2_OFFSET; i < DAIKIN_FRAME2_LENGTH - 1 ; i++){
		sum += daikin[i];
	}
        daikin[DAIKIN_FRAME2_LENGTH - 1] = sum &0xFF;


//  create frame structure

    uint16_t frameoffset = 0 ;

// create wake signal

    for (int i = 0 ; i < 5 ; i++ ){
        rmtdata[i].duration0 = DAIKIN_ZERO_MARK;
        rmtdata[i].level0 = 1;            
        rmtdata[i].duration1 = DAIKIN_ZERO_SPACE;
        rmtdata[i].level1 = 0;
        frameoffset ++ ;
    }

// create trailer signal

        rmtdata[frameoffset].duration0 = DAIKIN_ZERO_MARK;
        rmtdata[frameoffset].level0 = 1;            
        rmtdata[frameoffset].duration1 = DAIKIN_ZERO_TRAILER1;
        rmtdata[frameoffset].level1 = 0;
        frameoffset ++ ;

// create header signal

        rmtdata[frameoffset].duration0 = DAIKIN_HDR_MARK;
        rmtdata[frameoffset].level0 = 1;            
        rmtdata[frameoffset].duration1 = DAIKIN_HDR_SPACE;
        rmtdata[frameoffset].level1 = 0;
        frameoffset ++ ;

// create frame1 body

    int data2;
    for (int i = DAIKIN_FRAME1_OFFSET; i < DAIKIN_FRAME1_OFFSET + DAIKIN_FRAME1_LENGTH; i++) {
        data2=daikin[i];

        for (int j = 0; j < 8; j++) {
            rmtdata[frameoffset].duration0 = DAIKIN_ONE_MARK;
            rmtdata[frameoffset].level0 = 1;            

            if ((1 << j & data2)) {
            rmtdata[frameoffset].duration1 = DAIKIN_ONE_SPACE;
            }
            else 
            {
            rmtdata[frameoffset].duration1 = DAIKIN_ZERO_SPACE;
            }
            rmtdata[frameoffset].level1 = 0;
            frameoffset ++ ;
        }

    }

// create frame1 end

        rmtdata[frameoffset].duration0 = DAIKIN_ZERO_MARK;
        rmtdata[frameoffset].level0 = 1;            
        rmtdata[frameoffset].duration1 = DAIKIN_ZERO_TRAILER2;
        rmtdata[frameoffset].level1 = 0;
        frameoffset ++ ;

// frame 2

// create header signal

        rmtdata[frameoffset].duration0 = DAIKIN_HDR_MARK;
        rmtdata[frameoffset].level0 = 1;            
        rmtdata[frameoffset].duration1 = DAIKIN_HDR_SPACE;
        rmtdata[frameoffset].level1 = 0;
        frameoffset ++ ;

// create frame2 body

    for (int i = DAIKIN_FRAME2_OFFSET; i < DAIKIN_FRAME2_OFFSET + DAIKIN_FRAME2_LENGTH; i++) {
        data2=daikin[i];

        for (int j = 0; j < 8; j++) {
            rmtdata[frameoffset].duration0 = DAIKIN_ONE_MARK;
            rmtdata[frameoffset].level0 = 1;            

            if ((1 << j & data2)) {
            rmtdata[frameoffset].duration1 = DAIKIN_ONE_SPACE;
            }
            else 
            {
            rmtdata[frameoffset].duration1 = DAIKIN_ZERO_SPACE;
            }
            rmtdata[frameoffset].level1 = 0;
            frameoffset ++ ;
        }

    }

// create frame2 end

        rmtdata[frameoffset].duration0 = DAIKIN_ZERO_MARK;
        rmtdata[frameoffset].level0 = 1;            
        rmtdata[frameoffset].duration1 = DAIKIN_ZERO_SPACE;
        rmtdata[frameoffset].level1 = 0;
        frameoffset ++ ;

//send IR

  ret = rmt_write_items(channel, rmtdata, frameoffset, true);
  if (ret != ESP_OK )
  {
      printf ("rmt_write_items failed%d\n", ret);
      return;
  }

  rmt_wait_tx_done(channel, portMAX_DELAY);

printf("%d bits sent\n" , frameoffset);
free(rmtdata);

return;
}
