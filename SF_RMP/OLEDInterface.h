#include <Arduino.h>
#include "allocateMem.h"
#include "commandmessenger.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH   128  // OLED display width, in pixels
#define SCREEN_HEIGHT  32   // OLED display height, in pixels
#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c // address of the displays. All displays uses the same address

enum OLEDType {
    SSD1306,
};

class OLEDInterface
{
public:
    OLEDInterface(OLEDType type=SSD1306)
    {
        _type = SSD1306;
        if (_type == SSD1306) {
            if (!FitInMemory(sizeof(Adafruit_SSD1306))) {
                // Error Message to Connector
                cmdMessenger.sendCmd(kStatus, F("Custom Device does not fit in Memory"));
                return;
            }
            oled_1306 = new (allocateMemory(sizeof(Adafruit_SSD1306))) Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
        } 
    }
    void begin(uint8_t I2Caddress, bool status)
    {

            oled_1306->begin(SSD1306_SWITCHCAPVCC, I2Caddress, status);

    }
    void display()
    {

            oled_1306->display();

    }
    void clearDisplay()
    {
 
            oled_1306->clearDisplay();

    }
    void setTextColor(uint16_t color)
    {

            oled_1306->setTextColor(color);

    }
    void setFont(const GFXfont *f = NULL)
    {

            oled_1306->setFont(f);
;
    }
    void setCursor(int16_t x, int16_t y)
    {

            oled_1306->setCursor(x, y);

    }
    void setTextSize(uint8_t s)
    {

            oled_1306->setTextSize(s);

    }
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
    {

            oled_1306->fillCircle(x0, y0, r, color);

    }
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
    {

            oled_1306->drawFastVLine(x, y, h, color);

    }
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
    {

            oled_1306->drawFastHLine(x, y, w, color);

    }
    void println(const char *t)
    {

            oled_1306->println(t);

    }
    void println(String t)
    {

            oled_1306->println(t);

    }
    void print(const char *t)
    {

            oled_1306->print(t);

    }
    void print(String t)
    {

            oled_1306->print(t);

    }

private:
    OLEDType          _type;
    Adafruit_SSD1306 *oled_1306;

};