//Biblioteki
#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <BMP280_DEV.h>                         
#include <Adafruit_SGP30.h>

// Piny do BSM baterii
const int analogInPin = A0;
#define BATTERY 2
#define BATTERY_CHARGE 16


// Piny przycisków
#define RIGHT_B 12  // przycisk do poruszania menu w prawo
#define LEFT_B 13     // przycisk do poruszania menu w lewo
#define OK_B 14       // przycisk OK do egzekucji danej pozycji menu
#define CANCEL_B 15   // przycisk powrotu do poprzedniej wartości menu lub wyjścia z dajego polecenia

// Kontruktor obiektów
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_L(U8G2_R2); // I2C 0x3D
BMP280_DEV bmp280; // I2C 0x77                                
Adafruit_SGP30 sgp; // I2C 0x??

// Zmienne pomiarowe
float temperature, pressure, altitude; // BSP280
int tvoc, eCO2; // SGP30
int battery_voltage; // Do BMS


int stanBaterii = 5;
byte bateriaDelay = 0;
int pomiarBaterii = 0;
int stanTemp = 0;
int stanCount = 0;

// Zmienne wyświetlacza
float minVal, maxVal, v;
String nazwaPomiaru;

// Rozmiary dla obrazków
#define meatCheckW 54
#define meatCheckH 44
#define logo60 60
#define ikona12 12
#define exW 70
#define exH 48
#define bateriaW 14
#define bateriaH 6
#define navIkonaW 9
#define navIkonaH 6
#define OPDProz 56

// Obrazki w formacie XBM
static const unsigned char  meatCheckLogo[] U8X8_PROGMEM =  {  
  0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0xFF, 
  0x07, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0xFF, 
  0x07, 0xC0, 0x3F, 0x00, 0x00, 0x80, 0x7F, 0x00, 0x00, 0x7F, 0x00, 0x00, 
  0xE0, 0x1F, 0x00, 0x00, 0xFC, 0x00, 0x00, 0xF0, 0x07, 0x00, 0x00, 0xF0, 
  0x01, 0x00, 0xF8, 0x01, 0x00, 0x00, 0xE0, 0x03, 0x00, 0xFC, 0x00, 0x00, 
  0x00, 0xC0, 0x07, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x80, 0x1F, 
  0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x1F, 
  0xE0, 0x07, 0x00, 0x00, 0x78, 0x00, 0x1E, 0xF0, 0x03, 0x00, 0x00, 0xFE, 
  0x01, 0x1C, 0xF8, 0x01, 0x00, 0x00, 0xFF, 0x03, 0x3C, 0xF8, 0x00, 0x00, 
  0x00, 0xFF, 0x07, 0x3C, 0x7C, 0x00, 0x00, 0x80, 0x8F, 0x07, 0x38, 0x3E, 
  0x00, 0x00, 0x80, 0x07, 0x07, 0x38, 0x1F, 0x00, 0x00, 0x80, 0x03, 0x07, 
  0x38, 0x0F, 0x00, 0x00, 0x80, 0x07, 0x07, 0x38, 0x0F, 0x00, 0x00, 0x80, 
  0x8F, 0x07, 0x38, 0x07, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x38, 0x07, 0x00, 
  0x00, 0x00, 0xFF, 0x03, 0x38, 0x07, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x38, 
  0x07, 0x00, 0x00, 0x00, 0x78, 0x00, 0x38, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x38, 0x07, 0xE0, 0x03, 0x00, 0x00, 0x00, 0x3C, 0x0F, 0xFF, 0x3F, 
  0x00, 0x00, 0x00, 0x3E, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x3F, 0xFF, 
  0xFF, 0xFF, 0x03, 0x00, 0x80, 0x3F, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0xC0, 
  0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0xF0, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 
  0x00, 0xFC, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xFF, 0x3F, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 
  0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xFC, 0x1F, 0xFE, 0xFF, 0xFF, 
  0xFF, 0x0F, 0xF8, 0x01, 0xE0, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x80, 
  0xFF, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 
  0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x3F, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x0F, 0x00,  };

