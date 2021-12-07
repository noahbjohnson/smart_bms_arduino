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
    uint16_t data_checksum;
    uint8_t register_address;
    
    void updateChecksum() {
      uint8_t vectorSum = register_address;
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

  void sendCommand(){
    uint8_t message_length = 0;

    // Start byte
    message_length ++;

    // Read or write
    message_length ++;

    // Payload length
    message_length ++;

    // Payload
    message_length += data_length;

    // Checksum
    message_length +=2;

    // End Byte
    message_length++;

    // Construct message
    uint8_t message[message_length];
    message[0] = START;
    message[1] = read_or_write;
    message[2] = register_address;
    message[3] = data_length;

    // add payload data to message
    uint8_t data_index = 4;
    for (unsigned int i = 0; i < data_length; i++) {
      message[data_index] = data_stack.back();
      data_stack.pop_back();
      data_index++;
    }

    uint8_t checksum_one, checksum_two;
    union {
      uint16_t Word;
      uint8_t Bytes[2];
    } both;

    both.Word = data_checksum;
    checksum_one = both.Bytes[0];
    checksum_two = both.Bytes[1];

    message[data_index + 1] = checksum_one;
    message[data_index + 2] = checksum_two;
    message[data_index + 3] = END;

    BMSSerial.write(message, message_length);
  }

  void setRegister(uint8_t address) {
    register_address = address;
    updateChecksum();
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