#include "DHT.h"
#include <MsTimer2.h>
#include <ArduinoJson.h>
// #include <Wire.h>
// #include <TimeLib.h>
// #include <DS1307RTC.h>
// #include <string>

#define DHTTYPE DHT11

// #if defined(ARDUINO_ARCH_AVR)
 #define debug Serial
#define DHTPIN 2

// #elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM)
// #define debug SerialUSB
// #else
// #define debug Serial
// #endif

const byte pinAdc = A0;
int maxNoise = 700;

String startClock = "08:00:00";
String endClock = "18:00:00";

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  // Serial.begin(9600);
  // debug.begin(9600);
  // Wire.begin();
  // dht.begin();
  // // tempHum();
  String settings = getSettings();

  DynamicJsonDocument doc(400);
  deserializeJson(doc, settings);
  String sd = doc[0]["value"];
  Serial.println(sd);

  int timeGet = 3600 * 1000;
  int timeTempHum = 900 * 1000;

  MsTimer2::set(timeGet, getSettings);
  MsTimer2::set(timeTempHum, tempHum);
  MsTimer2::start();
}

void loop() {
  // sound();
}


// Temp + Humid
void tempHum() {
  float temp_hum_val[2] = { 0 };
  if (!dht.readTempAndHumidity(temp_hum_val)) {
    /* DynamicJsonDocument doc(200);
    doc["deviceId"] = 1;  // DeviceId 1 Correlates to the temperature&humidtysensor
    doc["temp"] = temp_hum_val[1];
    doc["humid"] = temp_hum_val[0];
    serializeJsonPretty(doc, Serial); */

    postSetup("2", String(temp_hum_val[1]));
    postSetup("3", String(temp_hum_val[0]));
  }
}

// Sound
void sound() {
  byte sum = 0;
  for (byte i = 0; i < 32; i++) {
    sum += analogRead(pinAdc);
  }
  sum >>= 5;

  if (sum > maxNoise) {
    Serial.println(String(sum));
    postSetup("1", String(sum));
    delay(10000);
  }
}
