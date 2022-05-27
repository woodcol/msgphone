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

ESP8266WiFiMulti WiFiMulti;

double upcall = 0.06;
double downcall = -0.0001;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Mi 10S", "7668150Tt00");

}
//打电话
void callPhone(){
  Serial.print("callPhone\n");
}

void stopCallPhone(){
  Serial.print("stop callPhone\n");
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

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

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