static const unsigned char  NOTlogo[] U8X8_PROGMEM ={
  0x00, 0x00, 0x80, 0x01, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 
  0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x03, 0xFC, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
  0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 
  0xFF, 0x1F, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 
  0x00, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0x0C, 0x00, 0x80, 0xFF, 0xFF, 0x01, 
  0xF8, 0xFF, 0x1F, 0x00, 0xC0, 0xFF, 0x3F, 0x00, 0xC0, 0xFF, 0x3F, 0x00, 
  0xE0, 0xFF, 0x0F, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0xE0, 0xFF, 0x07, 0x00, 
  0x00, 0xFE, 0x7F, 0x00, 0xF0, 0xFF, 0x01, 0x00, 0x00, 0xF8, 0xFF, 0x00, 
  0xF8, 0xFF, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x01, 0xF8, 0x7F, 0x00, 0x00, 
  0x00, 0xE0, 0xFF, 0x01, 0xE0, 0x3F, 0x00, 0x00, 0x00, 0xC0, 0x7F, 0x00, 
  0xE0, 0xBF, 0xC3, 0xE1, 0xF1, 0x9F, 0x7F, 0x00, 0xE0, 0x9F, 0xC3, 0xF9, 
  0xF7, 0x9F, 0x7F, 0x00, 0xE0, 0x8F, 0xC3, 0xF9, 0xF7, 0x1F, 0x7F, 0x00, 
  0xF0, 0x8F, 0xC7, 0x39, 0x87, 0x03, 0xFF, 0x00, 0xF0, 0x87, 0xC7, 0x1D, 
  0x8E, 0x03, 0xFE, 0x00, 0xF8, 0x87, 0xC7, 0x1D, 0x8E, 0x03, 0xFE, 0x01, 
  0xF8, 0x87, 0xCF, 0x1D, 0x8E, 0x03, 0xFE, 0x01, 0xFF, 0x87, 0xCF, 0x1D, 
  0x8E, 0x03, 0xFC, 0x0F, 0xFF, 0x83, 0xCF, 0x1D, 0x8E, 0x03, 0xFC, 0x0F, 
  0xFF, 0x83, 0xDF, 0x1D, 0x8E, 0x03, 0xFC, 0x0F, 0xFF, 0x83, 0xDF, 0x1D, 
  0x8E, 0x03, 0xFC, 0x0F, 0xFF, 0x83, 0xDF, 0x1D, 0x8E, 0x03, 0xFC, 0x0F, 
  0xFF, 0x83, 0xDB, 0x1D, 0x8E, 0x03, 0xFC, 0x0F, 0xFF, 0x83, 0xFB, 0x1D, 
  0x8E, 0x03, 0xFC, 0x0F, 0xFF, 0x83, 0xFB, 0x1D, 0x8E, 0x03, 0xFC, 0x0F, 
  0xFF, 0x83, 0xF3, 0x1D, 0x8E, 0x03, 0xFC, 0x0F, 0xFF, 0x87, 0xF3, 0x1D, 
  0x8E, 0x03, 0xFC, 0x0F, 0xF8, 0x87, 0xF3, 0x1D, 0x8E, 0x03, 0xFE, 0x01, 
  0xF8, 0x87, 0xE3, 0x1D, 0x8E, 0x03, 0xFE, 0x01, 0xF0, 0x87, 0xE3, 0x1D, 
  0x8E, 0x03, 0xFE, 0x00, 0xF0, 0x8F, 0xE3, 0x39, 0x87, 0x03, 0xFF, 0x00, 
  0xE0, 0x8F, 0xC3, 0xF9, 0x87, 0x03, 0x7F, 0x00, 0xE0, 0x9F, 0xC3, 0xF9, 
  0x87, 0x83, 0x7F, 0x00, 0xE0, 0x3F, 0x00, 0xE0, 0x01, 0x80, 0x7F, 0x00, 
  0xE0, 0x3F, 0x00, 0x00, 0x00, 0xC0, 0x7F, 0x00, 0xF8, 0x7F, 0x00, 0x00, 
  0x00, 0xE0, 0xFF, 0x01, 0xF8, 0xFF, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x01, 
  0xF0, 0xFF, 0x01, 0x00, 0x00, 0xF8, 0xFF, 0x00, 0xE0, 0xFF, 0x07, 0x00, 
  0x00, 0xFE, 0x7F, 0x00, 0xE0, 0xFF, 0x0F, 0x00, 0x00, 0xFF, 0x7F, 0x00, 
  0xC0, 0xFF, 0x3F, 0x00, 0xC0, 0xFF, 0x3F, 0x00, 0x80, 0xFF, 0xFF, 0x01, 
  0xF8, 0xFF, 0x1F, 0x00, 0x00, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0x0C, 0x00, 
  0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x80, 0xFF, 0xFF, 
  0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
  0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0xFE, 0x03, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 
  0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x18, 0x00, 0x00, 0x00,  };

static const unsigned char  exLogo[] U8X8_PROGMEM = {
  0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x3C, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x1F, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 
  0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0xC0, 0x1F, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFC, 0x01, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x03, 
  0xF8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x07, 0xF8, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xF0, 0x07, 0xFC, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xF0, 0x0F, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x0F, 
  0xFC, 0x01, 0xC0, 0x3F, 0x00, 0xFF, 0x00, 0xE0, 0x0F, 0xFE, 0x01, 0xC0, 
  0x7F, 0x80, 0xFF, 0x00, 0xE0, 0x1F, 0xFE, 0x01, 0xC0, 0xFF, 0xC0, 0xFF, 
  0x00, 0xE0, 0x1F, 0xFE, 0x00, 0xC0, 0xFF, 0xE1, 0xFF, 0x00, 0xC0, 0x1F, 
  0xFE, 0x00, 0xC0, 0xFF, 0xF3, 0xFF, 0x00, 0xC0, 0x1F, 0xFE, 0x00, 0x00, 
  0xFC, 0xFF, 0x0F, 0x00, 0xC0, 0x1F, 0xFF, 0x00, 0x00, 0xF8, 0xFF, 0x07, 
  0x00, 0xC0, 0x3F, 0xFF, 0x00, 0x00, 0xF0, 0xFF, 0x03, 0x00, 0xC0, 0x3F, 
  0xFF, 0x00, 0x00, 0xE0, 0xFF, 0x01, 0x00, 0xC0, 0x3F, 0xFF, 0x00, 0x00, 
  0xC0, 0xFF, 0x00, 0x00, 0xC0, 0x3F, 0xFF, 0x00, 0x00, 0xC0, 0xFF, 0x00, 
  0x00, 0xC0, 0x3F, 0xFF, 0x00, 0x00, 0xC0, 0xFF, 0x00, 0x00, 0xC0, 0x3F, 
  0xFF, 0x00, 0x00, 0xC0, 0xFF, 0x00, 0x00, 0xC0, 0x3F, 0xFF, 0x00, 0x00, 
  0xC0, 0xFF, 0x00, 0x00, 0xC0, 0x3F, 0xFF, 0x00, 0x00, 0xC0, 0xFF, 0x00, 
  0x00, 0xC0, 0x3F, 0xFF, 0x00, 0x00, 0xE0, 0xFF, 0x01, 0x00, 0xC0, 0x3F, 
  0xFF, 0x00, 0x00, 0xF0, 0xFF, 0x03, 0x00, 0xC0, 0x3F, 0xFF, 0x00, 0x00, 
  0xF8, 0xFF, 0x07, 0x00, 0xC0, 0x3F, 0xFE, 0x00, 0x00, 0xFC, 0xFF, 0x0F, 
  0x00, 0xC0, 0x1F, 0xFE, 0x00, 0xC0, 0xFF, 0xF3, 0xFF, 0x00, 0xC0, 0x1F, 
  0xFE, 0x00, 0xC0, 0xFF, 0xE1, 0xFF, 0x00, 0xC0, 0x1F, 0xFE, 0x01, 0xC0, 
  0xFF, 0xC0, 0xFF, 0x00, 0xE0, 0x1F, 0xFE, 0x01, 0xC0, 0x7F, 0x80, 0xFF, 
  0x00, 0xE0, 0x1F, 0xFC, 0x01, 0xC0, 0x3F, 0x00, 0xFF, 0x00, 0xE0, 0x0F, 
  0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x0F, 0xFC, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xF0, 0x0F, 0xF8, 0x03, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xF0, 0x07, 0xF8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x07, 
  0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x03, 0xE0, 0x0F, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xFC, 0x01, 0xC0, 0x1F, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFE, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 
  0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3E, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x0F, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 
  };

