#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

#define bateriaW 14
#define bateriaH 6
#define ikona12 12



static const unsigned char bateria[] U8X8_PROGMEM = {
  0xFE, 0x3F, 0x03, 0x20, 0x03, 0x20, 0x03, 0x20, 0x03, 0x20, 0xFE, 0x3F, 
  };

static const unsigned char mieso[] U8X8_PROGMEM = {
  0x00, 0x00, 0xE0, 0x03, 0x18, 0x04, 0x04, 0x04, 0x82, 0x08, 0xC1, 0x09, 
  0x81, 0x08, 0x01, 0x08, 0x01, 0x08, 0x79, 0x0C, 0xFF, 0x0F, 0xCE, 0x07, 
  };

static const unsigned char mucha[] U8X8_PROGMEM = {
  0x00, 0x02, 0x38, 0x03, 0xC6, 0x0F, 0x81, 0x07, 0x81, 0x03, 0x43, 0x02, 
  0x3C, 0x04, 0x3E, 0x04, 0x3F, 0x04, 0x3F, 0x02, 0x5E, 0x02, 0xCC, 0x01, 
  };


/* Constructor */
U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0);
/* u8x8.begin() is required and will sent the setup/init sequence to the display */

void Drawgauge(int v, int minVal, int maxVal) {

      u8g2.firstPage();
      do {
        float gs_rad=-1.572;
        float ge_rad=1.572;
        float i=((v-minVal)*(ge_rad-gs_rad)/(maxVal-minVal)+gs_rad);
        int xp = 63+(sin(i) * 15);
        int yp = 63-(cos(i) * 15);
        u8g2.setDrawColor(1);
        u8g2.drawDisc(63,63,23, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT );
        u8g2.setDrawColor(0);
        u8g2.drawDisc(63,63,21, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT );
        u8g2.setDrawColor(1);
        u8g2.drawLine(63,63,xp,yp);
        u8g2.drawButtonUTF8(64, 20, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "Stan świeżości:" );
      } while( u8g2.nextPage() );
}

void setup(void)
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_bitcasual_t_all);
}

void loop(void) {
    for (int c=0; c <= 100; c++) {  
        Drawgauge(c,0,100);
    }
}
