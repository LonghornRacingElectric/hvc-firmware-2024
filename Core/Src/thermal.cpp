//
// Created by rolandwang on 11/12/2023.
//

#include "thermal.h"

void updateParameters(float newMinTemp, float newMaxTemp) {
    minTemp = newMinTemp;
    maxTemp = newMaxTemp;
}

bool isTempWithinBounds() {
    float temp = 15.0f;
    return temp >= minTemp && temp <= maxTemp;
}