static const unsigned char ElektronikLogo[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x80, 0xC7, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0xFE, 
  0x03, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x01, 0xC0, 0x1F, 0x00, 0x00, 0x00, 
  0x00, 0xF0, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 
  0xF0, 0x1F, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 
  0x00, 0x1E, 0x00, 0x00, 0x00, 0xE0, 0x1F, 0x00, 0x00, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0xFF, 0x01, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x07, 
  0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
  0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0x80, 0xF3, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0x80, 0xF3, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x03, 0x80, 0xF9, 0xFF, 0x1F, 
  0x00, 0x00, 0x00, 0x03, 0x80, 0x3D, 0xC0, 0xFF, 0x00, 0x00, 0x00, 0x03, 
  0x80, 0x1F, 0x00, 0xF8, 0x03, 0x00, 0x00, 0x03, 0xC0, 0x07, 0x00, 0x80, 
  0x1F, 0x00, 0x80, 0x03, 0xC0, 0x03, 0x00, 0x00, 0x3E, 0x00, 0x80, 0x01, 
  0xC0, 0x03, 0x00, 0x00, 0x70, 0x00, 0x80, 0x01, 0xC0, 0x01, 0x00, 0x00, 
  0xE0, 0x00, 0x80, 0x01, 0xC0, 0x01, 0xF8, 0x00, 0xC0, 0x01, 0x80, 0x01, 
  0xC0, 0x01, 0xFC, 0x1F, 0xC0, 0x01, 0x80, 0x01, 0xE0, 0x00, 0x1C, 0x7E, 
  0x80, 0x03, 0xC0, 0x01, 0xE0, 0x00, 0x3E, 0xF0, 0x80, 0x03, 0xC0, 0x00, 
  0xE0, 0x00, 0xFC, 0xF3, 0x00, 0x03, 0xC0, 0x00, 0xE0, 0x00, 0xC0, 0xFF, 
  0x00, 0x03, 0xC0, 0x00, 0xE0, 0x00, 0x00, 0xFC, 0x00, 0x03, 0xC0, 0x00, 
  0xE0, 0x00, 0x00, 0xE0, 0x00, 0x03, 0xC0, 0x00, 0x70, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0xE0, 0x00, 0x70, 0x00, 0x0F, 0x00, 0x80, 0x03, 0xE0, 0x00, 
  0x70, 0x80, 0x3F, 0x00, 0x80, 0x03, 0x60, 0x00, 0x70, 0x80, 0xF3, 0x03, 
  0x80, 0x01, 0x60, 0x00, 0x70, 0x80, 0xC7, 0x0F, 0x80, 0x01, 0x60, 0x00, 
  0x70, 0x00, 0x0F, 0xFC, 0x81, 0x00, 0x60, 0x00, 0x78, 0x00, 0x3F, 0xF0, 
  0xFF, 0x00, 0x70, 0x00, 0xF8, 0x00, 0xFC, 0x39, 0xFE, 0x00, 0x70, 0x00, 
  0xD8, 0x01, 0xE0, 0x3F, 0x70, 0x00, 0x30, 0x00, 0xD8, 0x03, 0x00, 0x0F, 
  0x60, 0x00, 0x30, 0x00, 0x98, 0x07, 0x00, 0x00, 0x60, 0x00, 0x30, 0x00, 
  0x18, 0x1F, 0x00, 0x00, 0x60, 0x00, 0x30, 0x00, 0x0C, 0x7E, 0x00, 0x00, 
  0x60, 0x00, 0x30, 0x00, 0x0C, 0xF8, 0x00, 0x00, 0x60, 0x00, 0x38, 0x00, 
  0x0C, 0xF0, 0x07, 0x00, 0x30, 0x00, 0x3C, 0x00, 0xFC, 0xC0, 0x3F, 0x00, 
  0x18, 0x00, 0x1E, 0x00, 0xF8, 0x07, 0xFF, 0x00, 0x1E, 0x00, 0x0F, 0x00, 
  0x80, 0x3F, 0xFC, 0xFF, 0x0F, 0x80, 0x07, 0x00, 0x00, 0xFC, 0xC3, 0xFF, 
  0x03, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x1F, 0x00, 0x00, 0xF0, 0x00, 0x00, 
  0x00, 0x00, 0xFC, 0x01, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x0F, 
  0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x0F, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xF8, 0x87, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 
  0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 
  };


