#ifdef FAKE_ARDUINO
  #ifndef FAKE_ARDUINO_H
    #define FAKE_ARDUINO_H 1
    #include <ArduinoFake.h>
    #include <stdint.h>
  #endif
#else
  #include <Arduino.h>
#endif