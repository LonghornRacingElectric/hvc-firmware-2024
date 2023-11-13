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
    static int i = 0;
    if(++i == 50) {
        sendTempPacket();
    }
    else if (i >= 100) {
        sendVoltagePacket();
        i = 0;
    }
}

/** sends 13 can packets with 7 bytes of temp data each, 1 byte per temp
 *  only sends 1 can packet when called, resets when it reaches 13
 */
static void sendTempPacket() {
    // keeps track of which canID we should be sending can packet over, resets after 13th packet
    static uint32_t idTracker = HVC_VCU_CELL_TEMPS_START;
    if(idTracker == HVC_VCU_CELL_TEMPS_START + 91) {
        idTracker = HVC_VCU_CELL_TEMPS_START;
    }

    // converts tempData from float to uint8_t of the specific can packet
    static uint8_t rounded_temps[7];
    static int offset = (int)(idTracker - HVC_VCU_CELL_TEMPS_START);
    for(int i = offset ; i < (offset + 7) ; i++) {
        rounded_temps[i-offset] = (uint8_t)tempData[i];
    }

    // sends data over can and increments canID
    can_send(idTracker, 7, rounded_temps);
    idTracker = idTracker + 7;
}

/** sends 35 can packets with 8 bytes of voltage data each, 2 byte per voltage
 *  only sends 1 can packet when called, resets when it reaches 35
 */
static void sendVoltagePacket() {
    // keeps track of which canID we should be sending can packet over, resets after 35th packet
    static uint32_t idTracker = HVC_VCU_CELL_VOLTAGES_START;
    if(idTracker == HVC_VCU_CELL_VOLTAGES_START + 280) {
        idTracker = HVC_VCU_CELL_VOLTAGES_START;
    }

    // convert voltageData from float in mV to uint8_t that uses 2 bytes per voltage
    static uint8_t voltages[8];
    static int offset = (int)(idTracker - HVC_VCU_CELL_VOLTAGES_START) / 2;

    for(int i = offset ; i < (offset + 4) ; i++) {
        auto value = (uint16_t) (voltageData[i] / 0.001f);
        auto byte1 = static_cast<uint8_t>((value & 0xFF00) >> 8);
        auto byte2 = static_cast<uint8_t>(value & 0x00FF);

        voltages[i] = byte1;
        voltages[i+1] = byte2;
    }

    // sends data over can and increments canID
    can_send(idTracker, 8, voltages);
    idTracker = idTracker + 8;
}