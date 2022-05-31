/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include <Scheduler.h>

#include <SoftwareSerial.h>

SoftwareSerial swSer(13, 14);

ESP8266WiFiMulti WiFiMulti;

WiFiClient client;

HTTPClient http;

const char* ssid     = "Mage_2-4G";
const char* password = "QAZwsx_123";

double upcall = 0.06;
double downcall = -0.0001;


bool isNetOK = false;

void conventData(String &str);
void callPhone();
void stopCallPhone();
void ATTest();

class serialTask : public Task {
protected:
    void loop()  {
        while (swSer.available() > 0) {
          Serial.write(swSer.read());
        }
        delay(1);
    }
} serial_task;


class mainTask : public Task {
protected:
    void loop()  {
      if(isNetOK){
           ATTest();
           delay(10);
           Serial.print("[HTTP] begin...\n");
          if (http.begin(client, "http://flkf2.hockshop.xyz/p.json")) {  // HTTP
      
      
            Serial.print("[HTTP] GET...\n");
            // start connection and send HTTP header
            int httpCode = http.GET();
      
            // httpCode will be negative on error
            if (httpCode > 0) {
              // HTTP header has been send and Server response header has been handled
              Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      
              // file found at server
              if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String payload = http.getString();
                conventData(payload);
                Serial.println(payload);
              }
            } else {
              Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
      
            http.end();
          } else {
            Serial.printf("[HTTP} Unable to connect\n");
          }
        }
        delay(60000);
        
    }
} main_task;



void setup() {

  Serial.begin(115200);
  swSer.begin(115200);
  // Serial.setDebugOutput(true);
  pinMode(12, OUTPUT); 
  delay(1);
  digitalWrite(12, HIGH);
  Serial.println("\nSoftware serial test started");

  delay(1);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  isNetOK = true;
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Scheduler.start(&serial_task);
  Scheduler.start(&main_task);
  Scheduler.begin();

}
//打电话
#define CALLPHONECMD "ATD13590289064;\r\n"
#define STOPPHONECMD "ATH\r\n"
#define AT_CMD "AT\r\n"

void ATTest(){
  Serial.print("send 4G Mode AT\n");
  swSer.write(AT_CMD);
}

void callPhone(){
  Serial.print("callPhone\n");
  swSer.write(CALLPHONECMD);
}

void stopCallPhone(){
  Serial.print("stop callPhone\n");
  swSer.write(STOPPHONECMD);
}

void conventData(String &str){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(str);
  if (root.success()) {
    if (root.containsKey("erro")) {
        int erro = root["erro"].as<int>();
        if(erro !=0){
          callPhone();
          Serial.print("server erro\n");
          return;
        }
// {
//   "percent": 0.0101,
//   "price": 29492.5,
//   "cjprice": 29790.1,
//   "cjdate": 220930,
//   "erro": 0,
//   "zeng": 1
// }
        double percent = root["percent"].as<double>();
        double price = root["price"].as<double>();
        double cjprice = root["cjprice"].as<double>();
        String cjdate = root["cjdate"].as<String>();
        int zeng = root["zeng"].as<int>();
        Serial.printf("percent=%.5f,price=%.2f,cjprice=%.2f,cjdate=%s,zeng=%d\n",percent,price,cjprice,cjdate.c_str(),zeng);
        //价格差价到指定值了,打电话提示
        if(percent >= upcall || percent <= downcall){
          callPhone();
        }
    }
  }
}

void loop() {}
