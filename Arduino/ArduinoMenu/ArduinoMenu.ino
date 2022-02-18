#include <menu.h>
#include <menuIO/serialOut.h>
#include <menuIO/chainStream.h>
#include <menuIO/serialIn.h>
#include <menuIO/keyIn.h>
#include <menuIO/u8g2Out.h>
#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0);

using namespace Menu;

#define LEDPIN LED_BUILTIN
#define MAX_DEPTH 1

unsigned int timeOn=10;
unsigned int timeOff=90;

MENU(mainMenu, "Blink menu", Menu::doNothing, Menu::noEvent, Menu::wrapStyle
  ,FIELD(timeOn,"On","ms",0,1000,10,1, Menu::doNothing, Menu::noEvent, Menu::noStyle)
  ,FIELD(timeOff,"Off","ms",0,10000,10,1,Menu::doNothing, Menu::noEvent, Menu::noStyle)
  ,EXIT("<Back")
);

serialIn serial(Serial);
MENU_INPUTS(in,&serial);

MENU_OUTPUTS(out,MAX_DEPTH
  ,SERIAL_OUT(Serial)
  ,NONE//must have 2 items at least
);

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);

void setup() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_bitcasual_t_all);
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Menu 4.x");
  Serial.println("Use keys + - * /");
  Serial.println("to control the menu navigation");
}

bool blink(int timeOn,int timeOff) {return millis()%(unsigned long)(timeOn+timeOff)<(unsigned long)timeOn;}

void loop() {
  nav.poll();
  digitalWrite(LEDPIN, blink(timeOn,timeOff));
  u8g2.firstPage();
  do {
      u8g2.drawFrame(14,32,100,10);
      u8g2.drawButtonUTF8(63, 20, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "Trwa przygotowanie" );
      
  } while( u8g2.nextPage() );
}
