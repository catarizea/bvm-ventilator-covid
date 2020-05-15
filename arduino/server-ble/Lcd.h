/*
  Lcd.h - Library for printing data on a LCD 2004 via I2C
  Created by Catalin Rizea, May 1, 2020
  Released into the public domain 
*/
#ifndef Lcd_h
#define Lcd_h

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Lcd {
  public:
    Lcd();
    void start();
    void printSettings(int volume, int bpm, int inspiration, int expiration);
    String getSettingsAsString();
    void printNoSettings();
  private:
    void setSettings(int volume, int bpm, int inspiration, int expiration);
    int _volume;
    int _bpm;
    int _inspiration;
    int _expiration;
    LiquidCrystal_I2C _lcd;
};

#endif
