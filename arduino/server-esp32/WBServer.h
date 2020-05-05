/*
  WBServer.h - Library for ESP8266 web server
  Created by Catalin Rizea, May 1, 2020
  Released into the public domain 
*/
#ifndef WBServer_h
#define WBServer_h

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino_JSON.h>

class WBServer {
  public:
    WBServer();
    String start();
    void handleClient();
    int (&getSettings())[4] {
      return _settings;
    }
  private:
    void handleRoot();
    void handleNotFound();
    WebServer _server;
    IPAddress _ip;
    IPAddress _gateway;
    IPAddress _subnet;
    int _settings[4];
};

#endif
