//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_CELLS_H
#define HVC_FIRMWARE_2024_CELLS_H

// Variables
static float voltageData[140]; // 5 segments, 28 cells per segment
static float tempData[91]; // 65% of cells are covered
static float minTemp;
static float maxTemp;


// Functions
void cellsPeriodic(); // creating spi request, store data, choose when to send temp & voltage packets

static void sendTempPackets(); // 12 packets, 1 byte per temp (int)
static void sendVoltagePackets(); // 35 packets, 2 bytes per voltage (int)

#endif //HVC_FIRMWARE_2024_CELLS_H
