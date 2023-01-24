#include <SPI.h>
#include <Ethernet.h>
#include "secrets.h"

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0xAB, 0x95 };  //physical mac address

char serverName[] = "192.168.1.10";  // The raspberry pi server
EthernetClient client;


void postSetup(String endPoint, String data) {
  //Tests the ethernet connection
  if (Ethernet.begin(mac) == 0) {
    while (true)
      ;
  }

  Serial.begin(9600);
  String body = "{\"message\":\"" + data + "\"}";
  if (client.connect(serverName, 80)) {  //starts client connection, checks for connection
    client.println(String(F("POST /api/arduinoLogs/device_id/")) + endPoint + String(F(" HTTP/1.0")));
    client.println(token);
    client.println(String(F("Host: 192.168.1.10")));
    client.println(String(F("Content-Type: application/json")));
    client.print(String(F("Content-Length: ")));
    client.println(body.length());
    client.println();
    client.println(body);
    client.println();

    client.println();  //end of get request
  } else {
    Serial.println(String(F("connection failed")));  //error message if no client connect
    Serial.println();
  }

  while (client.connected() && !client.available()) delay(1);  //waits for data
  while (client.connected() || client.available()) {           //connected or data available
    char c = client.read();                                    //gets byte from ethernet buffer
    Serial.println(c);                                           //prints byte to serial monitor
    Serial.print(c);                                           //prints byte to serial monitor
  }
  client.stop();  //stop client
}