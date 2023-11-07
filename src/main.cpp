#include "secrets.h"
#include "main.h"
#include <WiFi.h> 
#include "mydisplay.h"
#include "myencoder.h"
#include "Audio.h"

#define STATIONS_NUM          10
#define STATIONS_URL_SIZE     150
#define STATIONS_NAME_SIZE    32
#define LEV_OUTTIME           3000

struct station_t
{
  char  url[STATIONS_URL_SIZE];
  char  name[STATIONS_NAME_SIZE];
};

station_t station[STATIONS_NUM];

MyDisplay display;
MyEncoder encoder;
Audio audio;


char ntp[] = "de.pool.ntp.org";
struct tm timeinfo;
char sttime[21];
unsigned long levStart;
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
    Serial.println("****Info****");
    Serial.println(info);
    Serial.println("---------------");
}

void audio_showstreamtitle(const char *info){
    String sinfo = String(info);
    sinfo.replace("|", "\n");
    Serial.println("####Titel####");
    Serial.println(String(info));
    Serial.println("-------------");
    display.displayTitle(info);
}

void audio_bitrate(const char *info) {
    Serial.println("####Bitrate####");
    Serial.println(String(info));
    Serial.println("-------------");
    display.displayFooter(info);
}

void audio_showstation(const char *info){
    String sinfo = String(info);
    sinfo.replace("|", "\n");
    Serial.println("####Station####");
    Serial.println(String(info));
    Serial.println("-------------");
//    display.displayTitle(sinfo);
}

void changeStation(int _stationNo) {
  char* _station[5];
  Serial.println("M0-0");
  if ( _stationNo > 1) {
    _station[0] = station[_stationNo-2].name; 
     Serial.println(String(station[_stationNo-2].name));
  } else { 
        _station[0] = NULL;
  }
  Serial.println("M0-1");
  if ( _stationNo > 0) {
    _station[1] = station[_stationNo-1].name; 
     Serial.println(String(station[_stationNo-1].name));
  } else { 
        _station[1] = NULL;
  }
  Serial.println("M0-2");
  _station[2] = station[_stationNo].name; 
  Serial.println(String(station[_stationNo].name));
  Serial.println("M0-3");
  if ( _stationNo < STATIONS_NUM -1) {
    _station[3] = station[_stationNo+1].name; 
     Serial.println(String(station[_stationNo+1].name));
  } else { 
        _station[3] = NULL;
  }
  Serial.println("M0-4");
  if ( _stationNo < STATIONS_NUM -2) {
    _station[4] = station[_stationNo+2].name; 
     Serial.println(String(station[_stationNo+2].name));
  } else { 
        _station[4] = NULL;
  }
  Serial.println("M1");
  display.stationLayout();
  Serial.println("M2");
  display.displayStationSelect(_station);
  Serial.println("M3");
}

void setup() {
  Serial.begin(115200);
  Serial.print("WiFi ");
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
    Serial.print(".");
    if (i>20) ESP.restart();
  }
  Serial.println();
  Serial.println("connected");
  display.displayWifi(ssid,true);
  delay(500);
  initStations();
  configTzTime("CET-1CEST,M3.5.0/03,M10.5.0/03", ntp);
  if (! getLocalTime(&timeinfo)) ESP.restart();
  if (audio.connecttohost(station[curStation].url)) Serial.println("Audio connected");
  display.radioLayout();
  display.displayHeader(sttime, curVol);
  display.displayStation(station[curStation].name);
  Serial.println("Setup Ende");
}

void actionLevel_0() {
  curVol = encoder.readPos();
  Serial.print("Lautstärke: ");
  Serial.println(curVol);
  display.displayHeader(sttime, curVol);
  display.displayFooter();
  audio.setVolume(curVol/5);
}

void actionLevel_1() {
  curStation = encoder.readPos();
  Serial.print("Station: ");
  Serial.println(station[curStation].name);
  changeStation(curStation);
  if (audio.connecttohost(station[curStation].url)) Serial.println("Audio connected");
}

void actionLevel_2() {
  Serial.print("Level 3: ");
  Serial.println(encoder.readPos());
}

void loop() {
  audio.loop();
  encoder.loop();
  if ( levStart > 0) {
    if ( millis() - levStart > LEV_OUTTIME ) {
      encoder.setLev(0);
      levStart = 0;
    }
  }
  if (getLocalTime(&timeinfo)) {
    //get date and time as a string
    strftime(sttime, sizeof(sttime), "%d. %b %Y %H:%M", &timeinfo);
  } else {
    snprintf(sttime,sizeof(sttime),"%s","??. ??? ???? ??:??");
  }
  if (encoder.newPos()) {
    levStart = millis();
    Serial.print("Level: ");
    Serial.print(encoder.readLev());
    Serial.print(" Position: ");
    Serial.println(encoder.readPos());
    switch (encoder.readLev()) {
      case 0:
        actionLevel_0();
      break;
      case 1:
        actionLevel_1();
      break;
      case 2:
        actionLevel_2();
      break;
      default:
      break;
    }
  }
  if ( encoder.newLev() ) {
    levStart = millis();
    Serial.print("Level geschaltet: ");
    Serial.print(encoder.readLev());
    Serial.print(" Position: ");
    Serial.println(encoder.readPos());
    switch (encoder.readLev()) {
      case 0:
        display.radioLayout();
        display.displayStation(station[curStation].name);
        actionLevel_0();
      break;
      case 1:
        display.stationLayout();
        actionLevel_1();
      break;
      case 2:
        display.clear();
        actionLevel_2();
      break;
      default:
      break;
    }
  }
}