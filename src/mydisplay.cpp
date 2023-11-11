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
//#include <Fonts/FreeSans9pt7b.h>
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
  clear();
}

void MyDisplay::clear() {
  tft.fillRect(0,0,160,128,ST77XX_BLACK);
}

void MyDisplay::displayText(uint8_t x0, uint8_t y0, uint8_t fontsize, const char* text, uint16_t color) {
  tft.setTextSize(fontsize);
  tft.setTextColor(color);
  tft.setCursor(x0, y0);
  tft.print(text);
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
  uint8_t wifi_y = 50;
  clear();
  if (isCon) {
    tft.setTextSize(2);
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(80-strlen(_ssid)*6, wifi_y);
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
    tft.setCursor(80-strlen(_ssid)*6, wifi_y);
    tft.print(ssid);
  }
}

void MyDisplay::stationLayout() {
  clear();

}

void MyDisplay::displayStationSelect(char* station[5]) {
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(10,20);
  if (station[0] != NULL) tft.print(String(station[0]));
  tft.setCursor(10,40);
  if (station[1] != NULL) tft.print(String(station[1]));
  tft.setCursor(10,80);
  if (station[3] != NULL) tft.print(String(station[3]));
  tft.setCursor(10,100);
  if (station[4] != NULL) tft.print(String(station[4]));
  tft.setTextSize(1);
  tft.setTextColor(ST7735_YELLOW);
  tft.setCursor(10,60);
  tft.print(String(station[2]));
  tft.drawRect(0,53,160,20,ST7735_YELLOW);
}

void MyDisplay::radioLayout() {
/*  Folgende Aufteilung des Bildschirmes:
0,0
     Headline mit Datum/Zeit und Lautstärke
                                            159,11
--------------------------------------------------                                            
12,0
     Anzeige des aktuellen Senders
                                            159,69
--------------------------------------------------
70,0      Trennlinie blau                   159,70
--------------------------------------------------
71,0
     Anzeige des aktuellen Titels
                                            159,114
--------------------------------------------------
115,0
     Footer mit techn. Infos
                                            159,127
*/
  clear();
  tft.drawLine(0,70,159,70,ST7735_BLUE);
}

void MyDisplay::displayStation(char *stationName) {
  char mystr1[] = "            ";
  char mystr2[] = "            ";
  int space2cut = 0;
  int strlenMax = strlen(mystr1) + strlen(mystr2);
//  tft.setFont(&FreeSans9pt7bBitmaps);
  tft.fillRect(0,12,160,58,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_RED);
  tft.setCursor(5,20);
  if (strlen(stationName) > strlen(mystr1)) {
    for(int i=0; i<strlen(stationName); i++) {
       if ( i < strlen(mystr1) && stationName[i] == ' ') space2cut = i;
    }    
  }
  Serial.print("Space 2 cut:");
  Serial.println(space2cut);
  if (strlen(stationName) > strlen(mystr1)) {
    if ( space2cut < strlen(mystr1)) {
      for(int i=0; i<strlen(stationName) && i<strlenMax; i++) {
        if ( i < space2cut) mystr1[i] = stationName[i];
        if ( i > space2cut && i-space2cut < strlen(mystr2)) mystr2[i-space2cut-1] = stationName[i];
      }
    } else {
      for(int i=0; i<strlen(stationName) && i<strlenMax; i++) {
        if ( i < 12) mystr1[i] = stationName[i];
        if ( i > 11) mystr2[i-strlen(mystr1)] = stationName[i];
      }
    }
    tft.print(String(mystr1));
    tft.setCursor(5,40);
    tft.print(String(mystr2));
  } else {
    tft.print(String(stationName));
  }
  tft.setFont();
}

void MyDisplay::displayTitle(const char* titleName) {
  char mystr1[] = "                      ";
  char mystr2[] = "                      ";
  uint8_t space2cut = 0;
  int strlenMax = strlen(mystr1) + strlen(mystr2);
  tft.fillRect(0,71,160,44,ST7735_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(5,80);
  if (strlen(titleName) > strlen(mystr1)) {
    for(int i=0; i<strlen(titleName); i++) {
       if ( i < strlen(mystr1) && titleName[i] == ' ') space2cut = i;
    }    
  }
  Serial.print("Space 2 cut:");
  Serial.println(space2cut);
  if (strlen(titleName) > strlen(mystr1)) {
    if ( space2cut < strlen(mystr1)) {
      for(int i=0; i<strlen(titleName) && i<strlenMax; i++) {
        if ( i < space2cut) mystr1[i] = titleName[i];
        if ( i > space2cut && i-space2cut < strlen(mystr2)) mystr2[i-space2cut-1] = titleName[i];
      }
    } else {
      for(int i=0; i<strlen(titleName) && i<strlenMax; i++) {
        if ( i < 12) mystr1[i] = titleName[i];
        if ( i > 11) mystr2[i-strlen(mystr1)] = titleName[i];
      }
    }
    tft.print(String(mystr1));
    tft.setCursor(5,95);
    tft.print(String(mystr2));
  } else {
    tft.print(String(titleName));
  }
  Serial.println("MyDisplay::displayTitle");
  Serial.println(titleName);
  Serial.println("---------------");
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
  tft.fillRect(0,0,159,12,ST77XX_BLUE);
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

void MyDisplay::displayFooter(const char* _bps) {
  displayFooter();
  tft.setTextSize(1);
  tft.setTextColor(ST7735_YELLOW);
  tft.setCursor(2,117);
  tft.print(String(_bps));
}

void MyDisplay::displayFooter() {
  tft.fillRect(0,115,159,12,ST77XX_BLUE);
}
#endif