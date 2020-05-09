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

ServerBLE::ServerBLE() {}

void ServerBLE::start() {
  BLEDevice::init("ESP32 BVM Device No. 1");
  this->_server = BLEDevice::createServer();

  BLEService *flowService = this->createBLEService(
    0,
    SERVICE_UUID_FLOW,
    CHARACTERISTIC_UUID_FLOW
  );

  BLEService *volumeService = this->createBLEService(
    1,
    SERVICE_UUID_VOLUME,
    CHARACTERISTIC_UUID_VOLUME
  );

  BLEService *pressureService = this->createBLEService(
    2,
    SERVICE_UUID_PRESSURE,
    CHARACTERISTIC_UUID_PRESSURE
  );

  flowService->start();
  volumeService->start();
  pressureService->start();

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_UUID_FLOW);
  advertising->addServiceUUID(SERVICE_UUID_VOLUME);
  advertising->addServiceUUID(SERVICE_UUID_PRESSURE);
  advertising->setScanResponse(true);
  advertising->setMinPreferred(0x06);
  advertising->setMinPreferred(0x12);

  BLEDevice::startAdvertising();
  
  Serial.println("\nThe BLE server is ready!");
}

BLEService* ServerBLE::createBLEService(
  byte type,  
  char* serviceUUID, 
  char* characteristicUUID
) {
  BLEService *service = this->_server->createService(serviceUUID);
  
  BLECharacteristic* characteristic = service->createCharacteristic(
    characteristicUUID,
    BLECharacteristic::PROPERTY_READ | 
    BLECharacteristic::PROPERTY_NOTIFY
  );
  
  characteristic->addDescriptor(new BLE2902());

  switch (type) {
    case 0:
      this->_flowCharacteristic = characteristic;
      break;
    case 1:
      this->_volumeCharacteristic = characteristic;
      break;
    case 2:
      this->_pressureCharacteristic = characteristic;
      break;
  }

  return service;
}

void ServerBLE::setSensorValue(byte sensor, uint16_t value) {
  char buffer[20];
  
  switch (sensor) {
    case 0:
      dtostrf(value, 1, 2, buffer);
      this->_flowCharacteristic->setValue((char*)&buffer);
      this->_flowCharacteristic->notify();
      break;
    case 1:
      dtostrf(value, 1, 0, buffer);
      this->_volumeCharacteristic->setValue((char*)&buffer);
      this->_volumeCharacteristic->notify();
      break;
    case 2:
      dtostrf(value, 1, 4, buffer);
      this->_pressureCharacteristic->setValue((char*)&buffer);
      this->_pressureCharacteristic->notify();
      break;
  }
}

