//
// Created by rolandwang on 11/12/2023.
// Developed by angelasrsh on 12/3/24.
//

#include "isense.h"
#include "adc.h"
//checks if current is within max to open shutdown in case if curr too high
bool isPackCurrentWithinBounds() {
    float lowCurr = getISenseLow(); // low curr
    float highCurr = getISenseHigh(); //high curr
    float maxCurrSetting; //TODO: Add this in when we do testing

    if (getPackCurrent() > maxCurrSetting) {
        return false;
    }
    return true;
}
float calculateHallCurrent(float hv_voltage_reading) {
    // current is measured by I = ((5/(supply voltage = Uc)) * Vout - (Voffset = 2.5)) * 1/(sensitivity = 6.67)
    //TODO check if we have a reading on supply voltage to enter in this formula
    float hv_supply_voltage; // bottom 5 in the eq
    return (float) ((5/5) * hv_voltage_reading - 2.5) * (1/(6.67))
}

//Returns the current rating measured directly (mostly HighCurrVal)
//±20 A for channel 1
//±500 A for channel 2
float getPackCurrent() {
    // current is measured by I = ((5/(supply voltage = Uc)) * Vout - Voffset) * 1/(sensitivity = 6.67)
    float lowCurr = calculateHallCurrent(getISenseLow());
    float highCurr = calculateHallCurrent(getISenseHigh());

    if (highCurr >= 20) { // most used cuz HV outputs higher than 20A around 130A
        return highCurr;
    } else return lowCurr;
}

