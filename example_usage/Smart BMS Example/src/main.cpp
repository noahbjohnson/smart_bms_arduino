#include <Arduino.h>
#include <SoftwareSerial.h>

#define USBSerial Serial
SoftwareSerial BitBangSerial(12, 11);
#define BMSSerial BitBangSerial 

void setup() {
  USBSerial.begin(115200);
  BMSSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}