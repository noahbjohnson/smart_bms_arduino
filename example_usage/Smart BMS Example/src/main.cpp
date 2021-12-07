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
  if (0xDD == 221) {
    Serial.println("ok");
  }
  delay(1000);
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

class command{
  private:
    uint8_t read_or_write;
    Vector<uint8_t> data_stack;
    uint8_t data_length;
    uint8_t data_checksum;
    
    void updateChecksum() {
      uint8_t vectorSum = 0;
      for (unsigned int i = 0; i < data_stack.size(); i++) {
        vectorSum += data_stack.at(i);
      }
      // The checksum is simply sum of the payload byte values subtracted from 0x10000 (65536).
      data_checksum = 0x10000 - vectorSum;
    }

  command(uint8_t read_write = READ){
    // init storage for vector
    uint8_t storage_array[5];
    data_stack.setStorage(storage_array);

    read_or_write = read_write;
  }

  void setData() {
    data_stack.clear();

    data_length = 0;

    updateChecksum();
  }

  void setData(uint8_t byte1) {
    data_stack.clear();

    data_length = 1;

    data_stack.push_back(byte1);

    updateChecksum();
  }

  void setData(uint8_t byte1, uint8_t byte2) {
    data_stack.clear();

    data_length = 2;

    data_stack.push_back(byte2);
    data_stack.push_back(byte1);

    updateChecksum();
  }

  void setData(uint8_t byte1, uint8_t byte2, uint8_t byte3) {
    data_stack.clear();

    data_length = 3;

    data_stack.push_back(byte3);
    data_stack.push_back(byte2);
    data_stack.push_back(byte1);

    updateChecksum();
  }

  void setData(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) {
    data_stack.clear();

    data_length = 4;

    data_stack.push_back(byte4);
    data_stack.push_back(byte3);
    data_stack.push_back(byte2);
    data_stack.push_back(byte1);

    updateChecksum();
  }


};