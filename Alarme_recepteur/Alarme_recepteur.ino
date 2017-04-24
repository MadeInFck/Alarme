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
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Time.h>
#include <TimeLib.h>

int VW_MESSAGE_LEN = 4;
bool intrusion = false;
String device = "I000";
RCSwitch mySwitch = RCSwitch();
WiFiClient client;
WiFiUDP ntpUDP;
int16_t utc = +2;
tmElements_t tm;
NTPClient timeClient(ntpUDP, timeServer, utc*3600, 60000);

void sendNotificationToFirebase(); 
void sendDataToFirebase(String);


void setup() { 
    Serial.begin(9600);	// Debugging only
    Serial.println("Setup");
    pinMode(D5,OUTPUT);
    digitalWrite(D5,LOW);
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

    timeClient.begin();
    timeClient.update();
 
    if (!client.connect(host, 80)) {
      Serial.println("Connection failed");
      return;
    } else {
      Serial.println("Connection succeeded !");
    }
    
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 

    // Initialise the RF module
    mySwitch.enableReceive(13);
    digitalWrite(D5,LOW);
}

void loop()
{
    timeClient.update();
    if (mySwitch.available()) {
      int value = mySwitch.getReceivedValue();
      Serial.println("");
      Serial.println(value);
      if (value == 0) {
        Serial.println("Unknown encoding");
      } else if (value == 1234) {
	      device = "I001";
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
      sendDataToFirebase(device);
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
      client.stop();
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

void sendDataToFirebase(String device) {
  StaticJsonBuffer<200> JSONBuffer;   //Memory pool   
  JsonObject& parsed= JSONBuffer.createObject(); //Parse message
  
  unsigned long epoch = timeClient.getEpochTime();
  time_t utcCalc = epoch ;
  String dateData = "";
  if (month(utcCalc < 10)) {
    dateData = String(day(utcCalc)) + "/0" + String(month(utcCalc)) + "/" + String(year(utcCalc));
  } else {
    dateData = String(day(utcCalc)) + "/" + String(month(utcCalc)) + "/" + String(year(utcCalc));
  }
  parsed["date"] = dateData;
  parsed["time"] = timeClient.getFormattedTime();
  parsed["device"] = device;
  Firebase.push("/intrusions",parsed);
  if (Firebase.failed()) {
    Serial.print("Data send failed:");
    Serial.println(Firebase.error());
  } else {
    Serial.println("Intrusion data send to Firebase !");
  }
}

