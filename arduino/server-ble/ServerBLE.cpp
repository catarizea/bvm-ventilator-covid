/*
  ServerBLE.cpp - Library for ESP32 board
  Created by Catalin Rizea, May 8, 2020
  Released into the public domain 
*/
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "ServerBLE.h"
#include "secret.h"

#define DESCRIPTOR_UUID BLEUUID((uint16_t)0x2901) // GATT Descriptor: Characteristic User Description

class Callback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) {
    std::string value = characteristic->getValue();

    if (value.length() > 0) {
      Serial.print("\nNew value: ");
      for (int i = 0; i < value.length(); i++) {
        Serial.print(value[i]);
      }
      Serial.println();
    }
  }
};

ServerBLE::ServerBLE() {}

void ServerBLE::start() {
  BLEDevice::init("ESP32 BVM Device No. 1");
  this->_server = BLEDevice::createServer();

  BLEService *sensorsService = this->createBLEReadService(
    0,
    SERVICE_UUID_SENSORS,
    CHARACTERISTIC_UUID_SENSORS
  );

  BLEService *settingsService = this->createBLEWriteService(
    0,
    SERVICE_UUID_SETTINGS,
    CHARACTERISTIC_UUID_SETTINGS
  );

  sensorsService->start();
  settingsService->start();

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID_SENSORS);
  advertising->addServiceUUID(SERVICE_UUID_SETTINGS);
  advertising->setScanResponse(true);

  BLEDevice::startAdvertising();
  
  Serial.println("\nThe BLE server is ready!");
}

BLEService* ServerBLE::createBLEReadService(
  byte type,  
  char* serviceUUID, 
  char* characteristicUUID
) {
  BLEService *service = this->_server->createService(serviceUUID);
  
  BLECharacteristic *characteristic = service->createCharacteristic(
    characteristicUUID,
    BLECharacteristic::PROPERTY_READ | 
    BLECharacteristic::PROPERTY_NOTIFY
  );
  
  characteristic->addDescriptor(new BLE2902());

  switch (type) {
    default:
      this->_sensorsCharacteristic = characteristic;
      break;
  }

  return service;
}

BLEService* ServerBLE::createBLEWriteService(
  byte type,  
  char* serviceUUID, 
  char* characteristicUUID
) {
  BLEService *service = this->_server->createService(serviceUUID);
  
  BLECharacteristic *characteristic = service->createCharacteristic(
    characteristicUUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE 
  );

  characteristic->setCallbacks(new Callback());

  switch (type) {
    default:
      this->_settingsCharacteristic = characteristic;
      break;
  }

  return service;
}

void ServerBLE::setSensorsValue(std::string value) {
  this->_sensorsCharacteristic->setValue(value);
  this->_sensorsCharacteristic->notify();
}

std::string ServerBLE::getSettings() {
  return this->_settingsCharacteristic->getValue();
}
