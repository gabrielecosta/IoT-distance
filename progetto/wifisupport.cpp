#include "Arduino.h"
#include "wifisupport.h"

#ifndef STASSID
#define STASSID "TIM-76480513"
#define STAPSK  "FrAk8SJLHmmcySi5"
#endif

WiFiSupport::WiFiSupport(){}

void WiFiSupport::scan(){
  Serial.println("Scanning connections");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if(n == 0)
    Serial.println("no networks found");
  else{
    Serial.print(n);
    Serial.println(" networks found");
    for(int i=0;i<n;++i){
      // Print SSID and RSSI for each network found
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ?  "" : "*");
      delay(10);
    }
  }
  Serial.println();
  delay(3000);
}

void WiFiSupport::connectToAp(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID,STAPSK);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(" .... ");
    Serial.print(WiFi.status());
  }
  Serial.println();
  Serial.print("Connected, IP Address: ");
  Serial.println(WiFi.localIP());
}
