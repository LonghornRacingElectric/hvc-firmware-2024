//
// Created by rolandwang on 11/12/2023.
//

#include "isense.h"
#include "adc.h"

bool isPackCurrentWithinBounds() {

}

float getPackCurrent() {
    float lowCurr = getA_HV_lowCurr_to_MCU(); // set value
    float highCurr = getA_HV_highCurr_to_MCU();



}

