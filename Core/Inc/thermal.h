//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_COOLING_H
#define HVC_FIRMWARE_2024_COOLING_H

float minTemp = 0.0f;
float maxTemp = 60.0f;

void updateParameters();
bool isTempWithinBounds();

#endif //HVC_FIRMWARE_2024_COOLING_H