static const unsigned char OPDP[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 
  0x00, 0x00, 0x00, 0xC0, 0x01, 0x7E, 0x80, 0x03, 0x00, 0x00, 0xF0, 0x03, 
  0x7E, 0xC0, 0x0F, 0x00, 0x00, 0xF8, 0x07, 0x7E, 0xE0, 0x1F, 0x00, 0x00, 
  0xFC, 0x07, 0xFF, 0xE0, 0x3F, 0x00, 0x00, 0xFC, 0xCF, 0xFF, 0xF3, 0x3F, 
  0x00, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 
  0xFF, 0x0F, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0xE0, 
  0xFF, 0xC1, 0xFF, 0x07, 0x00, 0x10, 0xE0, 0x7F, 0x00, 0xFE, 0x07, 0x08, 
  0x78, 0xF0, 0x0F, 0x00, 0xF0, 0x0F, 0x1E, 0xF8, 0xF9, 0x07, 0x00, 0xE0, 
  0x9F, 0x1F, 0xFC, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x3F, 0xFC, 0xFF, 0xE0, 
  0x00, 0x07, 0xFF, 0x3F, 0xFE, 0x3F, 0x30, 0x00, 0x0C, 0xFC, 0x7F, 0xFE, 
  0x3F, 0x10, 0x7E, 0x08, 0xFC, 0x7F, 0xFC, 0x1F, 0x18, 0xFF, 0x18, 0xF8, 
  0x3F, 0xF0, 0x1F, 0x08, 0xC3, 0x11, 0xF8, 0x0F, 0xC0, 0x0F, 0x0C, 0x80, 
  0x31, 0xF0, 0x03, 0x80, 0x0F, 0x04, 0x80, 0x21, 0xF0, 0x01, 0x80, 0x0F, 
  0x04, 0xC0, 0x21, 0xF0, 0x01, 0x80, 0x07, 0x04, 0xE0, 0x20, 0xE0, 0x01, 
  0xC0, 0x07, 0x04, 0x70, 0x20, 0xE0, 0x03, 0xFF, 0x03, 0x0C, 0x38, 0x30, 
  0xE0, 0xFF, 0xFF, 0x03, 0x08, 0x18, 0x10, 0xE0, 0xFF, 0xFF, 0x03, 0x08, 
  0x18, 0x10, 0xE0, 0xFF, 0xFF, 0x03, 0x18, 0x00, 0x18, 0xE0, 0xFF, 0xFF, 
  0x03, 0x30, 0x00, 0x0C, 0xE0, 0xFF, 0xFF, 0x07, 0x60, 0x18, 0x06, 0xE0, 
  0xFF, 0xC0, 0x07, 0x40, 0x18, 0x02, 0xE0, 0x03, 0x80, 0x07, 0xC0, 0x00, 
  0x03, 0xE0, 0x01, 0x80, 0x0F, 0x80, 0xFF, 0x01, 0xF0, 0x01, 0x80, 0x0F, 
  0x80, 0x00, 0x01, 0xF0, 0x01, 0xC0, 0x0F, 0x80, 0x00, 0x01, 0xF0, 0x03, 
  0xF0, 0x1F, 0x80, 0xFF, 0x01, 0xF8, 0x0F, 0xFC, 0x1F, 0x80, 0x00, 0x01, 
  0xF8, 0x3F, 0xFE, 0x3F, 0x80, 0xFF, 0x01, 0xFC, 0x7F, 0xFE, 0x3F, 0x80, 
  0x81, 0x01, 0xFC, 0x7F, 0xFC, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x3F, 0xFC, 
  0xFF, 0x01, 0x42, 0x80, 0xFF, 0x3F, 0xF8, 0xF9, 0x07, 0x66, 0xE0, 0x9F, 
  0x1F, 0x78, 0xF0, 0x0F, 0x3C, 0xF0, 0x0F, 0x1E, 0x10, 0xE0, 0x7F, 0x00, 
  0xFE, 0x07, 0x08, 0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 0x00, 0xF0, 
  0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 
  0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0xFC, 0xCF, 0xFF, 0xF3, 
  0x3F, 0x00, 0x00, 0xFC, 0x07, 0xFF, 0xE0, 0x3F, 0x00, 0x00, 0xF8, 0x07, 
  0x7E, 0xE0, 0x1F, 0x00, 0x00, 0xF0, 0x03, 0x7E, 0xC0, 0x0F, 0x00, 0x00, 
  0xC0, 0x01, 0x7E, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, };

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

static const unsigned char navOK[] U8X8_PROGMEM = {
  0x26, 0x01, 0xA9, 0x00, 0x69, 0x00, 0x69, 0x00, 0xA9, 0x00, 0x26, 0x01,
  };

