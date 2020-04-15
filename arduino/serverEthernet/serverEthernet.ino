#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>
#include <aWOT.h>
#include <Arduino_JSON.h>
#include "translations.h"

LiquidCrystal lcd(9, 8, 7, 6, 5, 2);

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = { 192, 168, 1, 177 };
byte dns[] = { 192, 168, 1, 1 };
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[] = { 255, 255, 255, 0 };

EthernetServer server(80);
Application app;

bool isClientConnected = false;

int volume = 0;
int bpm = 0;
int inspDuration = 0;

int prevVolume = 0;
int prevBpm = 0;
int prevInspDuration = 0;

String stringifyAndJoin(byte *arr, byte size, String separator, bool isHex) {
  String result = "";
  for (byte i = 0; i < size; i++) {
    if (isHex == false) {
      result = result + String(arr[i]);  
    } else {
      result = result + String(arr[i], HEX);
    }
    if (i < size - 1) {
      result = result + separator;
    }
  }
  return result;
}

void printServerInfo() {
  Serial.println("Server info:");
  Serial.print("IP ");
  Serial.println(Ethernet.localIP());
  Serial.print("MAC address: ");
  Serial.println(stringifyAndJoin(mac, sizeof(mac), String(":"), true));
}

void index(Request &req, Response &res) {
  JSONVar status;
  
  status["ip"] = stringifyAndJoin(ip, sizeof(ip), String("."), false);
  status["mac"] = stringifyAndJoin(mac, sizeof(mac), String(":"), true);
  status["status"] = "OK";

  String jsonString = JSON.stringify(status);
  
  res.status(200);
  res.set("Content-Type", "application/json");
  res.print(jsonString);
}

void postIndex(Request &req, Response &res) {
  byte buffer[100];

  if(!req.body(buffer, 100)){
    return res.sendStatus(400);
  }

  Serial.write(buffer, 100);

  JSONVar newSettings = JSON.parse(buffer);

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
  
  res.status(200);
  res.set("Content-Type", "application/json");
  res.print(jsonString);
}

void setup() {
  lcd.begin(16, 2);
  lcd.print(settings);
  lcd.setCursor(0, 1);
  lcd.print(noSettings);
  
  Serial.begin(9600);
  
  Ethernet.begin(mac, ip, dns, gateway, subnet);
  printServerInfo();
  
  app.get("/", &index);
  app.post("/", &postIndex);
  server.begin();
}

void loop() {
  if (volume != prevVolume || bpm != prevBpm || inspDuration != prevInspDuration) {
    String firstRow = "Voume " + String(volume) + "ml";
    String secondRow = "BPM " + String(bpm) + " Insp " + String(inspDuration) + "s";
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(firstRow);
    lcd.setCursor(0, 1);
    lcd.print(secondRow);
    
    prevVolume = volume;
    prevBpm = bpm;
    prevInspDuration = inspDuration;
  } else {
    if (!isClientConnected) {
     EthernetClient client = server.available();
      if (client.connected()) {
        isClientConnected = true;
        app.process(&client);
        client.stop();
        isClientConnected = false;
      } 
    }
  } 
}
