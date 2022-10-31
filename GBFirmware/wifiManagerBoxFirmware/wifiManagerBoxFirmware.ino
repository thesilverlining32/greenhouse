#include <ESP8266WiFi.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager​
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bmeIntern;
Adafruit_BME280 bmeExtern;

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
         
uint8_t Rpin_1 = D8;            // pin assignments; Digital pin 8
uint8_t Gpin_1 = D7;            
uint8_t Bpin_1 = 3;             //just 3 - directly assigns pin 3 of the Node MCU
uint8_t Rpin_2 = D5;            // pin assignments
uint8_t Gpin_2 = D4;
uint8_t Bpin_2 = D6;
uint8_t Ex_Fpin_1 = D3;

int R_1=255, G_1=255, B_1=255;        //upper RGB
int R_2=255, G_2=255, B_2=255;        //lower RGB
int Ex_F_1=100;

int R_1_Hold=255, G_1_Hold=255, B_1_Hold=255;        //upper RGB
int R_2_Hold=255, G_2_Hold=255, B_2_Hold=255;        //lower RGB
int Ex_F_1_Hold=100;
//exhaust fan1

float topOnTime = 18*60;          //measured in minutes: (18 hours * 60 minutes per hour) = 1080 minutes
float topOffTime = 6*60;

float botOnTime = 18*60;          //measured in minutes:
float botOffTime = 6*60;

bool topOn = false;             //tells us that the top box timer is on
bool botOn = false;

bool topTimerOn = false;        //tells us that we are counting down onTime or offTime
bool botTimerOn = false;

unsigned long currentMillis = 0;
unsigned long previousTopMillis = 0;
unsigned long previousBotMillis = 0;

String html_root;

void handleRoot();              // function prototypes for HTTP handlers
void handleInput();
void handleNotFound();

void setup(void){
    
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
  WiFi.hostname("GrowBox1");
  
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  wifiManager.autoConnect("Grow Box WiFi Manager");
  Serial.println("connected :)");

  pinMode(Rpin_1, OUTPUT);      // Set pins to output
  pinMode(Gpin_1, OUTPUT);
  pinMode(Bpin_1, OUTPUT);

  pinMode(Rpin_2, OUTPUT);     
  pinMode(Gpin_2, OUTPUT);
  pinMode(Bpin_2, OUTPUT);

  pinMode(Ex_Fpin_1, OUTPUT);   

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
  
  if (!bmeIntern.begin(0x77)) {
    Serial.println("Could not find a internal BME280 sensor, check wiring!");
  }

  if (!bmeExtern.begin(0x76)) {
    Serial.println("Could not find a external BME280 sensor, check wiring!");
  }
  
}

void loop(void){
  currentMillis = millis();
  
  analogWrite(Rpin_1, map(R_1, 0, 255, 1023, 0));  //set our lights pwm outputs top box
  analogWrite(Gpin_1, map(G_1, 0, 255, 1023, 0));
  analogWrite(Bpin_1, map(B_1, 0, 255, 1023, 0));
  
  analogWrite(Rpin_2, map(R_2, 0, 255, 1023, 0));  //set our lights pwm outputs bottom box
  analogWrite(Gpin_2, map(G_2, 0, 255, 1023, 0));
  analogWrite(Bpin_2, map(B_2, 0, 255, 1023, 0));

  analogWrite(Ex_Fpin_1, map(Ex_F_1, 0, 100, 1023, 0));  //set our fan pwm

  topBoxTimer();
  bottomBoxTimer();
  
  server.handleClient();                     // Listen for HTTP requests from clients
  
}

void topBoxTimer(){
  if(topOn){
    if (topTimerOn){
      if (currentMillis - previousTopMillis >= (topOnTime*60000)) {
        
        previousTopMillis = currentMillis;
        
        R_1 = 0;//set new pwm for upper box
        G_1 = 0;
        B_1 = 0;
        
        topTimerOn = false;
      }
    }
    else {// i.e. if topTimerOff
      if (currentMillis - previousTopMillis >= (topOffTime*60000)) {
        
        previousTopMillis = currentMillis;
        
        R_1 = R_1_Hold;//set new pwm for upper box
        G_1 = G_1_Hold;
        B_1 = B_1_Hold;
        
        topTimerOn = true; 
      }
    }
  }
}

