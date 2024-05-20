#pragma once

#include "Arduino.h"
#include "OLEDInterface.h"

// address of the multiplexer to change the channels
#define TCA9548A_I2C_ADDRESS  0x70
#define TCA9548A_CHANNEL_RMP_ACTIVE  0
#define TCA9548A_CHANNEL_RMP_STANDBY 1

class SF_RMP
{
public:
    SF_RMP();
    void begin();
    void attach(uint8_t addrI2C);
    void detach();
    void set(int16_t messageID, char *message);
    void update();

private:
    bool    _initialised;
    uint8_t _addrI2C;
    OLEDInterface *oled;

    void setTCAChannel(byte i);
    void updateDisplayRmpActive(void);
    void updateDisplayRmpStandby(void);
};