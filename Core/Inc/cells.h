//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_CELLS_H
#define HVC_FIRMWARE_2024_CELLS_H

// Variables
static float tempData[20];
static float minTemp;
static float maxTemp;

// Functions
void processTempData(); // creating spi request, storing data into array
void processVoltageData(); // creating spi request, storing data into array
void sendPackData();

#endif //HVC_FIRMWARE_2024_CELLS_H
