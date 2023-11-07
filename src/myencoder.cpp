/*******************************************************
 *  Connection between ESP32 and Rotary Encoder
 *    ESP32              Rotary
 *    ------             -------
 *    3V3                VCC
 *    GND                GND 
 *    GPIO             KEY
 *    GPIO             S1
 *    GPIO             S2
 ********************************************************/
#include "myencoder.h"
#ifdef _MYENCODER_H_
#include "AiEsp32RotaryEncoder.h"

#define SERIAL_DEBUG

#define ROTARY_ENCODER_A_PIN      4
#define ROTARY_ENCODER_B_PIN      0
#define ROTARY_ENCODER_BUTTON_PIN 16

#define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */
//depending on your encoder - try 1,2 or 4 to get expected behaviour
//#define ROTARY_ENCODER_STEPS 1
//#define ROTARY_ENCODER_STEPS 2
#define ROTARY_ENCODER_STEPS 4
//instance for rotary encoder
AiEsp32RotaryEncoder rotary = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

//interrupt handling for rotary encoder
void IRAM_ATTR readRotaryISR() {
  rotary.readEncoder_ISR();
}

void MyEncoder::initLev(uint8_t _lev, uint8_t _pos, uint8_t _min, uint8_t _max) {
    levStore[_lev].curPos = _pos;
    levStore[_lev].minPos = _min;
    levStore[_lev].maxPos = _max;
}

void MyEncoder::setPos(uint8_t newPos) {
  rotary.setEncoderValue(newPos);
  levStore[curLev].curPos = newPos;
  curPos = newPos;
#ifdef SERIAL_DEBUG
  Serial.print("Encoder new Pos: ");
  Serial.println(curPos);
#endif  
}

bool MyEncoder::newPos() {
  bool retval = posChanged;
  posChanged = false;
  return retval;
}

uint8_t MyEncoder::readPos() {
#ifdef SERIAL_DEBUG
  Serial.print("Encoder read Pos: ");
  Serial.println(curPos);
#endif
  levStore[curLev].curPos = curPos;
  posChanged = false;
  return curPos;
}

void MyEncoder::setLev(uint8_t newLev) {
  if ( newLev < NUMLEVEL ) {
    levStore[curLev].curPos = curPos;
    curLev = newLev;
    levChanged = true;
    curPos = levStore[curLev].curPos;
    rotary.setEncoderValue(curPos);
    rotary.setBoundaries(levStore[curLev].minPos, levStore[curLev].maxPos, false);
  }
}

bool MyEncoder::newLev() {
  return levChanged;
}

uint8_t MyEncoder::readLev() {
#ifdef SERIAL_DEBUG
  Serial.print("Encoder readLevel: ");
  Serial.println(curLev);
#endif  
  levChanged = false;
  return curLev;
}

bool MyEncoder::isLongClicked() {
  bool retval = false;
  if ( longClicked ) {
    longClicked = false;
    retval = true;
  }
  return retval;
}

bool MyEncoder::isShortClicked() {
  bool retval = false;
  if ( shortClicked ) {
    shortClicked = false;
    retval = true;
  }
  return retval;
}

void MyEncoder::begin() {
  rotary.begin(); //satrencoder handling
  rotary.setup(readRotaryISR); //register interrupt service routine
  rotary.setBoundaries(levStore[curLev].minPos, levStore[curLev].maxPos, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotary.setEncoderValue(levStore[curLev].curPos);
}

void MyEncoder::loop() {
  if (rotary.encoderChanged()) {
    curPos = rotary.readEncoder(); //get new value for gain
    levStore[curLev].curPos = curPos;
    posChanged = true;
  }
  
  if (rotary.isEncoderButtonDown()) {
#ifdef SERIAL_DEBUG
    Serial.println("Button down");
#endif
    if (buttonDownStartTime == 0) {
      buttonDownStartTime = millis();
    }
  } else {
    if ( buttonDownStartTime > 0) {
      unsigned long deltaTime = millis() - buttonDownStartTime;
      if ( (deltaTime > minPressTime) && (deltaTime < shortPressTime) ) {
        if ( curLev > (NUMLEVEL -2)) {
          setLev(0);
        } else {
          setLev(curLev+1);
        }
        levChanged = true;
        shortClicked = true;
      }
      if ( deltaTime > longPressTime) {
        longClicked = true;
      }
#ifdef SERIAL_DEBUG
      if (longClicked) Serial.println("Longclick detected");
      if (shortClicked) Serial.println("Shortclick detected");
#endif
      buttonDownStartTime = 0;
    }
  }
}


#endif