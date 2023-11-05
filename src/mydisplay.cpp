/****************************************************************
 *  Connection between ESP32 and the TFT 128x160 pixels Display
 *    ESP32              Display
 *    ------             -------
 *    3V3                VCC
 *    GND                GND
 *    GPIO 18            SCK
 *    GPIO 23            SDA
 *    GPIO 17            DC
 *    GPIO 5             CS
 *    GPIO 22            RES
 *    GPIO 21            BL
 ****************************************************************/
#include "mydisplay.h"
#ifdef _MYDISPLAY_H_
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Fonts/FreeSans9pt7b.h>
#include <SPI.h>

#define TFT_SCK    18  //SCK
#define TFT_SDA    23  //MOSI
#define TFT_DC     17  
#define TFT_CS     5   //SS
#define TFT_RES    22
#define TFT_BL     21

//MOSI: 23
//MISO: 19
//SCK: 18
//SS: 5

//#define SERIAL_DEBUG

//  Adafruit_ST7735(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst);
//  Adafruit_ST7735(int8_t cs, int8_t dc, int8_t rst);
//Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_SDA, TFT_SCK, TFT_RES);
Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RES);

MyDisplay::MyDisplay() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
}

void MyDisplay::begin() {
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.enableDisplay(true);        // Enable display
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(3);
}

void MyDisplay::loop() {
  
}

void MyDisplay::clear() {
  tft.fillRect(0,0,168,120,ST77XX_BLACK);
}

void MyDisplay::infoLayout() {
  tft.setTextSize(3);
  tft.setTextColor(ST7735_RED);
  tft.setCursor(0, 50);
  tft.print("InfoLayout");
}

void MyDisplay::displayWifiCon(uint8_t waitCount) {
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(10,70);
  for(int i=0; i<waitCount; i++) {
    tft.print(".");
  }
}

void MyDisplay::displayWifi(const char* _ssid, bool isCon) {
  String ssid = String(_ssid);
  uint8_t wifi_x = 50;
  clear();
  if (isCon) {
    tft.setTextSize(2);
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(80-strlen(_ssid)*5, wifi_x);
    tft.print(ssid);
    tft.setTextSize(1);
    tft.setTextColor(ST7735_GREEN);
    tft.setCursor(0, 90);
    tft.print("connected");
  } else {
    tft.setTextSize(1);
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(0, 10);
    tft.print("Try to connect to");
    tft.setTextSize(2);
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(80-strlen(_ssid)*5, wifi_x);
    tft.print(ssid);
  }
}

void MyDisplay::displayInfo() {
  tft.fillRect(0,0,168,120,ST77XX_BLACK);
}
void MyDisplay::stationLayout() {}
void MyDisplay::displayStationSelect(char station[5]) {}

void MyDisplay::radioLayout() {
  clear();
  tft.drawLine(0,70,168,70,ST7735_BLUE);
}

void MyDisplay::displayTitle(String titleName) {
  tft.fillRect(0,60,168,128,ST7735_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(0,80);
  tft.print(String(titleName));
  Serial.println("MyDisplay::displayTitle");
  Serial.println(titleName);
  Serial.println("---------------");
  tft.drawLine(0,125,168,125,ST7735_BLUE);
}

void MyDisplay::displayStation(char *stationName) {
//  String myStation;
  tft.fillRect(0,12,168,58,ST77XX_BLACK);
  if (strlen(stationName)> 12) {
    for (int i = 0; i < strlen(stationName); i++){
      if (stationName[i] == ' ') stationName[i] = '\n';
    }
  }
  tft.setTextSize(2);
  tft.setTextColor(ST7735_RED);
  tft.setCursor(5,20);
  tft.print(String(stationName));
}

void MyDisplay::displayHeader(char* dateTime, uint8_t myVol) {
  /*
  *  Zeichnet den oberen Bereich (12px) im Display mit
  *  1. Uhrzeit
  *  2. Lautstärkeinfo
  * 
  *  Ein Dreieck als Sybol für die Lautstärke
  *  myVol: zwischen 0 (=Ton aus) und 100 (=max. Lautst.)
  *  Maße: 25 Breit; 10 Hoch 
  */  
  tft.fillRect(0,0,168,12,ST77XX_BLUE);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_YELLOW);
  tft.setCursor(5,2);
  tft.print(String(dateTime));
  uint8_t h = 10;
  uint8_t w = 25;
  uint16_t x0,x1,x2,y0,y1,y2;
  x0 = 130;
  x1 = x0 + myVol*w/100;
  x2 = x0 + w;
  y0 = 10;
  y1 = y0 - myVol*h/100;
  y2 = y0 - h;
  //tft.fillTriangle(x0,y0,x2,y2,x2,y0,ST77XX_BLACK);
  //tft.drawTriangle(x0,y0,x2,y2,x2,y0,ST77XX_YELLOW);
  //tft.setTextSize(1);
  //tft.setTextColor(ST7735_YELLOW);
  //tft.setCursor(121, 0);
  //tft.print(myVol);
  tft.fillTriangle(x0,y0,x1,y1,x1,y0,ST7735_YELLOW);

}





#endif