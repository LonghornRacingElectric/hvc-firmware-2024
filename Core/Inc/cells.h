//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_CELLS_H
#define HVC_FIRMWARE_2024_CELLS_H

#include <cstdint>

// Variables

static uint8_t voltageData[340];  // 5 segments, 28 cells per segment
                                  // 14 per LTC6813 chip, 2 bytes per voltage, volts in mV
                                  // 280 bytes for voltages, 60 bytes buffer for reading
static uint8_t tempData[240];     // 9 thermistors per LTC6813 chip, 2 bytes per temp (raw gpio data)
                                  // 180 bytes for temps, 60 bytes buffer for reading
static float minTemp;
static float maxTemp;

// Functions
void cellsInit();
void cellsPeriodic();
void updateMinMaxTemps();
bool areCellVoltagesWithinBounds();
float getSoC();
float getMaxTemp();
float getMinTemp();

#endif //HVC_FIRMWARE_2024_CELLS_H
