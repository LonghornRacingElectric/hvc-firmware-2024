//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_COOLING_H
#define HVC_FIRMWARE_2024_COOLING_H

#include <stdbool.h>

static float minAllowedTemp = 0.0f;
static float maxAllowedTemp = 60.0f;

void updateParameters(float newMinTemp, float newMaxTemp);
bool isTempWithinBounds();

/**
 * check temps, update fan setpoints
 */
void thermalPeriodic();

#endif //HVC_FIRMWARE_2024_COOLING_H
