//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_CELLS_H
#define HVC_FIRMWARE_2024_CELLS_H

#include <cstdint>

// Variables
static uint8_t rawData[60];       // Raw data from LTC read command
static uint16_t voltageData[140]; // 5 segments, 28 cells per segment, 4 per LTC6813 chip, 2 bytes per voltage
static uint16_t tempData[90];     // 9 thermistors per LTC6813 chip, 2 bytes per temp
static float currentMinTemp = -999.0f;
static float currentMaxTemp = 999.0f;
static float minTemp;
static float maxTemp;
static float packVoltage;

// Functions
void cellsInit();
void cellsPeriodic();
void checkMinMaxTemps(float temp);
bool areCellVoltagesWithinBounds();
bool isPackVoltageWithinBounds();
float getPackVoltageFromCells();
float getSoC();
float getMaxTemp();
float getMinTemp();

#endif //HVC_FIRMWARE_2024_CELLS_H
