/*
 * LITECOIN TICKER
 * author: Nathan Shummoogum
 * Date: 8/21/2017
 * 
 * Description: Use NodeMCU ESP8266 and OLED monitor to track
 *              and display crypto currency prices.
 */

// Including libraries
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "SSD1306.h"


// Defining pins
#define SDA D3
#define SCL D5
#define I2C 0x3C

SSD1306 display(I2C, SDA, SCL); //display function

WiFiServer server(80); //server port 80

//Defining WiFi Credentials
char ssid[] = "BELL909";
char pass[] = "babeface";

const char* host = "api.coindesk.com"; // crypto currency API


//ESP8266 SET UP
void setup() {
  // put your setup code here, to run once:

  //setup pins
  pinMode(13, OUTPUT);
  
  Serial.begin(9600);

  // Display Setup
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.display();
  
  // WiFi Connection Setup
  WiFi.disconnect();

  delay(3000);
  Serial.println("");
  Serial.println("Connect");

  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("...");
  }

  if(WiFi.status() == WL_CONNECTED) 
    Serial.println("WiFi Connection Successful");
    Serial.print("Local IP Adress: ");
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Connecting to the server...");
  WiFiClient client;

  if(!client.connect(host, 80)){
    Serial.println("connection failed");
    return;
  }
  
  String url = "/v1/bpi/currentprice.json";
  String response;
  

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  delay(100);
  
    while(client.available()){
        String line = client.readStringUntil('\r');
        response += line;
      }

    
    
  client.stop();
  Serial.println();
  Serial.println("End Connection");

  
  Serial.println();
  Serial.println("Response: ");
  //Serial.println(response);
  //Serial.println("DO WE GET HERE??");
  String jsonResponse;
  int jsonIndex;

  //Get Response
  for (int i=0; i<response.length(); i++){
    if (response[i] == '{'){
      jsonIndex = i;
      break;
    }
  }

  //Trim Response
  jsonResponse = response.substring(jsonIndex);
  Serial.println();
  //Serial.println("JSON response: ");
  //Serial.println(jsonResponse);
  jsonResponse.trim();

  
  //Get Price
  int rateIndex = jsonResponse.indexOf("rate_float");
  String priceString = jsonResponse.substring(rateIndex + 12, rateIndex + 18);
  priceString.trim();



  //Display to OLED Monitor
  display.clear();
  display.setFont(ArialMT_Plain_24);
  display.drawString(5, 0, "BTC/USD: ");
  display.drawString(26, 20, priceString);
  display.display();

  delay(5000);
    
  
}
