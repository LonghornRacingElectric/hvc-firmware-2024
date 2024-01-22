//
// Created by rolandwang on 11/12/2023.
//

#include "tsense.h"
#include "spi.h"

void tsenseInit() {
    // TODO implement
}

void tsensePeriodic() {
    HAL_SPI_Receive(&hspi1, thermistorData, 1, SPI_TIMEOUT);
}

uint16_t getAmbientTemp() {
    return (uint16_t) ((thermistorData[1] << 8) || thermistorData[0]);
}