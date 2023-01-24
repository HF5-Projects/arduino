#include "DHT.h"
#include <MsTimer2.h>
#include <ArduinoJson.h>
// #include <Wire.h>
// #include <TimeLib.h>
// #include <DS1307RTC.h>
// #include <string>
/* #include <RTClib.h>
 */
#define ARRAY_SIZE(x) sizeof(x) / sizeof(x[0])
#define DHTTYPE DHT11

// #if defined(ARDUINO_ARCH_AVR)
#define debug Serial
#define DHTPIN 2

char serverRoom_open = ":00:00";
char serverRoom_close = ":00:00";
int noise_tooHigh_alert = 700;
const int pinAdc = A0;
/* RTC_DS3231 rtc;
 */

void setup() {
  Serial.begin(9600);
  updateSettings();
/*   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
 */  MsTimer2::set(3600000, updateSettings);
  MsTimer2::start();
}

void loop() {
  sound();
}

void updateSettings() {
  String options[3] = { F("6"), F("7"), F("8") };
  for (byte i = 0; i < ARRAY_SIZE(options); i++) {
    String cooldown = getSetting(options[i]);
    cooldown.remove(0, 1);
    cooldown.remove(cooldown.length() - 1, 1);
    StaticJsonDocument<256> doc;
    deserializeJson(doc, cooldown);
    const char* test = doc[F("value")];
    int number = atol(test);  //notice the function change to atoL

    if (options[i] == "6") {
      serverRoom_open = test;
    }
    if (options[i] == "7") {
      serverRoom_close = test;
    }
    if (options[i] == "8") {
      noise_tooHigh_alert = number;
    }
  }
}

// Sound
void sound() {
  long sum = 0;
  for (int i = 0; i < 32; i++) { sum += analogRead(pinAdc); }
  sum >>= 5;
  Serial.println(sum);
  delay(100);

/*   DateTime now = rtc.now();
  Serial.println(now.hour()); */


  if (sum > noise_tooHigh_alert) {
    Serial.println(String(sum));
    postSetup(F("2"), String(sum));
    delay(10000);
  }
}
