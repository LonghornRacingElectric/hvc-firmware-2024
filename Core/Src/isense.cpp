//
// Created by rolandwang on 11/12/2023.
// Developed by angelasrsh on 1/8/24.
//

#include "isense.h"
#include "adc.h"
//checks if current is within max to open shutdown in case if curr too high
bool isPackCurrentWithinBounds() {
    float maxCurrSetting; //TODO: Add this in when we do testing

    if (getPackCurrent() > maxCurrSetting) {
        return false;
    }
    return true;
}
float calculateHallCurrent(float hv_voltage_reading, float sensitivity) {
    // current is measured by I = ((5/(supply voltage = Uc)) * Vout - (Voffset = 2.5)) * 1/(sensitivity = 6.67)
    //TODO check if we have a reading on supply voltage to enter in this formula
    float hv_supply; // bottom 5 in the eq
    return (float) ((5.0f/5.0f) * hv_voltage_reading - 2.5f) * (1.0f/(sensitivity));
}

//Returns the current rating measured directly (mostly HighCurrVal)
//±50 A for channel 1
//±300 A for channel 2
float getPackCurrent() {
    // current is measured by I = ((5/(supply voltage = Uc)) * Vout - Voffset) * 1/(sensitivity = 6.67)
    float lowCurr = calculateHallCurrent(getISenseLow(), 40.0f);
    float highCurr = calculateHallCurrent(getISenseHigh(), 6.67f);

    if (highCurr >= 50.0f || highCurr <= -50.0f) { // most used cuz HV outputs higher than 50A around 300A
        return highCurr;
    } else return lowCurr;
}

