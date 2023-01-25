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
  Wire.begin();
  dht.begin();
}

void loop() {
  tempHum();
}

void updateSettings() {  // This function runs a get call from get.ino to our api and retuns a json. The json is then deserialized and the values are added to the settings.
  String cooldown = getSetting("1");
  cooldown.remove(0, 1);
  cooldown.remove(cooldown.length() - 1, 1);
  StaticJsonDocument<256> doc;
  deserializeJson(doc, cooldown);
  Serial.println(cooldown);
  tempHumid_cooldown = atol(doc[F("value")]);
}

void tempHum() {  // This function reads the temperature and humidity from the temp/humid sensor and sends a post from post.ino to our api.
  updateSettings();
  float temp_hum_val[2] = { 0 };
  if (!dht.readTempAndHumidity(temp_hum_val)) {
    if (temp_hum_val[0] > 0) {
      postSetup(F("3"), String(temp_hum_val[0]));
      postSetup(F("1"), String(temp_hum_val[1]));
    }
  }
  delay(tempHumid_cooldown * 1000); // The cooldown from the settings are applied here.
}
