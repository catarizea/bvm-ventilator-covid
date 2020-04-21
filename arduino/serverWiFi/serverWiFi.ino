#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Arduino_JSON.h>
#include "secret.h"
#include "translations.h"

LiquidCrystal lcd(2, 0, 15, 13, 12, 14);

const int translatorOEPin = 4;
const int translatorDelay = 10000;

const char* ssid     = ssidSecret;
const char* password = passSecret;

ESP8266WebServer server(80);

IPAddress ip(192, 168, 1, 110);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

int volume = 0;
int bpm = 0;
int inspDuration = 0;

int prevVolume = 0;
int prevBpm = 0;
int prevInspDuration = 0;

void handleRoot() {
  if (server.method() == HTTP_GET) {    
    JSONVar status;
    
    status["ip"] = WiFi.localIP().toString();
    status["mac"] = WiFi.macAddress();
    status["status"] = "OK";

    String jsonString = JSON.stringify(status);
    server.send(200, "application/json", jsonString);
  }

  if (server.method() == HTTP_POST) {    
    JSONVar newSettings = JSON.parse(server.arg("plain"));

    if (JSON.typeof(newSettings) == "undefined") {
      return;
    }

    if (newSettings.hasOwnProperty("volume")) {
      volume = int(newSettings["volume"]);
    }

    if (newSettings.hasOwnProperty("bpm")) {
      bpm = int(newSettings["bpm"]);
    }

    if (newSettings.hasOwnProperty("inspDuration")) {
      inspDuration = int(newSettings["inspDuration"]);
    }
    
    JSONVar status;
    status["status"] = "OK";
    String jsonString = JSON.stringify(status);
    
    server.send(200, "application/json", jsonString);
  }

  if (server.method() != HTTP_GET && server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Not Allowed");
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

void setup(void) {
  pinMode(translatorOEPin, OUTPUT);
  digitalWrite(translatorOEPin, LOW);
  
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  digitalWrite(translatorOEPin, HIGH);
  Serial.println("Translator output enabled. Waiting for " + String(translatorDelay / 1000) + " seconds...");
  delay(translatorDelay);
  
  
  lcd.begin(16, 2);
  lcd.print(settings);
  lcd.setCursor(0, 1);
  lcd.print(noSettings);
}

void loop(void) {
  if (volume != prevVolume || bpm != prevBpm || inspDuration != prevInspDuration) {
    String firstRow = "Voume " + String(volume) + "ml";
    String secondRow = "BPM " + String(bpm) + " Insp " + String(inspDuration) + "s";
    
    Serial.println(firstRow);
    Serial.println(secondRow);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(firstRow);
    lcd.setCursor(0, 1);
    lcd.print(secondRow);
    
    prevVolume = volume;
    prevBpm = bpm;
    prevInspDuration = inspDuration;
  } else {
    server.handleClient();
  }
}