void bottomBoxTimer(){
  if(botOn){
    if (botTimerOn){
      if (currentMillis - previousBotMillis >= botOnTime*60000) {
        
          previousBotMillis = currentMillis;
          
          R_2 = 0;//set turn off lower box lights
          G_2 = 0;
          B_2 = 0;
          
          botTimerOn = false;
       }
    }
    else {  // i.e. if bottomTimerOff
       if (currentMillis - previousBotMillis >= botOffTime*60000) {
  
          previousBotMillis = currentMillis;
  
          R_2 = R_2_Hold;//set new pwm for lower box
          G_2 = G_2_Hold;
          B_2 = B_2_Hold;
          
          botTimerOn = true; 
       }
     }      
  }
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
                + "|Ex_F_1:" + String(Ex_F_1)
                + "|topOnTime:" + String(topOnTime) 
                + "|topOffTime:" + String(topOffTime) 
                + "|botOnTime:" + String(botOnTime)
                + "|botOffTime:" + String(botOffTime) 
                + "|topOn:" + String(topOn) 
                + "|botOn:" + String(botOn)
                + "|topTimerOn:" + String(topTimerOn)
                + "|botTimerOn:" + String(botTimerOn));
                              
  server.send(200, "text/html", html_root);
}

void handleInput() { // If a POST request is made to URI /input
  //check to see if there is the correct post data
  if(  server.hasArg("R_1") && server.hasArg("G_1") && server.hasArg("B_1")
    && server.hasArg("R_2") && server.hasArg("G_2") && server.hasArg("B_2")
    && server.hasArg("Ex_F_1") && server.hasArg("topOnTime")&& server.hasArg("topOffTime")
    && server.hasArg("topOn") && server.hasArg("topTimerOn"))
    { 
      R_1 = R_1_Hold = server.arg("R_1").toInt();//set new pwm for upper box
      G_1 = G_1_Hold = server.arg("G_1").toInt();
      B_1 = B_1_Hold = server.arg("B_1").toInt();
  
      R_2 = R_2_Hold = server.arg("R_2").toInt();//set new pwm for lower box
      G_2 = G_2_Hold = server.arg("G_2").toInt();
      B_2 = B_2_Hold = server.arg("B_2").toInt();

      //timer assignment
      topOnTime = server.arg("topOnTime").toFloat()*60;         //input in hours
      topOffTime = server.arg("topOffTime").toFloat()*60;
      
      botOnTime = server.arg("botOnTime").toFloat()*60;          //measured in minutes:
      botOffTime = server.arg("botOffTime").toFloat()*60;

      topOn = server.arg("topOn");          //tells us that the top box timer is on
      botOn = server.arg("botOn");
      
      topTimerOn = server.arg("topTimerOn");        //tells us that we are counting down onTime or offTime
      botTimerOn = server.arg("botTimerOn");
  
      Ex_F_1 = Ex_F_1_Hold = server.arg("Ex_F_1").toInt();//set new pwm for exhaust fan
  
      //prep our html response
      html_root = String("intTemp:" + String(bmeIntern.readTemperature()) 
                + "|intPressure:" + String(bmeIntern.readPressure()/ 100.0F) 
                + "|intHumidity:" + String(bmeIntern.readHumidity())
                + "|extTemp:" + String(bmeExtern.readTemperature()) 
                + "|extPressure:" + String(bmeExtern.readPressure()/ 100.0F) 
                + "|extHumidity:" + String(bmeExtern.readHumidity())
                + "|uppR:" + String(R_1) + "|uppG:" + String(G_1) + "|uppB:" + String(B_1)
                + "|lowR:" + String(R_2) + "|lowG:" + String(G_2) + "|lowB:" + String(B_2)
                + "|Ex_F_1:" + String(Ex_F_1)
                + "|topOnTime:" + String(topOnTime) 
                + "|topOffTime:" + String(topOffTime) 
                + "|botOnTime:" + String(botOnTime)
                + "|botOffTime:" + String(botOffTime) 
                + "|topOn:" + String(topOn) 
                + "|botOn:" + String(botOn)
                + "|topTimerOn:" + String(topTimerOn)
                + "|botTimerOn:" + String(botTimerOn));
                  
      server.send(200, "text/html", html_root);
      
    }else{
      server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    } 

    return;
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
