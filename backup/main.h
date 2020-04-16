#ifndef main_h
#define main_h

#include <Arduino.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

typedef struct
{
  const char *wifiSsid;
  const char *wifiPass;
  const char *appHost;
  uint16_t gpioStatusInput;
  uint16_t gpioStatusOutput;
}manufData_t;

void setup();
void loop();

void iotSenseInit();
void iotSenseWork();
void iotSenseProcessRequest();
void blinkSample();

#endif