static const unsigned char navPowrot[] U8X8_PROGMEM = {
  0x04, 0x00, 0x06, 0x00, 0xFF, 0x00, 0xFF, 0x01, 0x86, 0x01, 0xC4, 0x00, 
  };


// MENU
// Stałe w menu

const char menu0[] PROGMEM = "Menu główne";

const char menu2_1_1[] PROGMEM = "Szynka gotowana";
const char menu2_1_2[] PROGMEM = "Szynka konserwowa";
const char menu2_1_3[] PROGMEM = "Szynka wędzona";
const char menu2_1_4[] PROGMEM = "Mielonka tyrolska";

const char menu2_2_1[] PROGMEM = "Filet";
const char menu2_2_2[] PROGMEM = "Serca";
const char menu2_2_3[] PROGMEM = "Udka, podudzia itp.";

const char menu2_3_1[] PROGMEM = "Karkówka";
const char menu2_3_2[] PROGMEM = "Polędwica";
const char menu2_3_3[] PROGMEM = "Podroby wołowe";
const char menu2_3_4[] PROGMEM = "Inne wołowe";

const char menu2_4_1[] PROGMEM = "Schab";
const char menu2_4_2[] PROGMEM = "Boczek";
const char menu2_4_3[] PROGMEM = "Szynka";
const char menu2_4_4[] PROGMEM = "Łopatka";
const char menu2_4_5[] PROGMEM = "Inne wieprzowe";

const char menu2_5_1[] PROGMEM = "Ryba słodka";
const char menu2_5_2[] PROGMEM = "Ryba słona";
const char menu2_5_3[] PROGMEM = "Ryba marynowana";


const char menu2_1[] PROGMEM = "Wędlina";
const char menu2_2[] PROGMEM = "Kurczak";
const char menu2_3[] PROGMEM = "Wołowina";
const char menu2_4[] PROGMEM = "Wieprzowina";
const char menu2_5[] PROGMEM = "Ryba";

const char menu3_1[] PROGMEM = "Serwer WiFi";
const char menu3_2[] PROGMEM = "Debugowanie";

const char menu1[] PROGMEM = "Pomiar na żywo";
const char menu2[] PROGMEM = "Wybór mięs";
const char menu3[] PROGMEM = "Ustawienia";
const char menu4[] PROGMEM = "Wyłącz urządzenie";

// Lista pozycji menu ułożona zgodnie z indeksami

const char *const listaMenu[] PROGMEM = {
  menu1, menu2, menu3, menu4,
    menu2_1, menu2_2, menu2_3, menu2_4, menu2_5,
      menu2_1_1, menu2_1_2, menu2_1_3, menu2_1_4,
      menu2_2_1, menu2_2_2, menu2_2_3,
      menu2_3_1, menu2_3_2, menu2_3_3, menu2_3_4,
      menu2_4_1, menu2_4_2, menu2_4_3, menu2_4_4, menu2_4_5,
      menu2_5_1, menu2_5_2, menu2_5_3,
    menu3_1, menu3_2
};

// Lista każdego subemu drzewa

const char *const listaSubmenu[] PROGMEM = {
  menu0,
    menu2,
      menu2_1, menu2_2, menu2_3, menu2_4, menu2_5,
    menu3  
};


// Dla szybszego wykonywania programu i uniknięcia błędów stałe rozmiarów menu są zapisane w pamięci SRAM

const int indeksy[20] = {
  0, 4, 9, 13, 16, 20, 25, 28 
};

const int rozmiary[20] = {
  4, 5, 4, 3, 4, 5, 3, 2
};


// Zmienna uniemożliwiajaca wykonywanie inputu w pętli, nie można używać kilku przycisków naraz kosztem przejrzystosci i intuicyjnosci

byte inputDelay = 0;

// Zmienne pamiętające indeks danego submenu

int poziom1 = -1;
int poziom2 = -1;

// Zmienne indeksujące

int menuRozmiar;
int menuIndeks;
int menuPozycja;
int indeks;

// Zmienna wyłącznika urządzenia

bool wylacznik;


// FUNKCJE
// Zwracaniie pozycji w menu

int pozycja(int stanObecny, int indeks, int rozmiar){
  
  if (digitalRead(RIGHT_B) == LOW){
    inputDelay = RIGHT_B; 
    if (stanObecny >= indeks + rozmiar - 1){
      return indeks;
    }
    else{
      stanObecny++;
      return stanObecny;
    }
  }
  if (digitalRead(LEFT_B) == LOW){
    inputDelay = LEFT_B;
    if (stanObecny <= indeks){
       return indeks + rozmiar - 1;
    }
    else{
       stanObecny--;
       return stanObecny;
    }
  }
  return(stanObecny);
}



int menuNawigacja(){
  
  if (digitalRead(OK_B) == LOW){
      inputDelay = OK_B; 
      return 1;
  }
    
  if (digitalRead(CANCEL_B) == HIGH){
    inputDelay = CANCEL_B;
    if (poziom1 == -1){  
      return 0;
    }else if (poziom2 == -1){
      indeks = poziom1;
      poziom1 = -1;
      return 2;
    }
    else
    {
      indeks = poziom2;
      poziom2 = -1;
      return 2;
    }
  }
  return 0;
}

// Funkcja wykonywana dla komendy która ma być wywołaniem i aktualizacją submenu

