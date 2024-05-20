#pragma once

#include <Arduino.h>
#include "SF_RMP.h"

// only one entry required if you have only one custom device
enum {
    SF_RMP_DEVICE = 1,
};
class MFCustomDevice
{
public:
    MFCustomDevice();
    void attach(uint16_t adrPin, uint16_t adrType, uint16_t adrConfig);
    void detach();
    void update();
    void set(int16_t messageID, char *setPoint);

private:
    bool           getStringFromEEPROM(uint16_t addreeprom, char *buffer);
    bool           _initialized = false;
    SF_RMP           *_my_SF_RMP;
    uint8_t        _customType = 0;
};
