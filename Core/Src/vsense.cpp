//
// Created by rolandwang on 11/12/2023.
//

isense
#include "../Inc/vsense.h"
#include "adc.h"
float getPackVoltage() {
    //insert calculations needed
    float VSenseVoltage = getA_HV_to_MCU();
    return VSenseVoltage;
}

