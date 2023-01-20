#include <SPI.h>
#include <Ethernet.h>
#include "secrets.h"

byte mac2[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x94, 0x83 };  //physical mac address

char serverName2[] = "192.168.1.10";  // The raspberry pi server
EthernetClient client2;

String getSettings() {
  String x;
  if (Ethernet.begin(mac2) == 0) {
    Serial.println(String(F("Failed to configure Ethernet using DHCP")));
    // no point in carrying on, so do nothing forevermore:
    while (true);
  }

  Serial.begin(9600);
  if (client2.connect(serverName2, 80)) {  //starts client connection, checks for connection
    Serial.println(String(F("connected")));
    client2.println(String(F("GET /api/settings/arduino HTTP/1.0")));
    client2.println(token);
    client2.println(String(F("Host: 192.168.1.10")));
    client2.println();  //end of get request
  } else {
    Serial.println(String(F("connection failed")));  //error message if no client connect
    Serial.println();
  }

  while (client2.connected() && !client2.available()) delay(1);  //waits for data
  while (client2.connected() || client2.available()) {
    char c = client2.read();  //gets byte from ethernet buffer
    x += c;         //prints byte to serial monitor
  }

  Serial.println();
  Serial.println("disconnecting.");
  Serial.println();
  client2.stop();  //stop client
  return x;
}