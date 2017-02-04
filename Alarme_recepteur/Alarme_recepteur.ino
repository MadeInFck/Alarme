
//============================================================================
//  Récepteur de messages via RF433MHz
//  Allume une LED si réception du bon message
//  
//  14/01/17
//============================================================================
//  Voir aussi  http://www.madeinfck.com
//  Licence Creative Commons https://creativecommons.org/licenses/by-nc-sa/4.0/
//  PIR sensor tester by MadeInFck
// ===========================================================================

#include <RCSwitch.h>
#include <ESP8266WiFi.h>
#include "keys.h"
#include <FirebaseArduino.h>


int VW_MESSAGE_LEN = 4;
bool intrusion = false;

RCSwitch mySwitch = RCSwitch();
WiFiClient client;
void sendNotificationToFirebase(); 
void sendDataToFirebase();


void setup() { 
    Serial.begin(9600);	// Debugging only
    Serial.println("Setup");
    pinMode(D5,OUTPUT);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    while ( WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    // you're connected now, so print out the data:
    Serial.println("You're connected to the network");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(2,HIGH);
     delay(1000);
    Serial.print("connecting to ");
    Serial.println(host);
  
    if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
    }
    
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 

    // Initialise the RF module
    mySwitch.enableReceive(13);
    digitalWrite(D5,LOW);
}

void loop()
{
    if (mySwitch.available()) {
      int value = mySwitch.getReceivedValue();
      Serial.println("");
      Serial.println(value);
      if (value == 0) {
        Serial.println("Unknown encoding");
      } else if (value == 1234) {
	      Serial.print("Got: ");
	      Serial.println("Sensor #1 detected motion!");
        digitalWrite(D5, HIGH);
        delay(5000);
        digitalWrite(D5,LOW);
        intrusion = true;
      }
      mySwitch.resetAvailable();
    }
    if (intrusion) {
      WiFiClient client;
      client.connect(host, 80);
      sendNotificationToFirebase();
      sendDataToFirebase();
      intrusion = false;
    }
    
     if (client.available()) {
     char c = client.read();
     Serial.print(c);
     }

    //if the server's disconnected, stop the client:
    if (!client.connected()) {
      //Serial.print(".");
      //Serial.println("disconnecting.");
      client.flush();
      //client.stop();
    }
}

void sendNotificationToFirebase() {
    String data = "{" ;
    data = data + "\"to\": \"";
    data = data + DEVICE;
    data = data + "\"," ;
    data = data + "\"notification\": {" ;
    data = data + "\"body\": \"Motion detected\"," ;
    data = data + "\"title\" : \"Alarm\" " ;
    data = data + "} }" ;
    Serial.println("Send data...");
    if (client.connect(host, 80)) {
        Serial.println("Connected to the server..");
        client.println("POST /fcm/send HTTP/1.1");
        client.print("Authorization: key=");
        client.println(AUTHKEY);
        client.println("Content-Type: application/json");
        client.println("Host: fcm.googleapis.com");
        client.println("Connection: close");
        client.print("Content-Length: ");
        client.println(data.length());
        client.print("\n");
        client.println(data);
    } else {
      Serial.println("Connection failed!");
    }
    delay(100);
}

void sendDataToFirebase() {
  char data[] = "{ \"date\" : \"03/02/2017\", \"heure\" : \"23:49\", \"device\": \"I001\" }";
  StaticJsonBuffer<300> JSONBuffer;   //Memory pool   
  JsonObject& parsed= JSONBuffer.parseObject(data); //Parse message
  int value = parsed["date"];
  Serial.println(value);
  Firebase.pushString("intrusions",data);
  if (Firebase.failed()) {
    Serial.print("setting /truth failed:");
    Serial.println(Firebase.error());
  }
}

