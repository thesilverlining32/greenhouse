#include <ESP8266WiFi.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bmeIntern;
Adafruit_BME280 bmeExtern;

const char *ssid = "MyBox1"; // The name of the Wi-Fi network that will be created
const char *password = "thereisnospoon";   // The password required to connect to it, leave blank for an open network

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
         
uint8_t Rpin_1 = D8;            // pin assignments
uint8_t Gpin_1 = D7;
uint8_t Bpin_1 = D6;
uint8_t Rpin_2 = D5;            // pin assignments
uint8_t Gpin_2 = D4;
uint8_t Bpin_2 = D3;
uint8_t Ex_Fpin_1 = 3;

int R_1=255, G_1=255, B_1=255;        //upper RGB
int R_2=255, G_2=255, B_2=255;        //lower RGB
int Ex_F_1=255;                       //exhaust fan1

String html_root;

void handleRoot();              // function prototypes for HTTP handlers
void handleInput();
void handleNotFound();

void setup(void){
  
  pinMode(Rpin_1, OUTPUT);      // Set pins to output
  pinMode(Gpin_1, OUTPUT);
  pinMode(Bpin_1, OUTPUT);

  pinMode(Rpin_2, OUTPUT);     
  pinMode(Gpin_2, OUTPUT);
  pinMode(Bpin_2, OUTPUT);

  pinMode(Ex_Fpin_1, OUTPUT);
  
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  WiFi.softAP(ssid, password);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);        // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/input", HTTP_POST, handleInput); // Call the 'handleInput' function when a POST request is made to URI "/input"
  server.onNotFound(handleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                            // Actually start the server
  Serial.println("HTTP server started");
  
  if (!bmeIntern.begin(0x76)) {
    Serial.println("Could not find a internal BME280 sensor, check wiring!");
    while (1);
  }

  if (!bmeExtern.begin(0x77)) {
    Serial.println("Could not find a external BME280 sensor, check wiring!");
    //while (1);
  }
  
}

void loop(void){

  analogWrite(Rpin_1, R_1);  //set our lights pwm outputs top box
  analogWrite(Gpin_1, G_1);
  analogWrite(Bpin_1, B_1);
  
  analogWrite(Rpin_2, R_2);  //set our lights pwm outputs bottom box
  analogWrite(Gpin_2, G_2);
  analogWrite(Bpin_2, B_2);

  analogWrite(Ex_Fpin_1, Ex_F_1);  //set our fan pwm
  
  server.handleClient();                     // Listen for HTTP requests from clients
}

void handleRoot() {                          // When URI / is requested, send a web page with a button to toggle the LED
      
      //prep our html response
      html_root = String("intTemp:" + String(bmeIntern.readTemperature()) 
                + "|intPressure:" + String(bmeIntern.readPressure()/ 100.0F) 
                + "|intHumidity:" + String(bmeIntern.readHumidity())
                + "|extTemp:" + String(bmeExtern.readTemperature()) 
                + "|extPressure:" + String(bmeExtern.readPressure()/ 100.0F) 
                + "|extHumidity:" + String(bmeExtern.readHumidity())
                + "|uppR:" + String(R_1) + "|uppG:" + String(G_1) + "|uppB:" + String(B_1)
                + "|lowR:" + String(R_2) + "|lowG:" + String(G_2) + "|lowB:" + String(B_2)
                + "|Ex_F_1:" + String(Ex_F_1));
                              
  server.send(200, "text/html", html_root);
}

void handleInput() {                         // If a POST request is made to URI /input
  if( ! server.hasArg("R_1")&&! server.hasArg("G_1")&&! server.hasArg("B_1")
    &&! server.hasArg("R_2")&&! server.hasArg("G_2")&&! server.hasArg("B_2")
    &&! server.hasArg("Ex_F_1")) 
    { 
      server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
      return;
    }else{
      R_1 = server.arg("R_1").toInt();//set new pwm for upper box
      G_1 = server.arg("G_1").toInt();
      B_1 = server.arg("B_1").toInt();
  
      R_2 = server.arg("R_2").toInt();//set new pwm for lower box
      G_2 = server.arg("G_2").toInt();
      B_2 = server.arg("B_2").toInt();
  
      Ex_F_1 = server.arg("Ex_F_1").toInt();//set new pwm for exhaust fan
  
      //prep our html response
      html_root = String("intTemp:" + String(bmeIntern.readTemperature()) 
                + "|intPressure:" + String(bmeIntern.readPressure()/ 100.0F) 
                + "|intHumidity:" + String(bmeIntern.readHumidity())
                + "|extTemp:" + String(bmeExtern.readTemperature()) 
                + "|extPressure:" + String(bmeExtern.readPressure()/ 100.0F) 
                + "|extHumidity:" + String(bmeExtern.readHumidity())
                + "|uppR:" + String(R_1) + "|uppG:" + String(G_1) + "|uppB:" + String(B_1)
                + "|lowR:" + String(R_2) + "|lowG:" + String(G_2) + "|lowB:" + String(B_2)
                + "|Ex_F_1:" + String(Ex_F_1));
                  
      server.send(200, "text/html", html_root);
    } 
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