void nawigacjaSubmenu(int temp){
   menuIndeks = indeksy[temp];
   menuRozmiar = rozmiary[temp];
   menuPozycja = menuIndeks;
   if (poziom1 == -1){
     poziom1 = indeks;  
   }else if (poziom2 == -1){
     poziom2 = indeks;
 }
    indeks = temp;
}

// Opcja printująca pozycje menu zapisane w pamięci PROGMEM

char buffer[22];

char* printStringow(byte id){
  strcpy_P(buffer, (char *)pgm_read_dword(&(listaMenu[id])));
  return buffer;
}

char* printNazwyMenu(byte id){
  strcpy_P(buffer, (char *)pgm_read_dword(&(listaSubmenu[id])));
  return buffer;
}



// Skrypt aktywujący dane polecenie zgodnie z listą

void aktywacjaSkryptu(int id){
  switch(id){
    case 0:
      minVal = 50;
      maxVal = 1050;
      nazwaPomiaru = "Na żywo";
      while (digitalRead(CANCEL_B) == LOW){
        rysunekPomiaru(0);
      }
      delay(50);
      break;        
    case 1:
      nawigacjaSubmenu(1);
      break;      
    case 2:
      nawigacjaSubmenu(7);
      break; 
    case 3:
      while (digitalRead(OK_B) == LOW){
        rysunekWylacznika();
      }
      wylacznik = false;
      while (digitalRead(CANCEL_B) == LOW){
        rysunekWylacznika();
        if (digitalRead(OK_B) == LOW){
          break;
        }
      }
      if (wylacznik == true and digitalRead(OK_B) == LOW){
        wylaczanie(0);
      }
      while (digitalRead(CANCEL_B) == HIGH or digitalRead(OK_B) == LOW){}
      break;  
    case 4:
      nawigacjaSubmenu(2);
      break;      
    case 5:
      nawigacjaSubmenu(3);
      break;      
    case 6:
      nawigacjaSubmenu(4);
      break;      
    case 7:
      nawigacjaSubmenu(5);
      break;
    case 8:
      nawigacjaSubmenu(6);
      break;
    case 9:
      pomiarInitVars(1);
      break;
    case 10:
      pomiarInitVars(2);
      break;  
    case 11:
      pomiarInitVars(3);
      break;      
    case 12:
      pomiarInitVars(4);
      break;      
    case 13:
      pomiarInitVars(5);
      break;      
    case 14:
      pomiarInitVars(6);
      break;      
    case 15:
      pomiarInitVars(7);
      break;      
    case 16:
      pomiarInitVars(8);
      break;
    case 17:
      pomiarInitVars(9);
      break;
    case 18:
      pomiarInitVars(10);
      break;    
    case 19:
      pomiarInitVars(11);
      break;
    case 20:
      pomiarInitVars(12);
      break;      
    case 21:
      pomiarInitVars(13);    
      break;
    case 22:
      pomiarInitVars(14);
      break;
    case 23:
      pomiarInitVars(15);
      break;
    case 24:
      pomiarInitVars(16);
      break;
    case 25:
      pomiarInitVars(17);
      break;
    case 26:
      pomiarInitVars(18);
      break;
    case 27:
      pomiarInitVars(19);
      break;
    case 28:
      break;  
    case 29:
      u8g2_L.clearBuffer();
      u8g2_L.sendBuffer();
      int temp = 0;
      while (digitalRead(CANCEL_B) == LOW){
        if (temp < 40){
          temp++;
          delay(1);
        }
        else{
          temperatura();
          u8g2_L.clearBuffer();
          u8g2_L.drawButtonUTF8(0, 12, U8G2_BTN_BW0, 0,  1,  1, "TVOC:");
          u8g2_L.drawButtonUTF8(85, 12, U8G2_BTN_BW0, 0,  1,  1, String(pomiar()).c_str());
          u8g2_L.drawButtonUTF8(0, 24, U8G2_BTN_BW0, 0,  1,  1, "Baterry:");
          u8g2_L.drawButtonUTF8(85, 24, U8G2_BTN_BW0, 0,  1,  1, String(pomiarNapieciaBaterii()).c_str());
          u8g2_L.drawButtonUTF8(0, 36, U8G2_BTN_BW0, 0,  1,  1, "Temp.:");
          u8g2_L.drawButtonUTF8(85, 36, U8G2_BTN_BW0, 0,  1,  1, String(temperature).c_str());
          u8g2_L.sendBuffer();
          temp = 0;
          
        }
      }
      break;
  }
}



// FUNKCJE POMIAROWE

// Funkcja pomiaru napięcia baterii dla pomiaru czujnika oraz wydajności samej baterii

int pomiarNapieciaBaterii(){
  return analogRead(analogInPin);
}

// Funkcja pomiaru temperatury
void temperatura(){
  bmp280.startForcedConversion(); 
  bmp280.getMeasurements(temperature, pressure, altitude);
  return;
}

int pomiar() {
  temperatura();
  sgp.IAQmeasure();
  tvoc =  sgp.TVOC;
  eCO2 = sgp.eCO2;
  return tvoc;
}

