/********************************************************************************
 * Ein Wrapper für den Rotaryencoder mit folgenden Funktionen:
 * 
 * Shortpress und Longpress Erkennung
 * 
 * Beim Shortpress wird der Level geändert.
 * Beim Longpress wird lediglich der Longpress festgestellt und kann
 * extern weiter verwendet werden.
*********************************************************************************/
#ifndef _MYENCODER_H_
#define _MYENCODER_H_
#include <Arduino.h>

// Zeitdauer in Millisekunden für einen langen Knopfdruck
#define LONGPRESSTIME 1000
// Zeitdauer in Millisekunden für einen langen Knopfdruck
#define SHORTPRESSTIME 500
// Zeitdauer in Millisekunden für einen langen Knopfdruck
#define MINPRESSTIME 50
// Maximale Anzahl von Levels, die bei einem Longpress durchschritten werden
// 3 => Level 0,1,2 
#define NUMLEVEL 3

struct levStore_t
{
    uint8_t curPos;
    uint8_t minPos;
    uint8_t maxPos;
};


class MyEncoder {

public:

void begin();
void begin(uint8_t startPos);
void loop();

void initLev(uint8_t lev, uint8_t pos, uint8_t min, uint8_t max);

void setPos(uint8_t newPos);
bool newPos();
uint8_t readPos();
//void setLimits(uint8_t minLim, uint8_t maxLim);

void setLev(uint8_t newLev);
bool newLev();
uint8_t readLev();

bool isLongClicked(); 
bool isShortClicked(); 

private:
uint8_t curPos = 0;
levStore_t levStore[NUMLEVEL];
bool posChanged = false;
uint8_t curLev = 0;
uint8_t maxLev = 0;
bool levChanged = false;
bool shortClicked = false;
bool longClicked = false;
unsigned long buttonDownStartTime = 0;
unsigned long minPressTime   = MINPRESSTIME;    //Minimum press time. Do not set too low to avoid bouncing (false press events).
unsigned long shortPressTime = SHORTPRESSTIME;  //how long short press shoud be (maximun).
unsigned long longPressTime  = LONGPRESSTIME;   //how long long press shoud be (minimum).


};

#endif