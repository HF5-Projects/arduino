#include "DHT.h"
#include <MsTimer2.h>
#include <ArduinoJson.h>

#define ARRAY_SIZE(x) sizeof(x) / sizeof(x[0])
#define DHTTYPE DHT11
#define debug Serial
#define DHTPIN 2

char serverRoom_open = "08:00:00";   // Default serveroom opening time.
char serverRoom_close = "18:00:00";  // Default servertoom closing time.
int noise_tooHigh_alert = 300;       // Default setting for too noisy.
const int pinAdc = A0;               // Sound sensor port.

void setup() {
  Serial.begin(9600);
  updateSettings();
  MsTimer2::set(4000, updateSettings);  // Timer looks for updates in settings every hour.
  MsTimer2::start();
}

void loop() {
  sound();  // Runs time constantly
}

void updateSettings() {  // This function runs a get call from get.ino to our api and retuns a json. The json is then deserialized and the values are added to the settings.
  String options[3] = { F("6"), F("7"), F("8") };
  for (byte i = 0; i < ARRAY_SIZE(options); i++) {
    String cooldown = getSetting(options[i]);
    cooldown.remove(0, 1);
    cooldown.remove(cooldown.length() - 1, 1);
    StaticJsonDocument<256> doc;
    deserializeJson(doc, cooldown);
    const char* test = doc[F("value")];
    int number = atol(test);
    Serial.println(test);

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

void sound() {  // This function reads the sound sensor and checks if the sound is louder than the selected level, if it is then it will send a message to the server via post.ino
  long sum = 0;
  for (int i = 0; i < 32; i++) { sum += analogRead(pinAdc); }
  sum >>= 5;
  delay(100);

  if (sum > noise_tooHigh_alert) {
    Serial.println(String(sum));
    Serial.println("");
    postSetup(F("2"), String(sum));
    delay(10000);
  }
}
