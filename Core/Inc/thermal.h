//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_COOLING_H
#define HVC_FIRMWARE_2024_COOLING_H

static float minTemp = 0.0f;
static float maxTemp = 60.0f;

void updateParameters(float newMinTemp, float newMaxTemp);
bool isTempWithinBounds();

/**
 * check temps, update fan setpoints
 */
void thermalPeriodic();

#endif //HVC_FIRMWARE_2024_COOLING_H
