#include <common_arduino.h>
#include <protocol.h>
#include <Vector.h>

class Command{
  private:
    uint8_t read_or_write;
    uint8_t data_length;
    uint8_t register_address;

    // The message checksum is really 16-bits but we buffer it as two bytes
    uint8_t checksum_high; // first checksum byte
    uint8_t checksum_low; // second checksum byte
    
    uint8_t data_stack[10]; // The payload data
    Vector<uint8_t> command_buffer; // The complete message
    
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

    // Generate the command and store it in the command buffer
    void generateCommand(){
      // Construct message
      uint8_t message[get_message_length()];
      message[0] = START;
      message[1] = read_or_write;
      message[2] = register_address;
      message[3] = data_length;

      // add payload data to message
      uint8_t data_index = 4;
      for (unsigned int i = 0; i < data_length; i++) {
        message[data_index] = data_stack[i];
        data_index++;
      }

      // add checksum and end byte
      update_checksum();
      message[data_index + 1] = checksum_high;
      message[data_index + 2] = checksum_low;
      message[data_index + 3] = END;

      Vector<uint8_t> message_vector;
      message_vector.setStorage(message, get_message_length(), get_message_length());
      command_buffer = message_vector;
    }

  public:
    Command(uint8_t read_write = READ){
      // init storage for vector
      uint8_t storage_array[5];
      #ifdef Arduino_h
        data_stack.setStorage(storage_array);
      #endif

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
    Vector<uint8_t> get_buffer (){
      generateCommand();
      return command_buffer;
    }
};