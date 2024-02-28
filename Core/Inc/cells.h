//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_CELLS_H
#define HVC_FIRMWARE_2024_CELLS_H

#include <cstdint>

// Variables
static uint8_t rawData[60];       // Raw data from LTC read command
static float voltageData[140]; // 5 segments, 28 cells per segment, 4 per LTC6813 chip, 2 bytes per voltage
static float tempData[90];     // 9 thermistors per LTC6813 chip, 2 bytes per temp
static float Voc = 500.0f;        // mV
static float Tc = 10.0f;          // mV/C
static float currentMinTemp = 999.0f;
static float currentMaxTemp = -999.0f;
static float minAllowedTemp = 0.0f;
static float maxAllowedTemp = 60.0f;
static float minTemp;
static float maxTemp;
static float packVoltage;
static int numCells = 140;


// Functions
void cellsInit();
void cellsPeriodic();
void checkMinMaxTemps(float temp);
bool areCellVoltagesWithinBounds();
bool isPackVoltageWithinBounds();
bool isTempWithinBounds();
float getPackVoltageFromCells();
float getSoC();
float getMaxTemp();
float getMinTemp();
float convertTemp(uint16_t Vout);
float convertVoltage(uint16_t voltage);
void updateTempParameters(float newMinTemp, float newMaxTemp);

#endif //HVC_FIRMWARE_2024_CELLS_H