void pomiarB(){
  if (digitalRead(BATTERY_CHARGE) == HIGH){
    if (bateriaDelay == 5){
      bateriaDelay = 0;
      switch (stanBaterii){
        case 5:
          stanBaterii = 0;
          break;
        default:
          stanBaterii++;
          break;
      }
    } else{
      bateriaDelay++;
    }
  } else{
    stanTemp = 0;
    pomiarBaterii = (pomiarNapieciaBaterii() - 890) / 18;
    if (pomiarBaterii < stanBaterii){
      stanCount++;
      if (stanCount = 5){
        stanBaterii = pomiarBaterii;
        stanCount = 0;
      }
    } else{
      stanCount = 0;
    }
    if (stanBaterii < 0){
      wylaczanie(1);
    }
    if (stanBaterii > 5){
      stanBaterii = 5;
    }
  }
  return;
  
}

void pomiarInitVars(int pomID){
  switch pomID{
    case 1:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Szynka gotowana";
      break;
    case 2:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Szynka konserwowa";
      break;      
    case 3:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Szynka wędzona";
      break;      
    case 4:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Mielonka tyrolska";
      break;      
    case 5:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Filet";
      break;      
    case 6:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Serca";
      break;      
    case 7:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Udka, podudzia itp.";
      break;
    case 8:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Karkówka";
      break;
    case 9:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Polędwica";
      break;
    case 10:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Podroby wołowe";
      break;
    case 11:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Inne wołowe";
      break;
    case 12:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Schab";
      break;
    case 13:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Boczek";
      break;
    case 14:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Szynka";
      break;
    case 15:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Łopatka";
      break;
    case 16:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Inne wieprzowe";
      break;
    case 17:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Ryba słodka";
      break;
    case 18:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Ryba słona";
      break;
    case 19:
      minVal = 50;
      maxVal = 350;
      nazwaPomiaru = "Ryba marynowana";
      break;

  }
  
}

// RYSOWANIE

// Rysowanie wstepnych logotypow przy bootowaniu

void rysunekWstep(int wybor){
  u8g2_L.clearBuffer();
  switch (wybor){
      case 0:
        u8g2_L.drawXBMP(34, 2, logo60,  logo60, NOTlogo);
        break;

      case 1:
        u8g2_L.drawXBMP(29, 8, exW, exH, exLogo);
        break;

      case 2:
        u8g2_L.drawXBMP(34, 2, logo60, logo60, ElektronikLogo);
        break;

      case 3:
        u8g2_L.drawXBMP(36, 4, OPDProz, OPDProz, OPDP);
        break;
        
      case 4:
          u8g2_L.drawXBMP(36, 2, meatCheckW, meatCheckH, meatCheckLogo);
          u8g2_L.drawStr(30,62,"MeatChech+");
          u8g2_L.drawStr(110,10,"v2");
          break;

  }
  u8g2_L.sendBuffer();
}

// Rysowanie oczekiwania na rozgrzanie czujnika i przygotowanie do pomiarów

void przygotowanie(){
  int number = 0;
  while (number <= 100){
    number++;
    u8g2_L.clearBuffer();
    u8g2_L.drawFrame(14,32,100,10);
    u8g2_L.drawBox(14,32,number, 10);
    u8g2_L.drawButtonUTF8(63, 20, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "Trwa rozgrzewanie" );
    u8g2_L.sendBuffer();
    delay(14);
  }
  return;
}

// Rysunek elementów menu

void uiDraw(int menuID){
  u8g2_L.drawXBMP(113, 1, bateriaW, bateriaH, bateria);
  u8g2_L.drawLine(0, 10, 128, 10);
  for (int temp = 0; temp < stanBaterii; temp++){
      u8g2_L.drawLine((116 + temp*2), 3, (116 + temp*2), 4); 
  }
  switch(menuID){
    case 1:
      u8g2_L.drawXBMP(118, 57, navIkonaW, navIkonaH, navOK);
      break;
      
    case 2:
      u8g2_L.drawXBMP(118, 57, navIkonaW, navIkonaH, navOK);
      u8g2_L.drawXBMP(0, 57, navIkonaW, navIkonaH, navPowrot);
      break;

    case 3:
      u8g2_L.drawXBMP(0, 57, navIkonaW, navIkonaH, navPowrot);
      break;

    default:

      break;
  
  }
}

// Rysunek menu

void rysunekMenu() {
    u8g2_L.clearBuffer();
    if (indeks != 0){
      uiDraw(2);
    }
    else{
      uiDraw(1);
    }
    u8g2_L.drawButtonUTF8(63, 45, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, printStringow(menuPozycja));
    u8g2_L.drawButtonUTF8(63, 28, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, printNazwyMenu(indeks));
    int kropkiMenu = 63 - (menuRozmiar - 1) * 3;
    int pozycjaMenu = menuPozycja - menuIndeks;
    for (int temp = 0; temp < menuRozmiar; temp++){
      u8g2_L.drawDisc(kropkiMenu,60,1, U8G2_DRAW_ALL );
      if (pozycjaMenu == temp){
        u8g2_L.drawDisc(kropkiMenu,60,2, U8G2_DRAW_ALL );
       }
      kropkiMenu = kropkiMenu + 6;
    }
    u8g2_L.sendBuffer();
}


// Stringi do rysunku pomiaru

String tekstPomiaru(float dane, float minV, float maxV){
  float rozmiar = maxV - minV;
  int testPomiarowy = (dane - minV) / rozmiar * 2.65 - 0.35;
  switch (testPomiarowy){
    case 0:
      return "Produkt świeży";
    
    case 1:
      return "Produkt nieświeży";

    case 2:
      return "Produkt zepsuty";
  }
}

// Rysowanie miernika lub innych funkcji menu

