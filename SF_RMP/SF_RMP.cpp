#include "SF_RMP.h"
#include "allocateMem.h"
#include "commandmessenger.h"
#include "Fonts/DSEG7Classic_Regular12pt7b.h"
#include "Fonts/FreeSans8pt7b.h"
#include <SPI.h>

// RMP Active
String  RMPActiveValue = "122.800";

// RMP Standby
String  RMPStandbyValue = "122.800";

// Light Test
uint8_t lightTestOn = 0x00;

SF_RMP::SF_RMP()
{
    _initialised = false;
}

void SF_RMP::attach(uint8_t addrI2C)
{
    _addrI2C = addrI2C;
    Wire.begin();
    Wire.setClock(400000);
    if (!FitInMemory(sizeof(OLEDInterface))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("Custom Device does not fit in Memory"));
        return;
    }

        oled = new (allocateMemory(sizeof(OLEDInterface))) OLEDInterface(SSD1306);

    _initialised = true;
}

void SF_RMP::begin()
{
    if (!_initialised)
        return;

    //**************************
    // // RMP Active
    // //**************************
    setTCAChannel(TCA9548A_CHANNEL_RMP_ACTIVE);
    oled->begin(SCREEN_ADDRESS, true); // Address 0x3C default
    oled->display();
    updateDisplayRmpActive();

    //**************************
    // RMP Standby
    //**************************
    setTCAChannel(TCA9548A_CHANNEL_RMP_STANDBY);
    oled->begin(SCREEN_ADDRESS, true); // Address 0x3C default
    oled->display();
    updateDisplayRmpStandby();
}


void SF_RMP::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void SF_RMP::set(int16_t messageID, char *message)
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
    case 3:
        lightTestOn = atoi(message);
        updateDisplayRmpActive();
        updateDisplayRmpStandby();
        break;
    default:
        break;
    }
}

void SF_RMP::update()
{
    // Do something which is required regulary
    updateDisplayRmpActive();
    updateDisplayRmpStandby();
}

/*
  switch multiplexer channel
*/
void SF_RMP::setTCAChannel(byte i)
{
    Wire.beginTransmission(_addrI2C);
    Wire.write(1 << i);
    Wire.endTransmission();
    delay(5); // Pause
}

void SF_RMP::updateDisplayRmpActive(void)
{
    setTCAChannel(TCA9548A_CHANNEL_RMP_ACTIVE);
    // Clear the buffer
    oled->clearDisplay();
    oled->setTextColor(SSD1306_WHITE);
    if(lightTestOn == 1) {
        oled->setFont(&FreeSans8pt7b);
        oled->setTextSize(1);
        oled->setCursor(20,15);
        oled->println("TEST");
        oled->setFont(&DSEG7Classic_Regular12pt7b);
        oled->setCursor(0,30);
        oled->println("RADIO ACT");    
    } else{
        oled->setFont(&DSEG7Classic_Regular12pt7b);
        oled->setCursor(0, 30);
        oled->println(RMPActiveValue);        
    }
    oled->display();
}

void SF_RMP::updateDisplayRmpStandby(void)
{
    setTCAChannel(TCA9548A_CHANNEL_RMP_STANDBY);
    // Clear the buffer
    oled->clearDisplay();
    oled->setTextColor(SSD1306_WHITE);
    if(lightTestOn == 1) {
        oled->setFont(&FreeSans8pt7b);
        oled->setTextSize(1);
        oled->setCursor(20,15);
        oled->println("TEST");
        oled->setFont(&DSEG7Classic_Regular12pt7b);
        oled->setCursor(0,30);
        oled->println("RADIO STBY");
        oled->fillCircle(16,60,2, SSD1306_WHITE);
    } else{
        oled->setFont(&DSEG7Classic_Regular12pt7b);
        oled->setCursor(0, 30);
        oled->println(RMPStandbyValue);        
    }
    oled->display();
}
 
