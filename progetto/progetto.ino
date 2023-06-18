#include <Arduino.h>
#include "wifisupport.h"

#include <Servo.h>
#include <ESP8266WiFi.h>

WiFiServer server(80);

//Stores the header of the HTTP request
String header;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
unsigned long timeoutTime = 2000;

#define servoPin D4
#define echoPin1 D7
#define trigPin1 D8 
#define echoPin2 D2
#define trigPin2 D3
#define ledPin1 D6
#define ledPin2 D5

WiFiSupport wifisupport;

Servo servo;
int angle = 0;
long duration, distance;
boolean turn = false;

void setupPin(){
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  servo.attach(servoPin);
  servo.write(angle);
  servo.write(angle);
}
 
void setup(){
  Serial.begin(9600);
  setupPin();
  wifisupport.scan();
  wifisupport.connectToAp(); 
  server.begin();
}
 
void loop(){
  // clears the trigPin condition
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(1);

  WiFiClient client = server.available(); //Listen for incoming clients

  if(client){
    Serial.println("\n\n");
    Serial.println("New client connected");
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = ""; 
    while(client.connected() && currentTime - previousTime <= timeoutTime){
      currentTime = millis();
      if(client.available()){
        char c = client.read();
        Serial.write(c);
        header += c;
        if(c == '\n'){
          if(currentLine.length() == 0){
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println(); 
            int receivedValueInt = 0;
            if(header.indexOf("GET /set?angle=") >= 0){
              String receivedValueString;
              int pos1 = header.indexOf('=');
              int pos2 = header.indexOf('HTTP');
              receivedValueString = header.substring(pos1+1,pos2-4);
              receivedValueInt = receivedValueString.toInt();
              if(receivedValueInt < 0)
                receivedValueInt = -receivedValueInt;
              if(receivedValueInt <= 180){
                angle = receivedValueInt;
              }else if(receivedValueInt == 360){
                turn = true;
                angle = 0;
              }else if(receivedValueInt > 180){
                turn = true;
                receivedValueInt = receivedValueInt % 180;
                angle = receivedValueInt;
              }
              Serial.print("Change angle from web to : ");
              Serial.println(angle);
              servo.write(angle);
              delay(1000);
              captureDistance();
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            // Then, the following line makes the web page responsive in any web browser.
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            // The next one is used to prevent requests related to the favicon – You don’t need to worry about this line.
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            client.println(getHtmlStyle());
            client.println("</head>");

            // Web Page Heading
            client.println("<body><h1><a href=\"/\">IoT Web Server</a></h1>");

            //paragraph to display the distance.
            
            client.println("<p> <form action=\"/set\"> <p> Set angle value (0:360) : </p><input type=\"text\" name=\"angle\"> <input type=\"submit\" value=\"Submit\"> </form> </p>\n");
            client.print("<p> Distance from ");
            client.print(receivedValueInt);
            client.print(" degrees angle is : ");
            client.print(distance);
            client.println(" [cm] </p>\n");
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }else
            currentLine = "";
        }else if(c != '\r')
          currentLine += c;
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected\n");
  }
  //per evitare di farlo impallare
  servo.write(angle);
}

void captureDistance(){
 if(!turn){
   Serial.println("STO USANDO IL SENSORE DAVANTI");
   digitalWrite(ledPin2, LOW);
   digitalWrite(ledPin1, HIGH);
   digitalWrite(trigPin1, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin1, LOW);   
   duration = pulseIn(echoPin1, HIGH);
 }else{
   Serial.println("STO USANDO IL SENSORE DIETRO");
   digitalWrite(ledPin1, LOW);
   digitalWrite(ledPin2, HIGH);
   digitalWrite(trigPin2, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin2, LOW);   
   duration = pulseIn(echoPin2, HIGH); 
   turn = false;  
 }
 //Calcolo la distanza (in cm) in base la velocità del suono (343 m/s)
 distance = duration * 0.034/2;
 Serial.print("Distanza dall'ostacolo : ");
 Serial.println(distance);
}

String getHtmlStyle(){
  String ptr = "";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{background-image: url('');background-color:#cccccc;margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p{font-weight: bold;}  h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +="</style>\n";
  return ptr;
}
