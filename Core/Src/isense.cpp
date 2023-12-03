//
// Created by rolandwang on 11/12/2023.
// Developed by angelasrsh on 12/3/24.
//

#include "isense.h"
#include "adc.h"

//checks if current is within max to open shutdown in case if curr too high
bool isPackCurrentWithinBounds() {
    float lowCurr = getA_HV_lowCurr_to_MCU(); // set value
    float highCurr = getA_HV_highCurr_to_MCU();
    float maxCurrSetting; //TODO: Add this in when we figure val out

    if (getPackCurrent() > maxCurrSetting) {
        return false;
    }
    return true;
}

//Returns the current rating measured directly (mostly HighCurrVal)
//±20 A for channel 1
//±500 A for channel 2
float getPackCurrent() {
    float lowCurr = getA_HV_lowCurr_to_MCU();
    float highCurr = getA_HV_highCurr_to_MCU();

    if (highCurr <= lowCurr) {
        return lowCurr;
    } else return highCurr;
}

