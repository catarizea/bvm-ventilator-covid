/*
  WebServer.h - Library for ESP8266 web server
  Creted by Catalin Rizea, May 1, 2020
  Released into the public domain 
*/
#ifndef WebServer_h
#define WebServer_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Arduino_JSON.h>

class WebServer {
  public:
    WebServer();
    String start();
    void handleClient();
    int (&getSettings())[4] {
      return _settings;
    }
  private:
    void handleRoot();
    void handleNotFound();
    ESP8266WebServer _server;
    IPAddress _ip;
    IPAddress _gateway;
    IPAddress _subnet;
    int _settings[4];
};

#endif
