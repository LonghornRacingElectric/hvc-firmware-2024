//
// Created by rolandwang on 11/12/2023.
//

#include "thermal.h"
#include "cells.h"

void updateParameters(float newMinTemp, float newMaxTemp) {
    minAllowedTemp = newMinTemp;
    maxAllowedTemp = newMaxTemp;
}

bool isTempWithinBounds() {
    return getMinTemp() >= minAllowedTemp && getMaxTemp() <= maxAllowedTemp;
}