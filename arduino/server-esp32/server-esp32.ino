/*
  Server.ino - Sketch file to control a BVM Ventilator for COVID-19
  Created by Catalin Rizea, May 1, 2020
  Released into the public domain 
*/
#include <Wire.h>
#include "WBServer.h"
#include "Lcd.h"

WBServer server;
Lcd lcd;

const int I2CSlaveAddress = 4;

const int translatorOEPin = 23;
const int translatorDelay = 10000;

int prevSettings[] = {0, 0, 0, 0};

TwoWire I2Ctwo = TwoWire(1);

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
  I2Ctwo.begin(21, 22, 100000);
  lcd.start();
}

void loop(void) {
  const int size = 4;
  int settings[size] = {server.getSettings()[0], server.getSettings()[1], server.getSettings()[2], server.getSettings()[3]};
  
  if (checkIfNewValues(settings, prevSettings, size) == true) {
    lcd.printSettings(settings[0], settings[1], settings[2], settings[3]);

    String fromServer = "From WebServer: ";
    
    for (byte i = 0; i < size; i++) {
      prevSettings[i] = settings[i];
      fromServer = fromServer + String(settings[i]);
      if (i < size - 1) {
        fromServer = fromServer + " == ";
      }
    }

    Serial.println(fromServer);
    Serial.println("From Lcd: " + lcd.getSettingsAsString());
    
    I2Ctwo.beginTransmission(I2CSlaveAddress);
    
    for (byte i = 0; i < size; i++) {
      char t[30];
      dtostrf(settings[i], 1, 0, t);
      I2Ctwo.write(t);
      if (i < size - 1) {
        I2Ctwo.write("x");
      }
    }

    I2Ctwo.endTransmission();
    
    delay(500);
  } else {
    server.handleClient();
  }
}

bool checkIfNewValues(int *arr1, int *arr2, byte size) {
  bool hasNewValues = false;
  
  for (byte i = 0; i < size; i++) {
    if (hasNewValues == false && arr1[i] != arr2[i]) {
      hasNewValues = true;
    }
  }

  return hasNewValues;
}
