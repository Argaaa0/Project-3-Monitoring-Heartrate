#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "Index.h" //HTML webpage contents with javascripts
const char* ssid = "Galaxy";  //Your wifi name
const char* password = "12345678"; //Your wifi password
ESP8266WebServer server(80);
unsigned long previousMillisGetHR = 0; 
unsigned long previousMillisHR = 0; 
const long intervalGetHR = 10; 
const long intervalHR = 10000;
const int PulseSensorHRWire = A0; //PulseSensor connected to ANALOG PIN 0.
int Threshold = 600; 
int cntHB = 0;
boolean ThresholdStat = true;
int BPMval = 0; 

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleHeartRate() {
  String BPMvalSend = String(BPMval); 
  server.send(200, "text/plane", BPMvalSend); //Sends BPM value to client request / browser.
}

void GetHeartRate() {
  unsigned long currentMillisGetHR = millis();
  if (currentMillisGetHR - previousMillisGetHR >= intervalGetHR) {
    previousMillisGetHR = currentMillisGetHR;
    int PulseSensorHRVal = analogRead(PulseSensorHRWire);
    if (PulseSensorHRVal > Threshold && ThresholdStat == true) {
      cntHB++;
      ThresholdStat = false;
    }
    if (PulseSensorHRVal < Threshold) {
      ThresholdStat = true;
    }
  }
  unsigned long currentMillisHR = millis();
  if (currentMillisHR - previousMillisHR >= intervalHR) {
    previousMillisHR = currentMillisHR;
    BPMval = cntHB * 6;
    Serial.print("BPM : ");
    Serial.println(BPMval);    
    cntHB = 0;
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); 
  Serial.println("");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  server.on("/", handleRoot); 
  server.on("/getHeartRate", handleHeartRate); 
  server.begin(); 
  Serial.println("HTTP server started");
  Serial.println();
}

void loop() {
  server.handleClient(); 
  GetHeartRate(); 
}
