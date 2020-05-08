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
    CHARACTERISTIC_UUID_FLOW,
    "flow"
  );

  BLEService *volumeService = this->createBLEService(
    1,
    SERVICE_UUID_VOLUME,
    CHARACTERISTIC_UUID_VOLUME,
    "volume"
  );

  BLEService *pressureService = this->createBLEService(
    2,
    SERVICE_UUID_PRESSURE,
    CHARACTERISTIC_UUID_PRESSURE,
    "pressure"
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
  char* characteristicUUID,
  std::string descriptorValue
) {
  BLEService *service = this->_server->createService(serviceUUID);
  
  BLECharacteristic* characteristic = service->createCharacteristic(
    characteristicUUID,
    BLECharacteristic::PROPERTY_READ | 
    BLECharacteristic::PROPERTY_NOTIFY
  );
  
  characteristic->addDescriptor(new BLE2902());
  
  BLEDescriptor descriptor(DESCRIPTOR_UUID);
  descriptor.setValue(descriptorValue);
  characteristic->addDescriptor(&descriptor);

  switch (type) {
    case 0:
      this->_flowCharacteristic = characteristic;
      break;
    case 1:
      this->_volumeCharacteristic = characteristic;
      break;
    default:
      this->_pressureCharacteristic = characteristic;
      break;
  }

  return service;
}

void ServerBLE::setSensorValue(byte sensor, uint16_t value) {
  switch (sensor) {
    case 0:
      this->_flowCharacteristic->setValue(value);
      this->_flowCharacteristic->notify();
      break;
    case 1:
      this->_volumeCharacteristic->setValue(value);
      this->_volumeCharacteristic->notify();
      break;
    default:
      this->_pressureCharacteristic->setValue(value);
      this->_pressureCharacteristic->notify();
      break;
  }
}
