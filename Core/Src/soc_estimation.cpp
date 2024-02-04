//
// Created by Angela Suresh on 1/14/24.
//



#include "isense.h"
#include "cells.h"

static float battery_rated_capacity = 40407.84f; // 6.6kWh at 588 V => 11.2244 Ah => 40407.84 Coulombs

float calculateSoc(float deltaTime) {

    float dt = deltaTime;
    float soc = 0;
    static float chargeRemaining = 0;

    float outputCurr = getPackCurrent();
    float outputVoltage = getPackVoltageFromCells();

    float currentBound = 0.002f; // can set to something else

    if (outputCurr < currentBound && outputCurr > -currentBound) {
        return getSocWithVoltage(dt, soc, chargeRemaining, outputVoltage);
    } else {
        return getSocWithCurrent(dt, soc, chargeRemaining, outputCurr);
    }

}

float getSocWithVoltage(float dt, float soc, float chargeRemaining, float outputVoltage) {
    //piecewise function

    return (float);
}

float getSocWithCurrent(float dt, float soc, float chargeRemaining, float outputCurr) {
    chargeRemaining += outputCurr * dt;
    soc =  (chargeRemaining/battery_rated_capacity) * 100.0f;

    return (float) soc;
}

