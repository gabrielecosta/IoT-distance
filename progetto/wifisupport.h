#ifndef WiFiSupport_h
#define WiFiSupport_h

#include "Arduino.h"
#include "ESP8266WiFi.h"

class WiFiSupport{
  public:
    WiFiSupport();
    void scan();
    void connectToAp();
};

#endif
