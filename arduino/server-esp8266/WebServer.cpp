/*
  WebServer.h - Library for ESP8266 web server
  Created by Catalin Rizea, May 1, 2020
  Released into the public domain 
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Arduino_JSON.h>

#include "WebServer.h"
#include "secret.h"

WebServer::WebServer(): _server(80), _ip(192, 168, 1, 110), _gateway(192, 168, 1, 1), _subnet(255, 255, 255, 0) {
  this->getSettings()[0] = 0;
  this->getSettings()[1] = 0;
  this->getSettings()[2] = 0;
  this->getSettings()[3] = 0;
}

void WebServer::handleRoot() {
  if (this->_server.method() == HTTP_GET) {
    Serial.println("WebServer handleRoot GET called");
    JSONVar status;
    
    status["ip"] = WiFi.localIP().toString();
    status["mac"] = WiFi.macAddress();
    status["status"] = "OK";

    String jsonString = JSON.stringify(status);
    this->_server.send(200, "application/json", jsonString);
  }

  if (this->_server.method() == HTTP_POST) {
    Serial.println("WebServer handleRoot POST called");    
    JSONVar newSettings = JSON.parse(this->_server.arg("plain"));

    if (JSON.typeof(newSettings) == "undefined") {
      return;
    }

    if (newSettings.hasOwnProperty("volume")) {
      this->getSettings()[0] = int(newSettings["volume"]);
    }

    if (newSettings.hasOwnProperty("bpm")) {
      this->getSettings()[1] = int(newSettings["bpm"]);
    }

    if (newSettings.hasOwnProperty("inspiration")) {
      this->getSettings()[2] = int(newSettings["inspiration"]);
    }

    if (newSettings.hasOwnProperty("expiration")) {
      this->getSettings()[4] = int(newSettings["expiration"]);
    }
    
    JSONVar status;
    status["status"] = "OK";
    String jsonString = JSON.stringify(status);
    
    this->_server.send(200, "application/json", jsonString);
  }

  if (this->_server.method() != HTTP_GET && this->_server.method() != HTTP_POST) {
    Serial.println("WebServer handleRoot notAllowed called");
    this->_server.send(405, "text/plain", "Not Allowed");
  }
}

void WebServer::handleNotFound() {
  Serial.println("WebServer handleNotFound called");
  this->_server.send(404, "text/plain", "Not found");
}

String WebServer::start() {
  WiFi.begin(ssidSecret, passSecret);
  WiFi.config(this->_ip, this->_gateway, this->_subnet);

  String message;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  message = message + "\nConnected to " + String(ssidSecret) + "\nIP address: " + this->_ip.toString() + "\n";

  if (MDNS.begin("esp8266")) {
    message = message + "MDNS responder started\n";
  }

  this->_server.on("/", std::bind(&WebServer::handleRoot, this));
  this->_server.onNotFound(std::bind(&WebServer::handleNotFound, this));

  this->_server.begin();
  message = message + "HTTP server started";
  
  return message;
}

void WebServer::handleClient() {
  this->_server.handleClient();
}
