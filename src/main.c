#include "driver/rmt.h"
#include "irsend.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



void app_main () {

txsetup();
    printf ("txsetup\n");

for (int i = 0; i < 2; i++)
    {


setSwing_off();
setSwingLR_off();

//   0 FAN 1 COOL 2 DRY 3 HEAT
setMode( 1 );
// 0~4 speed,5 auto,6 moon
setFan( 5 );
// temparature
setTemp( 24 );
on();

    sendframe();
    printf ("irsend finished:%d\n", i);

vTaskDelay(10000 / portTICK_PERIOD_MS);

    }

}