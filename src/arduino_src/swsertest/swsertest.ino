
#include <SoftwareSerial.h>

SoftwareSerial swSer(13, 14);

void setup() {
  Serial.begin(115200);
  swSer.begin(115200);


  pinMode(12, OUTPUT); 
  delay(1);
  digitalWrite(12, HIGH);
  Serial.println("\nSoftware serial test started");

  delay(1);

}

void loop() {
  while (swSer.available() > 0) {
    Serial.write(swSer.read());
  }
  delay(1);
  while (Serial.available() > 0) {
    swSer.write(Serial.read());
  }
  delay(1);
}
