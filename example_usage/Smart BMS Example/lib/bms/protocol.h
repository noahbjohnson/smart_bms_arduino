#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <common_arduino.h>

// General
#define START 0xDD
#define END 0x77
#define READ 0xA5
#define WRITE 0x5A

// From bms-tools
#define CELL_CAL_REG_START 0xB0
#define CELL_CAL_REG_END 0xCF

#define NTC_CAL_REG_START 0xD0
#define NTC_CAL_REG_END 0xD7

#define I_CAL_IDLE_REG 0xAD
#define I_CAL_CHG_REG 0xAE
#define I_CAL_DSG_REG 0xAF

#define CAP_REM_REG 0xE0
#define CHG_DSG_EN_REG 0xE1
#define BAL_CTRL_REG 0xE2

// Register Addresses
#define ENTER_FACTORY_MODE 0x00
#define EXIT_FACTORY_MODE 0x01
#define READ_BASIC_INFO 0x03
#define READ_BATTERY_VOLTAGE 0x04
#define READ_HARDWARE_VERSION 0x05

// Data Length Bytes

// EEPROM

// Commands
uint8_t ENTER_FACTORY_MODE_COMMAND[2] = {0x56, 0x78};

#endif