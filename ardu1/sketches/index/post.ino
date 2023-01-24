#include <SPI.h>
#include <Ethernet.h>
#include "secrets.h"

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0xAB, 0x95 };  // Physical mac address.

char serverName[] = "192.168.1.10";  // The raspberry pi server.
EthernetClient client;


void postSetup(String endPoint, String data) {
  if (Ethernet.begin(mac) == 0) {  // Tests the ethernet connection.
    while (true)
      ;
  }

  Serial.begin(9600);
  String body = "{\"message\":\"" + data + "\"}";                                                       // The data body.
  if (client.connect(serverName, 80)) {                                                                 // Starts client connection, checks for connection.
    client.println(String(F("POST /api/arduinoLogs/device_id/")) + endPoint + String(F(" HTTP/1.0")));  // Server endpoint.
    client.println(token);                                                                              // The bearer token from secrets.h.
    client.println(String(F("Host: 192.168.1.10")));
    client.println(String(F("Content-Type: application/json")));
    client.print(String(F("Content-Length: ")));
    client.println(body.length());
    client.println();
    client.println(body);
    client.println();

    client.println();
  } else {
    Serial.println(String(F("connection failed")));  // Error message if no client connection.
    Serial.println();
  }

  while (client.connected() && !client.available()) delay(1);  // Waits for data.
  while (client.connected() || client.available()) {           // Connected or data available.
    char c = client.read();                                    // Gets char from ethernet buffer.
    Serial.print(c);                                           // Prints char to serial monitor
  }
  client.stop();  // Stop client.
}