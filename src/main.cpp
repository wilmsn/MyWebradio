#include "secrets.h"
#include <WiFi.h> 
#include "myencoder.h"

#define STATIONS_NUM          10
#define STATIONS_URL_SIZE     150
#define STATIONS_NAME_SIZE    32

struct station_t
{
  char  url[STATIONS_URL_SIZE];
  char  name[STATIONS_NAME_SIZE];
};

station_t station[STATIONS_NUM];

MyEncoder encoder;

void initStations() {
  
  strncpy(station[0].url,"http://streams.radio21.de/nrw/mp3-192/web",STATIONS_URL_SIZE);
  strncpy(station[0].name,"Radio 21 NRW",STATIONS_NAME_SIZE);

  strncpy(station[1].url,"http://stream.lokalradio.nrw/4459m27",STATIONS_URL_SIZE);
  strncpy(station[1].name,"Radio Kiepenkerl",STATIONS_NAME_SIZE);

  strncpy(station[2].url,"http://icecast.ndr.de/ndr/njoy/live/mp3/128/stream.mp3",STATIONS_URL_SIZE);
  strncpy(station[2].name,"N-JOY",STATIONS_NAME_SIZE);

  strncpy(station[3].url,"http://rnrw.cast.addradio.de/rnrw-0182/dein90er/low/stream.mp3",STATIONS_URL_SIZE);
  strncpy(station[3].name,"NRW 90er",STATIONS_NAME_SIZE);
  
  strncpy(station[4].name,"fill me",STATIONS_NAME_SIZE);
  strncpy(station[5].name,"fill me",STATIONS_NAME_SIZE);
  strncpy(station[6].name,"fill me",STATIONS_NAME_SIZE);
  strncpy(station[7].name,"fill me",STATIONS_NAME_SIZE);
  strncpy(station[8].name,"fill me",STATIONS_NAME_SIZE);
  strncpy(station[9].name,"fill me",STATIONS_NAME_SIZE);

}

void setup() {
  Serial.begin(115200);
  Serial.print("WiFi ");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
    Serial.print(".");
  }
  Serial.println(" connected");
  encoder.initLev(0,5,0,20);
  encoder.initLev(1,0,0,STATIONS_NUM-1);
  encoder.initLev(2,0,0,100);
  encoder.begin();
  initStations();
 }

void actionLevel_0() {
  Serial.print("Lautstärke: ");
  Serial.println(encoder.readPos());
}

void actionLevel_1() {
  Serial.print("Station: ");
  Serial.println(station[encoder.readPos()].name);
}

void actionLevel_2() {
  Serial.print("Level 3: ");
  Serial.println(encoder.readPos());
}

void loop() {
  encoder.loop();
  if (encoder.newPos()) {
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
    Serial.print("Level geschaltet: ");
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
}