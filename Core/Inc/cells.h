//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_CELLS_H
#define HVC_FIRMWARE_2024_CELLS_H

#include <cstdint>

// Variables

static uint8_t voltageData[340];  // 5 segments, 28 cells per segment
                                  // 14 per LTC6813 chip, 2 bytes per voltage, volts in mV
                                  // 280 bytes for voltages, 60 bytes buffer for storing

static float tempData[91]; // 65% of cells are covered
static float minTemp;
static float maxTemp;

// Functions
void cellsInit();
void cellsPeriodic();
static float receiveSPIData();
static void sendTempPacket(); // 12 packets, 1 byte per temp (int)
static void sendVoltagePacket(); // 35 packets, 2 bytes per voltage (int)
bool areCellVoltagesWithinBounds();

float getSoC();
float getMaxTemp();
float getMinTemp();

#endif //HVC_FIRMWARE_2024_CELLS_H
