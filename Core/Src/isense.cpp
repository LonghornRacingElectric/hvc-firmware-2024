//
// Created by rolandwang on 11/12/2023.
// Developed by angelasrsh on 12/3/24.
//

#include "isense.h"
#include "adc.h"



bool isPackCurrentWithinBounds() {
    float lowCurr = getA_HV_lowCurr_to_MCU(); // set value
    float highCurr = getA_HV_highCurr_to_MCU();

    if (lowCurr < -20 && lowCurr > 20) {
        return false;
    }
    if (highCurr < -500 && highCurr > 500) {
        return false;
    }
    return true;
}


//Returns the current rating measured directly
//±20 A for channel 1
//±500 A for channel 2
float getPackCurrent() {
    float lowCurr = getA_HV_lowCurr_to_MCU(); // set value
    float highCurr = getA_HV_highCurr_to_MCU();

    // return the actual value of the current
    return lowCurr; // if (we want
}

