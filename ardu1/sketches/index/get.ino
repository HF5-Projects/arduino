#include <Ethernet.h>
#include "secrets.h"

byte mac2[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x94, 0x83 };  // Physical mac address.

char serverName2[] = "192.168.1.10";  // The raspberry pi server.
EthernetClient client2;

String getSetting(String id) {
  if (Ethernet.begin(mac2) == 0) {
    Serial.println(String(F("Failed to configure Ethernet using DHCP")));  // No point in carrying on, so do nothing forevermore.
    while (true)
      ;
  }

  Serial.begin(9600);
  if (client2.connect(serverName2, 80)) {                                                                // Starts client connection, checks for connection
    client2.println(String(F("GET /api/settings/arduino/settings_id/")) + id + String(F(" HTTP/1.0")));  // The server address.
    client2.println(token);
    client2.println(String(F("Host: 192.168.1.10")));
    client2.println();  // End of get request.
  } else {
    Serial.println(String(F("connection failed")));  // Error message if no client connection.
    Serial.println();
  }

  String payload = "";
  bool start = false;

  while (client2.connected() && !client2.available()) delay(1000);  // Waits for data.
  while (client2.connected() || client2.available()) {
    char c = client2.read();  // Reads the data onto c.
    if (c == '[') {
      start = true;
    }
    if (start) {
      payload += c;
    }
  }

  client2.stop();  // Stop client.
  return payload;
}