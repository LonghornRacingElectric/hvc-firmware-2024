
#include "thermal.h"
#include "cells.h"
#include "fan.h"

void updateParameters(float newMinTemp, float newMaxTemp) {
    minAllowedTemp = newMinTemp;
    maxAllowedTemp = newMaxTemp;
}

bool isTempWithinBounds() {
    return getMinTemp() >= minAllowedTemp && getMaxTemp() <= maxAllowedTemp;
}

void thermalPeriodic() {
    setFanRpm();
}