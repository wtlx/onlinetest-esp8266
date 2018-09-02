//**************************************************************//
//  Name    : Onlinetest                       
//  Author  : wtlx 
//  Date    : September, 2nd, 2018    
//  Modified:                                 
//  Version : 1.0                                             
//  Notes   : Just another apprentice piece  //
//****************************************************************//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

Servo servo;  //initialising Servo


int servoPin = D4;
int redledPin =  D8;       // Pin-Number of red LED
int redledState = LOW;     // Status of red LED-Pin
int greenledPin =  D9;     // Pin-Number of green LED
int greenledState = LOW;   // Status of green LED-Pin
int servoState = 0;        // to control if servo action is necessary
int servoPos1 = 45;        //Position 1 of the servo
int servoPos2 = 120;       //Position 1 of the servo
int pauseTime = 20000;     // How long (in ms) should we wait to test again?  
String onlineTest = "http://example_com/test.htm";  //try to get this file to test if you are connected to the internet

#define WLAN_SSID       "_YOUR_Wifi_NAME_"
#define WLAN_PASS       "_YOUR_Wifi_PASSWORD_"
WiFiClient client;         //initialise


void setup() {

servo.attach(servoPin);

servo.write(0);            //bring servo in position 0
delay(2000);              //time to move
pinMode(servoPin, INPUT); //change Servopin (to prevent shaking)

pinMode(redledPin, OUTPUT);     
pinMode(greenledPin, OUTPUT);  
  
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Hello from ESP8266!"));

  // Connect to WiFi accesspoint
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("sucessfully connected to Wifi");
}

void loop() {

        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // try to fetch a file from the intenet
        http.begin(onlineTest);


          Serial.print("[HTTP] GET...\n");
        // start connection
        int httpAnswer = http.GET();

        // if the answer is more than zero you are connected. Yay!
        if(httpAnswer > 0) {
              Serial.printf("[HTTP] Code: %d\n Internet! Yay! - ", httpAnswer);
            if(httpAnswer == HTTP_CODE_OK) {
                String content = http.getString();
                  Serial.println(content);
            }
                  redledState = LOW;  // Turn it off
                  digitalWrite(redledPin, redledState);     // Update the actual LED state (not really neded in this simple Version)
                  greenledState = HIGH;                     // turn it on
                  digitalWrite(greenledPin, greenledState); // Update the actual LED state (not really neded in this simple Version)
                  if (servoState != servoPos2) {            // read servo position from variable
                      pinMode(servoPin, OUTPUT);            // set Servopin to Output to change it
                      servo.write(servoPos2);               // change to position 2
                      delay(1000);                          // wait for the movement
                      servoState = servoPos2;               // write new servo positon to variable
                      pinMode(servoPin, INPUT);             // set servopin to Intput to prevent shaking
                  } else {}
            
        } else {
              Serial.printf("[HTTP] GET... failed: %s\n We are not connected to the Internet. Aargh!", http.errorToString(httpAnswer).c_str());
                  greenledState = LOW;                      
                  digitalWrite(greenledPin, greenledState); 
                  redledState = HIGH;                                     
                  digitalWrite(redledPin, redledState);     
                  if (servoState != servoPos1) {
                      pinMode(servoPin, OUTPUT); 
                      servo.write(servoPos1);
                      delay(1000);
                      servoState = servoPos1;
                      pinMode(servoPin, INPUT); 
                  }
              
        }

        http.end();
        delay(pauseTime);
          Serial.println("Are we still online? Let's see...");
    }


