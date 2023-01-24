#include "DHT.h"
#include <MsTimer2.h>
#include <ArduinoJson.h>
#define ARRAY_SIZE(x) sizeof(x) / sizeof(x[0])  // Gets size of array.
#define DHTTYPE DHT11
#define debug Serial
#define DHTPIN 2

int tempHumid_cooldown = 700;  // Default cooldown for the temp/humid sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  updateSettings();  // Updates settings on startup.
  tempHum();         // Updates settings on startup.

  MsTimer2::set(3600000, updateSettings);             // Timer looks for updates in settings every hour.
  MsTimer2::set(tempHumid_cooldown * 1000, tempHum);  // Timer activates the temp/humid sensor when at the chosen time.
  MsTimer2::start();
}

void loop() {}

void updateSettings() {  // This function runs a get call from get.ino to our api and retuns a json. The json is then deserialized and the values are added to the settings.
  String cooldown = getSetting("1");
  cooldown.remove(0, 1);
  cooldown.remove(cooldown.length() - 1, 1);
  StaticJsonDocument<256> doc;
  deserializeJson(doc, cooldown);
  tempHumid_cooldown = atol(doc[F("value")]);
}

void tempHum() {  // This function reads the temperature and humidity from the temp/humid sensor and sends a post from post.ino to our api.
  float temp_hum_val[2] = { 0 };
  if (!dht.readTempAndHumidity(temp_hum_val)) {
    postSetup(F("3"), String(temp_hum_val[0]));
    postSetup(F("1"), String(temp_hum_val[1]));
  }
}
