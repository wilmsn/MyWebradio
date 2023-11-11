#include "secrets.h"
#include "main.h"
#include <WiFi.h> 
#include "mydisplay.h"
#include "myencoder.h"
#include "Audio.h"
#include <Preferences.h>

#define STATIONS_NUM          10
#define STATIONS_URL_SIZE     150
#define STATIONS_NAME_SIZE    32
#define LEV_OUTTIME           3000

#define DEBUG_SERIAL

enum modetype {
  mode_off,
  mode_radio,
  mode_station,
  mode_lev2
};
enum modetype mode;

struct station_t
{
  char  url[STATIONS_URL_SIZE];
  char  name[STATIONS_NAME_SIZE];
};
station_t station[STATIONS_NUM];

MyDisplay display;
MyEncoder encoder;
Audio audio;
Preferences preferences;


char ntp[] = "de.pool.ntp.org";
char off_str[] = "off";
struct tm timeinfo;
char sttime[21];
unsigned long levStart;
unsigned long secInterv;
uint8_t curVol = 20;
uint8_t curStation = 0;

void initStations() {
  
  strncpy(station[0].url,"http://streams.radio21.de/nrw/mp3-192/web",STATIONS_URL_SIZE);
  strncpy(station[0].name,"Radio 21 NRW",STATIONS_NAME_SIZE);

  strncpy(station[1].url,"http://stream.lokalradio.nrw/4459m27",STATIONS_URL_SIZE);
  strncpy(station[1].name,"Radio Kiepenkerl",STATIONS_NAME_SIZE);

  strncpy(station[2].url,"http://icecast.ndr.de/ndr/njoy/live/mp3/128/stream.mp3",STATIONS_URL_SIZE);
  strncpy(station[2].name,"N-JOY",STATIONS_NAME_SIZE);

  strncpy(station[3].url,"http://rnrw.cast.addradio.de/rnrw-0182/dein90er/low/stream.mp3",STATIONS_URL_SIZE);
  strncpy(station[3].name,"NRW 90er",STATIONS_NAME_SIZE);
  
  strncpy(station[4].url,"http://dispatcher.rndfnk.com/br/br3/live/mp3/low",STATIONS_URL_SIZE);
  strncpy(station[4].name,"Bayern3",STATIONS_NAME_SIZE);

  strncpy(station[5].url,"http://dispatcher.rndfnk.com/hr/hr3/live/mp3/48/stream.mp3",STATIONS_URL_SIZE);
  strncpy(station[5].name,"Hessen3",STATIONS_NAME_SIZE);

  strncpy(station[6].url,"http://radiohagen.cast.addradio.de/radiohagen/simulcast/high/stream.mp3",STATIONS_URL_SIZE);
  strncpy(station[6].name,"Radio Hagen",STATIONS_NAME_SIZE);

  strncpy(station[7].url,"http://wdr-1live-live.icecast.wdr.de/wdr/1live/live/mp3/128/stream.mp3",STATIONS_URL_SIZE);
  strncpy(station[7].name,"WDR1",STATIONS_NAME_SIZE);

  strncpy(station[8].url,"http://icecast.ndr.de/ndr/ndr1niedersachsen/hannover/mp3/128/stream.mp3",STATIONS_URL_SIZE);
  strncpy(station[8].name,"NDR1 Hannover",STATIONS_NAME_SIZE);

  strncpy(station[9].url,"http://icecast.ndr.de/ndr/ndr2/niedersachsen/mp3/128/stream.mp3",STATIONS_URL_SIZE);
  strncpy(station[9].name,"NDR2 Niedersachsen",STATIONS_NAME_SIZE);

}

void audio_info(const char *info){
  if (mode == mode_radio) {
#ifdef SERIAL_DEBUG
    Serial.println("****Info****");
    Serial.println(info);
    Serial.println("---------------");
#endif
  }
}

void audio_showstreamtitle(const char *info){
  if (mode == mode_radio) {
    String sinfo = String(info);
    sinfo.replace("|", "\n");
#ifdef SERIAL_DEBUG
    Serial.println("####Titel####");
    Serial.println(String(info));
    Serial.println("-------------");
#endif
    if ( mode == mode_radio ) {
      display.displayTitle(info);
    }
  }
}

void audio_bitrate(const char *info) {
  if (mode == mode_radio) {
#ifdef DEBUG_SERIAL
    Serial.println("####Bitrate####");
    Serial.println(String(info));
    Serial.println("-------------");
#endif
    if ( mode == mode_radio ) {
      display.displayFooter(info);
    }
  }
}

void audio_showstation(const char *info){
  if (mode == mode_radio) {
    String sinfo = String(info);
    sinfo.replace("|", "\n");
#ifdef DEBUG_SERIAL
    Serial.println("####Station####");
    Serial.println(String(info));
    Serial.println("-------------");
#endif
//    display.displayTitle(sinfo);
  }
}

void switchOff() {
  mode = mode_off;
  display.clear();
  preferences.putInt("curStation", curStation);
  display.displayText(20,50,3,off_str,RED);
  // ToDo: Internetstream abschalten !!!
}

void switchOn() {
  curStation = preferences.getInt("curStation");
#ifdef DEBUG_SERIAL
  Serial.print("Switched ON: Station = ");
  Serial.println(station[curStation].name);
#endif
  if (audio.connecttohost(station[curStation].url)) {
#ifdef DEBUG_SERIAL
    Serial.println("Audio connected");
#endif
    mode = mode_radio;
    show_radio();
  }
//  audio.init();
  mode = mode_radio;  
  show_radio();
}

