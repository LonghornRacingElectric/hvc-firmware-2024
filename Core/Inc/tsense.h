//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_TSENSE_H
#define HVC_FIRMWARE_2024_TSENSE_H

#define SPI_TIMEOUT 1000 // Timeout

#include <cstdint>

uint8_t thermistorData[2];

void tsenseInit();

void tsensePeriodic();

#endif //HVC_FIRMWARE_2024_TSENSE_H
