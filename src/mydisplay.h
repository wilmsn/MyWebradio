/********************************************************************************
 * Ein Wrapper für das Display mit folgenden Funktionen:
 * 
 * Je ein definiertes Seitenlayout für
 * 1) Radio im Normalbetrieb
 * 2) Stationswahl
 * zusätzlich generische Funktionen zur Anzeige von Text
 * 
 ********************************************************************************/
#ifndef _MYDISPLAY_H_
#define _MYDISPLAY_H_
#include <Arduino.h>

class MyDisplay {

  public:
    MyDisplay();
    void begin();
    // Generische Funktionen
    // Löscht den Bildschirm (schwarz)
    void clear();
    // Ausgabe eines Textes mit Parametern
    void DisplayText(uint8_t x0, uint8_t y0, uint8_t fontsize, String& text, uint16_t color);
    // Ein Dialog zur Stationswahl
    void stationLayout();
    void displayStationSelect(char* station[5]);
    // Ein Dialog zum Betrieb des Radios
    void radioLayout();
    void displayStation(char *stationName);
    void displayTitle(const char* titleName);
    void displayHeader(char* dateTime, uint8_t myVol);
    void displayFooter();
    void displayFooter(const char* _bps);
    // Ein Dialog zum Start des Wifi Netzes
    void displayWifi(const char* _ssid, bool isCon);
    void displayWifiCon(uint8_t waitCount);

  private:

};






#endif