void pomiarInit(){
  while (digitalRead(CANCEL_B) == LOW){
          
  }
  u8g2_L.clearBuffer();
  u8g2_L.drawButtonUTF8(64, 26, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, ");
  u8g2_L.drawFrame(14,42,102,9);
  int procent = map(v, minVal, maxVal, 0, 100);

  u8g2_L.drawBox(15,42,procent,9);

  u8g2_L.drawButtonUTF8(63, 64, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, String(String(procent) + "%").c_str() );
  u8g2_L.sendBuffer();  
  

}
  
void rysunekWylacznika(){
  if (digitalRead(RIGHT_B) == LOW or digitalRead(LEFT_B) == LOW){
      wylacznik = !wylacznik;
      while (digitalRead(RIGHT_B) == LOW or digitalRead(LEFT_B) == LOW){}
      delay(30);
  }
  u8g2_L.clearBuffer();
  
  u8g2_L.drawButtonUTF8(64, 16, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "Czy na pewno" );
  u8g2_L.drawButtonUTF8(64, 28, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "chcesz wyłączyć" );
  u8g2_L.drawButtonUTF8(64, 40, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "urządznie?" );
  u8g2_L.drawButtonUTF8(38, 61, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "Nie" );
  u8g2_L.drawButtonUTF8(88, 61, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "Tak" );
  u8g2_L.drawFrame(23 + wylacznik*49, 51, 29 + wylacznik*2, 13);
  u8g2_L.sendBuffer();
}

void rysunekPomiaru(int idPom) {
  pomiarB();
  if (idPom == 0){
    v = pomiar();
  }
  if (v < minVal){
     v = minVal;
  }   
  else if (v > maxVal){
     v = maxVal;
  }   
  u8g2_L.clearBuffer();
  u8g2_L.setDrawColor(1);
  u8g2_L.drawXBMP(0, 57, navIkonaW, navIkonaH, navPowrot);
  u8g2_L.drawButtonUTF8(64, 26, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, nazwaPomiaru.c_str() );
  u8g2_L.drawButtonUTF8(64, 37, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, tekstPomiaru(v, minVal, maxVal).c_str() );
  u8g2_L.drawFrame(14,42,102,9);
  int procent = map(v, minVal, maxVal, 0, 100);
  u8g2_L.drawBox(15,42,procent,9);
  u8g2_L.drawXBMP(18, 52, ikona12, ikona12, mieso);
  u8g2_L.drawXBMP(98, 52, ikona12, ikona12, mucha);
  u8g2_L.drawButtonUTF8(63, 64, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, String(String(procent) + "%").c_str() );
  u8g2_L.sendBuffer();  
  
}

// Funkcja wyłączająca urządzenie

void wylaczanie(int wyl_id){
  u8g2_L.clearBuffer();
  
  switch (wyl_id){
    case 0:
      u8g2_L.drawButtonUTF8(64, 32, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "Wyłączanie..." );
      break;
    case 1:
      u8g2_L.drawButtonUTF8(64, 26, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "Bateria" );
      u8g2_L.drawButtonUTF8(64, 38, U8G2_BTN_HCENTER|U8G2_BTN_BW0, 0,  1,  1, "rozładowana" );
      break;
    
  }
  
  u8g2_L.sendBuffer();
  delay(1500);
  u8g2_L.clearBuffer();
  u8g2_L.sendBuffer();
  digitalWrite(BATTERY, HIGH);
  while(true){
    delay(5);
  }  
}


void setup() {
  // Aktywacja BSM
  pomiarB();
  pinMode(BATTERY, OUTPUT);
  digitalWrite(BATTERY, LOW);
  if (stanBaterii < 0){
     wylaczanie(1);
  }

  pinMode(BATTERY_CHARGE, INPUT); 

  // Inicjacja urządzeń
  
  u8g2_L.setI2CAddress(0x7A);                       
  u8g2_L.begin();
  u8g2_L.setDrawColor(1);
  u8g2_L.setFont(u8g2_font_bitcasual_t_all);
  bmp280.begin(BMP280_I2C_ALT_ADDR); 
  sgp.begin();

  // Inicjacja przycisków
  
  pinMode(RIGHT_B, INPUT_PULLUP);
  pinMode(LEFT_B, INPUT_PULLUP);
  pinMode(OK_B, INPUT_PULLUP);
  pinMode(CANCEL_B, INPUT_PULLUP);

  // Rysunki

  for (byte i = 0; i < 5; i++){
    rysunekWstep(i);
    delay(750 + (int(i/4) *500));
  }    
   przygotowanie();
   menuPozycja = 0;
   menuIndeks = indeksy[0];
   menuRozmiar = rozmiary[0];
}


void loop() {
  bool pinLogic;
  if (inputDelay != 0){
    if (inputDelay == CANCEL_B){
      pinLogic = 0;
    } else{
      pinLogic = 1;
    }
    if (digitalRead(inputDelay) == pinLogic){
      inputDelay = 0;
      delay(10);
    }
  } else{
     menuPozycja = pozycja(menuPozycja, menuIndeks, menuRozmiar);
     int j = menuNawigacja();
    if (j == 1){
      aktywacjaSkryptu(menuPozycja);
    } else if (j == 2){
      menuIndeks = indeksy[indeks];
      menuRozmiar = rozmiary[indeks];
      menuPozycja = menuIndeks;   
    }
  }
  rysunekMenu();
  delay(1);
}
