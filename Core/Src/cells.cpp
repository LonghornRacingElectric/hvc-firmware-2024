//
// Created by rolandwang on 11/12/2023.
//

#include "cells.h"
#include "can.h"

/** creating spi request, store data, choose when to send temp & voltage packets
 * find min max temps after low pass filter
 **/
void cellsPeriodic() {
    // Create SPI inbox request

    // Process Temperature Data
    for(int i = 0 ; i < 91 ; i++) {
        // store data

        // check for min and max temp of each data point
        if(maxTemp < tempData[i]) maxTemp = tempData[i];
        if(minTemp > tempData[i]) minTemp = tempData[i];
    }

    // Process Voltage Data
    for(int i = 0 ; i < 140 ; i++) {
        // store data

    }

    // Send Temp and Voltage Packets (periodically)

}

static void sendTempPacket() {

}

static void sendVoltagePacket() {

}