//
// Created by Angela Suresh on 1/14/24.
//



#include "isense.h"

float outputCurr = getPackCurrent();
float battery_rated_capacity = 11.2244; // 6.6kWh at 588 V => 11.2244 Ah

float calculateSoc(float deltaTime) {
    float step_size = 0.003f;
    float dt = deltaTime/step_size;
    float soc = 0;
    float charge = 0;

    charge += outputCurr * dt;
    soc = 100.0f + (-charge/battery_rated_capacity) * 100.0f;

    return soc;
}

