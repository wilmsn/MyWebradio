/********************************************************************************
 * Ein Wrapper für den Rotaryencoder mit folgenden Funktionen:
 * 
 * Shortpress und Longpress Erkennung
 * 
 * Beim Shortpress wird der Level geändert.
 * Beim Longpress wird lediglich der Longpress festgestellt und kann
 * extern weiter verwendet werden.
*********************************************************************************/
#ifndef _MYDISPLAY_H_
#define _MYDISPLAY_H_
#include <Arduino.h>

#define LINESIZE 20

class MyDisplay {

  public:
    MyDisplay();
    void begin();
    void loop();
    void clear();
    void infoLayout();
    void displayInfo();
    void stationLayout();
    void displayStationSelect(char station[5]);
    void radioLayout();
    void displayStation(char *stationName);
    void displayTitle(String titleName);
    void displayHeader(char* dateTime, uint8_t myVol);
    void displayWifi(const char* _ssid, bool isCon);
    void displayWifiCon(uint8_t waitCount);
    void display();

  private:

};






#endif