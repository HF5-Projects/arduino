#include <SPI.h>
#include <Ethernet.h>
#include "secrets.h"

byte mac2[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x94, 0x83 };  //physical mac address

char serverName2[] = "192.168.1.10";  // The raspberry pi server
EthernetClient client2;


void getSetup() {
  //Tests the ethernet connection
  if (Ethernet.begin(mac2) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while (true);
  }
  Serial.begin(9600);
  Serial.println(token);
  sendGet();
}


//client function to send/receive GET request data.
void sendGet()  
{
  if (client2.connect(serverName2, 80)) {  //starts client connection, checks for connection
    Serial.println("connected");
    client2.println("GET /api/arduinoLogs/device_id/1 HTTP/1.0");
    client2.println(token);
    client2.println("Host: 192.168.1.10");
    client2.println();  //end of get request
  } else {
    Serial.println("connection failed");  //error message if no client connect
    Serial.println();
  }

  while (client2.connected() && !client2.available()) delay(1);  //waits for data
  while (client2.connected() || client2.available()) {           //connected or data available
    char c = client2.read();                                    //gets byte from ethernet buffer
    Serial.print(c);                                           //prints byte to serial monitor
  }

  Serial.println();
  Serial.println("disconnecting.");
  Serial.println("==================");
  Serial.println();
  client2.stop();  //stop client
}