/*
  Server.ino - Sketch file to control a BVM Ventilator for COVID-19
  Created by Catalin Rizea, May 8, 2020
  Released into the public domain 
*/
#include <DHT.h>
#include <Wire.h>
#include "Lcd.h"
#include "ServerBLE.h"

Lcd lcd;
ServerBLE server;

const int I2CSlaveAddress = 4;

const int translatorOEPin = 23;
const int translatorDelay = 10000;

const int size = 4;
int settings[size] = {0, 0, 0, 0};
int prevSettings[size] = {0, 0, 0, 0};

TwoWire I2Ctwo = TwoWire(1);

bool messageSent = false;

const int tempPin = 15;
DHT dht(tempPin, DHT11);

const int photoPin = 35;
const int soundPin = 32;

unsigned long previousMillis = 0;
const long sensorsInterval = 100; 

void setup(void) {
  pinMode(photoPin, INPUT);
  pinMode(soundPin, INPUT);
  
  pinMode(translatorOEPin, OUTPUT);
  digitalWrite(translatorOEPin, LOW);
  
  Serial.begin(115200);

  server.start();

  digitalWrite(translatorOEPin, HIGH);
  Serial.println("Translator output enabled. Waiting for " + String(translatorDelay / 1000) + " seconds...");
  delay(translatorDelay);
  
  I2Ctwo.begin(21, 22, 100000);
  
  lcd.start();
  dht.begin();
}

void loop(void) {
  refreshSettings();
  
  if (checkIfNewValues(settings, prevSettings, size) == true) {
    lcd.printSettings(settings[0], settings[1], settings[2], settings[3]);

    Serial.println("From Lcd: " + lcd.getSettingsAsString());
    
    I2Ctwo.beginTransmission(I2CSlaveAddress);
    
    for (byte i = 0; i < size; i++) {
      char t[30];
      dtostrf(settings[i], 1, 0, t);
      I2Ctwo.write(t);
      if (i < size - 1) {
        I2Ctwo.write("x");
      }
      prevSettings[i] = settings[i];
    }

    I2Ctwo.endTransmission();
    
    delay(500);
  } else {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= sensorsInterval) {
      previousMillis = currentMillis;
      
      int tempVal = dht.readTemperature() * 10;
      int photoVal = analogRead(photoPin);
      int soundVal = analogRead(soundPin);
      
      Serial.println("Photo sensor " + String(photoVal));
      Serial.println("Sound sensor " + String(soundVal));
      Serial.println("Temp sensor " + String(tempVal));
      
      String cumulated = String(soundVal) + "x" + String(photoVal) + "x" + String(tempVal);
      
      char buffer[cumulated.length() + 1];
      cumulated.toCharArray(buffer, cumulated.length() + 1);
      
      server.setSensorsValue((char*)&buffer);
    }
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

void refreshSettings() {
  std::string value = server.getSettings();

  if (value.length() == 0) {
    return;
  }

  // Serial.println(value.c_str());
  
  int j = 0;
  String currentValue = "";
  
  for (int i = 0; i < value.length(); i++) {
    if (String(value[i]) != "x") {
      currentValue = currentValue + String(value[i]);
    } else {
      if (settings[j] != currentValue.toInt()) {
        settings[j] = currentValue.toInt();
      }
      currentValue = "";
      j++;
    }
    if (i == value.length() - 1) {
      settings[j] = currentValue.toInt();
    }
  }
}
