#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Vector.h>
#include <protocol.h>

#define USBSerial Serial
SoftwareSerial BitBangSerial(12, 11);
#define BMSSerial BitBangSerial 

void setup() {
  USBSerial.begin(115200);
  BMSSerial.begin(9600);
}

void loop() {
  handshake();
}


/**
 *                                              UTILITY FUNCTIONS
 * */


// Flush the serial buffer
void flush()
{
  delay(100);
  while (BMSSerial.available() > 0)
  { BMSSerial.read();
  }
  delay(50);
}

// connect and disconect twice
void handshake() {
  write_request_start();
  write_request_end();
  write_request_start();
  write_request_end();
}

/**
 *                                              COMMANDS
 * */

// Enter factory mode
void write_request_start()
{
  flush();

  uint8_t data[9] = {START, WRITE, ENTER_FACTORY_MODE, 0x02, 0x56, 0x78, 0xFF, 0x30, END};
  BMSSerial.write(data, 9);
}

// Exit factory mode
void write_request_end()
{
  flush();

  uint8_t data[9] = {START, WRITE, EXIT_FACTORY_MODE, 0x02, 0x00, 0x00, 0xFF, 0xFD, END};
  BMSSerial.write(data, 9);
}

