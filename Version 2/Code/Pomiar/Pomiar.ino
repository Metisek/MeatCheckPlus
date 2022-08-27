#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0);


float gs_rad; 
float ge_rad; 

byte cx=64;
byte cy=62; 
byte promien=18;
byte procent=80; 


#define bateriaW 14
#define bateriaH 6
#define ikonki12 12


static const unsigned char bateria[] U8X8_PROGMEM = {
  0xFE, 0x3F, 0x03, 0x20, 0x03, 0x20, 0x03, 0x20, 0x03, 0x20, 0xFE, 0x3F, 
  };


static const unsigned char mieso[] U8X8_PROGMEM{
  0x00, 0x00, 0xE0, 0x03, 0x18, 0x04, 0x04, 0x04, 0x82, 0x08, 0xC1, 0x09, 
  0x81, 0x08, 0x01, 0x08, 0x01, 0x08, 0x79, 0x0C, 0xFF, 0x0F, 0xCE, 0x07, 
  };


static const unsigned char mucha[] U8X8_PROGMEM  {
  0x00, 0x02, 0x38, 0x03, 0xC6, 0x0F, 0x81, 0x07, 0x81, 0x03, 0x43, 0x02, 
  0x3C, 0x04, 0x3E, 0x04, 0x3F, 0x04, 0x3F, 0x02, 0x5E, 0x02, 0xCC, 0x01, 
  };

String stan(float zmienna, float minV, float maxV){
  float srednia = maxV - minV;
  int wynik = round(zmienna / srednia * 2.0 -0.3);
  switch (wynik){
    case 0: {
      return ("Świeże");
    }
    break;
    case 1: {
      return ("Nieświeże");
    }
    case 2: {
      return ("Zepsute");
    }
    
  }
}

char swiezosc;

void NarysujChuja(byte r, float v, int minVal, int maxVal ) {
    if (v < minVal){
      v = minVal; 
    }
    else if(v > maxVal){
      v = maxVal;
    }

u8g2.firstPage();
      do { 
                
                float gs_rad=-1.572;
                float ge_rad=1.572;
                float i=((v-minVal)*(ge_rad-gs_rad)/(maxVal-minVal)+gs_rad);
                
                int xp = 64+(sin(i) * 15);
                int yp = 62-(cos(i) * 15);
                u8g2.setFont(u8g2_font_bitcasual_t_all);
                   u8g2.setDrawColor(1);
                   u8g2.drawXBMP(113, 1, bateriaW, bateriaH, bateria);
                   u8g2.drawLine(0,10,128,10);
                   u8g2.drawDisc(64,63,r+1, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT );
                   u8g2.setDrawColor(0);
                   u8g2.drawDisc(64,63,r-1, U8G2_DRAW_UPPER_LEFT|U8G2_DRAW_UPPER_RIGHT);
                   u8g2.setDrawColor(1);
                   u8g2.drawDisc(64,62,2, U8G2_DRAW_ALL);
                   u8g2.drawLine(64,62,xp,yp); 
                   u8g2.drawXBMP(29, 52, ikonki12, ikonki12, mieso);
                   u8g2.drawXBMP(87, 52, ikonki12, ikonki12, mucha);
                u8g2.drawButtonUTF8(64, 24, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "Stan świeżości" );
                String swiezosc = stan(v, minVal, maxVal);
                u8g2.drawButtonUTF8(64, 38, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  0,  0, swiezosc.c_str());
                
                   
  
          } while ( u8g2.nextPage() );
}

void setup() {
  u8g2.begin();
//  u8g2.enableUTF8Print();
}

void loop() {
  for (float pomiar=1.0; pomiar <= 10; pomiar = pomiar + 0.2) { 
     NarysujChuja(promien,pomiar,1, 10);
     
  }
}