void changeStation(int _stationNo) {
  char* _station[5];
  if ( _stationNo > 1) {
    _station[0] = station[_stationNo-2].name; 
#ifdef DEBUG_SERIAL
     Serial.println(String(station[_stationNo-2].name));
#endif
  } else { 
        _station[0] = NULL;
  }
  if ( _stationNo > 0) {
    _station[1] = station[_stationNo-1].name; 
#ifdef DEBUG_SERIAL
     Serial.println(String(station[_stationNo-1].name));
#endif
  } else { 
        _station[1] = NULL;
  }
  _station[2] = station[_stationNo].name; 
#ifdef DEBUG_SERIAL
  Serial.println(String(station[_stationNo].name));
#endif
  if ( _stationNo < STATIONS_NUM -1) {
    _station[3] = station[_stationNo+1].name; 
#ifdef DEBUG_SERIAL
    Serial.println(String(station[_stationNo+1].name));
#endif
  } else { 
        _station[3] = NULL;
  }
  if ( _stationNo < STATIONS_NUM -2) {
    _station[4] = station[_stationNo+2].name; 
#ifdef DEBUG_SERIAL
    Serial.println(String(station[_stationNo+2].name));
#endif
  } else { 
        _station[4] = NULL;
  }
  display.stationLayout();
  display.displayStationSelect(_station);
}

void show_radio_header() {
  if ( mode == mode_radio ) {
    if (getLocalTime(&timeinfo)) {
      //get date and time as a string
      strftime(sttime, sizeof(sttime), "%d. %b %Y %H:%M", &timeinfo);
    } else {
      snprintf(sttime,sizeof(sttime),"%s","??. ??? ???? ??:??");
    }
    display.displayHeader(sttime, curVol);
  }
}

void show_radio() {
  if ( mode == mode_radio ) {
    display.radioLayout();
    show_radio_header();
    display.displayStation(station[curStation].name);
    display.displayFooter();
  }
}

void setup() {
#ifdef DEBUG_SERIAL
  Serial.begin(115200);
  Serial.print("WiFi ");
#endif
  curStation = preferences.getInt("curStation");
  mode = mode_radio;
  display.begin();
  display.displayWifi(ssid,false);
  encoder.initLev(0,curVol,0,100);
  encoder.initLev(1,curStation,0,STATIONS_NUM-1);
  encoder.initLev(2,0,0,100);
  encoder.begin();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  uint8_t i=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
    i++;
    display.displayWifiCon(i);
#ifdef DEBUG_SERIAL
    Serial.print(".");
#endif
    if (i>20) ESP.restart();
  }
#ifdef DEBUG_SERIAL
  Serial.println();
  Serial.println("connected");
#endif
  display.displayWifi(ssid,true);
  delay(500);
  initStations();
  configTzTime("CET-1CEST,M3.5.0/03,M10.5.0/03", ntp);
  if (! getLocalTime(&timeinfo)) ESP.restart();
  if (audio.connecttohost(station[curStation].url)) {
#ifdef SERIAL_DEBUG
    Serial.println("Audio connected");
#endif
  }
  show_radio();
#ifdef DEBUG_SERIAL
  Serial.println("Setup Ende");
#endif
  secInterv = millis();
}

void actionLevel_0(bool newLevel) {
  if (newLevel) {
    mode = mode_radio;
  }
  curVol = encoder.readPos();
#ifdef DEBUG_SERIAL
  Serial.print(newLevel? "newLevel ":"oldLevel ");
  Serial.print("Lautstärke: ");
  Serial.println(curVol);
#endif
  if ( curVol == 0 && mode == mode_radio ) {
    switchOff();
  }
  if ( curVol > 0 && mode == mode_off ) {
    switchOn();
  }
  if (newLevel) {
    show_radio();
  } else {
    show_radio_header();
  }
}

void actionLevel_1(bool newLevel) {
  mode = mode_station;
  curStation = encoder.readPos();
  levStart = millis();
  display.stationLayout();
#ifdef DEBUG_SERIAL
  Serial.print("Station: ");
  Serial.println(station[curStation].name);
#endif
  changeStation(curStation);
  if (audio.connecttohost(station[curStation].url)) {
#ifdef DEBUG_SERIAL
    Serial.println("Audio connected");
#endif
  }
}

void actionLevel_2(bool newLevel) {
  mode = mode_lev2;
  levStart = millis();
  display.clear();
#ifdef DEBUG_SERIAL
  Serial.print("Level 3: ");
  Serial.println(encoder.readPos());
#endif
}

void loop() {
  audio.loop();
  encoder.loop();
  if (millis() - secInterv > 60000 || millis() < secInterv) {
    show_radio_header();
    secInterv = millis();
  }
//  Serial.print("levStart: ");
//  Serial.println(levStart);
  if ( levStart > 0) {
    if ( millis() - levStart > LEV_OUTTIME ) {
      encoder.setLev(0);
      levStart = 0;
    }
  }
  if (encoder.newPos() || encoder.newLev()) {
    bool newLev = encoder.newLev();
    switch (encoder.readLev()) {
      case 0:
        actionLevel_0(newLev);
      break;
      case 1:
        actionLevel_1(newLev);
      break;
      default:
        actionLevel_2(newLev);
      break;
    }
#ifdef DEBUG_SERIAL
    Serial.print("Level: ");
    Serial.print(encoder.readLev());
    Serial.print(" Position: ");
    Serial.println(encoder.readPos());
#endif
  }
  if ( encoder.isLongClicked() ) {
    switch (mode) {
      case mode_off:     switchOn();    break;
      case mode_radio:   switchOff();   break;
    }
  }
}