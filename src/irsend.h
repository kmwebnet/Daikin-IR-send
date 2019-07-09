
#include "driver/rmt.h"

void txsetup (void);
void on();
void off();
void setPower(uint8_t state);
void setSwing_on();
void setSwing_off();
void setSwing(uint8_t state);
void setSwingLR_on();
void setSwingLR_off();
void setSwingLR(uint8_t state);
uint8_t getPower();
void setMode(uint8_t mode);
void setFan(uint8_t speed);
void setTemp(uint8_t temp);
void checksum();
void sendframe();

