#include "SfRmp.h"
#include "allocateMem.h"
#include "commandmessenger.h"
#include "Fonts/DSEG7Classic_Regular20pt7b.h"
#include <SPI.h>

// RMP Active
String  RMPActiveValue = "000.000";

// Efis right
String  RMPStandbyValue = "000.000";



SfRmp::SfRmp()
{
    _initialised = false;
}

void SfRmp::attach(uint8_t addrI2C)
{
    _addrI2C = addrI2C;
    Wire.begin();
    Wire.setClock(400000);
    if (!FitInMemory(sizeof(OLEDInterface))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("Custom Device does not fit in Memory"));
        return;
    }
    if (_addrI2C & 0x01) {
        oled = new (allocateMemory(sizeof(OLEDInterface))) OLEDInterface(SSD1306);
    } else {
        oled = new (allocateMemory(sizeof(OLEDInterface))) OLEDInterface(SH1106);
    }
    _initialised = true;
}

void SfRmp::begin()
{
    if (!_initialised)
        return;

    //**************************
    // // Efis left
    // //**************************
    setTCAChannel(TCA9548A_CHANNEL_RMP_ACTIVE);
    oled->begin(SCREEN_ADDRESS, true); // Address 0x3C default
    oled->display();
    updateDisplayRmpActive();

    //**************************
    // Efis right
    //**************************
    setTCAChannel(TCA9548A_CHANNEL_RMP_STANDBY);
    oled->begin(SCREEN_ADDRESS, true); // Address 0x3C default
    oled->display();
    oled->setTextColor(SSD1306_WHITE);
    updateDisplayRmpStandby();
}


void SfRmp::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void SfRmp::set(int16_t messageID, char *message)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do.
        Important Remark!
        MessageID == -1 will be send from the connector when Mobiflight is closed
        Put in your code to shut down your custom device (e.g. clear a display)
        MessageID == -2 will be send from the connector when PowerSavingMode is entered
        Put in your code to enter this mode (e.g. clear a display)

    ********************************************************************************** */

    // do something according your messageID
    switch (messageID) {
    case -1:
        // tbd., get's called when Mobiflight shuts down
    case -2:
        // tbd., get's called when PowerSavingMode is entered
    case 0:
        /* code */
        RMPActiveValue = message;
        updateDisplayRmpActive();
        break;
    case 1:
        /* code */
        RMPStandbyValue = message;
        updateDisplayRmpStandby();
        break;
    default:
        break;
    }
}

void SfRmp::update()
{
    // Do something which is required regulary
    updateDisplayRmpActive();
    updateDisplayRmpStandby();
}

/*
  switch multiplexer channel
*/
void SfRmp::setTCAChannel(byte i)
{
    Wire.beginTransmission(_addrI2C);
    Wire.write(1 << i);
    Wire.endTransmission();
    delay(5); // Pause
}

void SfRmp::updateDisplayRmpActive(void)
{
    setTCAChannel(TCA9548A_CHANNEL_RMP_ACTIVE);
    // Clear the buffer
    oled->clearDisplay();
    oled->setTextColor(SSD1306_WHITE);
    oled->setFont(&DSEG7Classic_Regular20pt7b);
    oled->setCursor(0, 60);
    oled->println(RMPActiveValue);
    oled->display();
}

void SfRmp::updateDisplayRmpStandby(void)
{
    setTCAChannel(TCA9548A_CHANNEL_RMP_STANDBY);
    // Clear the buffer
    oled->clearDisplay();
    oled->setTextColor(SSD1306_WHITE);
    oled->setFont(&DSEG7Classic_Regular20pt7b);
    oled->setCursor(0, 60);
    oled->println(RMPStandbyValue);
    oled->display();
} 
