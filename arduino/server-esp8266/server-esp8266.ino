/*
  Server.ino - Sketch file to control a BVM Ventilator for COVID-19
  Created by Catalin Rizea, May 1, 2020
  Released into the public domain 
*/
#include "WebServer.h"
#include "Lcd.h"

WebServer server;
Lcd lcd;

const int translatorOEPin = 4;
const int translatorDelay = 10000;

int prevSettings[] = {0, 0, 0, 0};

void setup(void) {
  pinMode(translatorOEPin, OUTPUT);
  digitalWrite(translatorOEPin, LOW);
  
  Serial.begin(115200);
  
  String serverMessage;
  serverMessage = server.start();
  Serial.println(serverMessage);

  digitalWrite(translatorOEPin, HIGH);
  Serial.println("Translator output enabled. Waiting for " + String(translatorDelay / 1000) + " seconds...");
  delay(translatorDelay);
  
  lcd.start();
}

void loop(void) {
  int settings[4] = {server.getSettings()[0], server.getSettings()[1], server.getSettings()[2], server.getSettings()[3]};
  
  if (settings[0] != prevSettings[0] || settings[1] != prevSettings[1] 
    || settings[2] != prevSettings[2] || settings[3] != prevSettings[3]) {
    lcd.printSettings(settings[0], settings[1], settings[2], settings[3]);

    Serial.println("From WebServer: " + String(settings[0]) + " == " + String(settings[1]) + " == " + String(settings[2]) + " == " + String(settings[4]));
    Serial.println("From Lcd: " + lcd.getSettingsAsString());

    prevSettings[0] = settings[0]; 
    prevSettings[1] = settings[1]; 
    prevSettings[2] = settings[2]; 
    prevSettings[3] = settings[3];
  } else {
    server.handleClient();
  }
}
