#include <common_arduino.h>
#include <SoftwareSerial.h>
#include <Vector.h>
#include <protocol.h>
#include <command.h>

#define USBSerial Serial
SoftwareSerial BitBangSerial(12, 11);
#define BMSSerial BitBangSerial 

void write_request_start();
void write_request_end();
void handshake();
void flush();