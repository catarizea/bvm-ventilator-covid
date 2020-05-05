/*
  Lcd.h - Library for printing data on a LCD 2004 via I2C
  Created by Catalin Rizea, May 1, 2020
  Released into the public domain 
*/
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "Lcd.h"
#include "translations.h"

Lcd::Lcd(): _lcd(0x3F, 20, 4)  {}

void Lcd::start() {
  Wire.begin(18, 19);
  
  this->_lcd.init();
  this->_lcd.backlight();
  
  this->_lcd.print(settings);
  this->_lcd.setCursor(0, 1);
  this->_lcd.print(noSettings);
}

void Lcd::setSettings(int volume, int bpm, int inspiration, int expiration) {
  this->_volume = volume;
  this->_bpm = bpm;
  this->_inspiration = inspiration;
  this->_expiration = expiration;
}

void Lcd::printSettings(int volume, int bpm, int inspiration, int expiration) {
  this->setSettings(volume, bpm, inspiration, expiration);
  
  String secondRow = "TV " + String(volume) + " ml";
  String thirdRow = "RR " + String(bpm) + " resp/min";
  String fourthRow = "I:E " + String(inspiration) + ":" + String(expiration);

  this->_lcd.clear();
  this->_lcd.setCursor(0, 0);
  this->_lcd.print(settings);
  this->_lcd.setCursor(0, 1);
  this->_lcd.print(secondRow);
  this->_lcd.setCursor(0, 2);
  this->_lcd.print(thirdRow);
  this->_lcd.setCursor(0, 3);
  this->_lcd.print(fourthRow);
}

String Lcd::getSettingsAsString() {
  return "Volume=" + String(this->_volume) + " BPM=" + String(this->_bpm) + " I:E=" + String(this->_inspiration) + ":" + String(this->_expiration);
}
