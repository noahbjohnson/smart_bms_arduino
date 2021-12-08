#include <common_arduino.h>
#include <protocol.h>
#include <Vector.h>

struct Generated_Command
{
    uint8_t data[100];
    uint8_t length;
};


class Command{
  private:
    uint8_t read_or_write;
    uint8_t data_length;
    uint8_t register_address;

    // The message checksum is really 16-bits but we buffer it as two bytes
    uint8_t checksum_high; // first checksum byte
    uint8_t checksum_low; // second checksum byte
    
    uint8_t data_stack[10]; // The payload data
    
    // Get the checksum value for the message
    void update_checksum() {
      uint8_t vectorSum = register_address;
      for (unsigned int i = 0; i < data_length; i++) {
        vectorSum += data_stack[i];
      }
      // The checksum is simply sum of the payload byte values subtracted from 0x10000 (65536).
      uint16_t checksum = 0x10000 - vectorSum;
      checksum_high=(checksum>>8) & 0xff;
      checksum_low=checksum & 0xff;
    }

    // Get the total number of bytes the message will be
    uint8_t get_message_length() {
      // broke this up here for readability
      uint8_t message_length = 0; // running total
      message_length ++; // Start byte
      message_length ++; // Read or write
      message_length ++; // Payload length
      message_length += data_length; // Payload
      message_length +=2; // Checksum
      message_length ++; // End byte
      return message_length;
    }

  public:
    Command(uint8_t read_write = READ){
      read_or_write = read_write;
    }
    
    void setRegister(uint8_t address) {
      register_address = address;
      update_checksum();
    }

    // Manual recursion because I can
    void setData() {
      for (unsigned int i = 0; i < 10; i++) {
        data_stack[i] = 0;
      }
      data_length = 0;
    }

    void setData(uint8_t b0) {
      setData();
      data_length = 1;
      data_stack[0] = b0;
    }

    void setData(uint8_t b0, uint8_t b1) {
      setData(b0);
      data_length = 2;
      data_stack[1] = b1;
    }

    void setData(uint8_t b0, uint8_t b1, uint8_t b2) {
      setData(b0, b1);
      data_length = 3;
      data_stack[2] = b2;
    }

    void setData(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
      setData(b0, b1, b2);
      data_length = 4;
      data_stack[3] = b3;
    }

    void setData(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
      setData(b0, b1, b2, b3);
      data_length = 5;
      data_stack[4] = b4;
    }

    // return the complete message vector
    Generated_Command get_command (){
      // Construct message
      Generated_Command new_command;
      new_command.data[0] = START;
      new_command.data[1] = read_or_write;
      new_command.data[2] = register_address;
      new_command.data[3] = data_length;

      // add payload data to message
      uint8_t data_index = 4;
      for (unsigned int i = 0; i < data_length; i++) {
        new_command.data[data_index] = data_stack[i];
        data_index++;
      }

      // add checksum and end byte
      update_checksum();
      new_command.data[data_index] = checksum_high;
      data_index++;
      new_command.data[data_index] = checksum_low;
      data_index++;
      new_command.data[data_index] = END;
      data_index++;

      new_command.length = data_index;

      return new_command;
    }
};