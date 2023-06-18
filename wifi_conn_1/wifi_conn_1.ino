#include <ESP8266WiFi.h>

#define r_pin 14 //D5
#define b_pin 13 //D7
#define g_pin 12 //D6

const char* net_name = "Costa's Wifi";
const char* net_pass = "gbrl_2002";

void setup() {
  Serial.begin(115220);
  Serial.println();
  connection(net_name, net_pass);
}

void loop() {
   
}

void connection(const char* net_name, const char* net_pass) {
  WiFi.begin(net_name, net_pass);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
     if(WiFi.status() == WL_CONNECTED) {
    //colora di verde
    rgb(0,255,0);
    Serial.println("Connected!");
  } else if (WiFi.status() == WL_NO_SSID_AVAIL) {
    //colora di giallo
    rgb(255,255,0);
    Serial.println("Wrong SSID");
    break;
  } else if (WiFi.status() == WL_CONNECT_FAILED) {
    //colora di ciano
    rgb(0,255,255);
    Serial.println("Wrong password");
    break;
  } else if (WiFi.status() == WL_IDLE_STATUS) {
    //colora di magenta
    rgb(255,0,255);
  } else if (WiFi.status() == WL_DISCONNECTED) {
    //colora di rosso
    rgb(255,0,0);
    Serial.println("Disconnected");
  }
    Serial.print(".");
  }
  Serial.println();

  if(WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void rgb(int red, int green, int blue) {
  analogWrite(r_pin, red);
  analogWrite(g_pin, green);
  analogWrite(b_pin, blue);
}
