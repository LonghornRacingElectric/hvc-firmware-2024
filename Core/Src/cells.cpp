//
// Created by rolandwang on 11/12/2023.
//

#include "cells.h"

/** creating spi request, store data, choose when to send temp & voltage packets
 * find min max temps after low pass filter
 **/
void cellsPeriodic() {
    // Create SPI inbox request

    // Process Temperature Data
    for(int i = 0 ; i < sizeof(tempData) ; i++) {
        // store data

        // check for min and max temp of each data point
        float temp = tempData[i];
        if(maxTemp < temp) maxTemp = temp;
        if(minTemp > temp) minTemp = temp;
    }

    // Process Voltage Data
    for(int i = 0 ; i < sizeof(voltageData) ; i++) {
        // store data

    }
}