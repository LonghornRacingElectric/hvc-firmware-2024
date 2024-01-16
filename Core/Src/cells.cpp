
#include "cells.h"
#include "angel_can.h"
#include "adbms.h"


static LTC6813_Command_t CMD_RDCVs[5] = {CMD_RDCVA,CMD_RDCVB,CMD_RDCVC,CMD_RDCVD,CMD_RDCVE};
static CanOutbox cellVoltages[35];
static CanOutbox cellTemps[13];

/**
 * Initializes CAN outboxes to send voltage and temp data
 **/
void cellsInit() {
    can_addOutboxes(HVC_VCU_CELL_VOLTAGES_START, HVC_VCU_CELL_VOLTAGES_END, 0.0285f, cellVoltages);
    can_addOutboxes(HVC_VCU_CELL_TEMPS_START, HVC_VCU_CELL_TEMPS_END, 0.0769f, cellTemps);
}

/**
 * Reads voltage and temperature data from BMS
 * Converts and writes data into respective CanOutboxes
 **/
void cellsPeriodic() {

    // Reads in 30 voltages per loop into indexes 280-339, then sorts them into correct order from 0-279 (28 bytes/chip, 10 LTC chips)
    for(int i = 0 ; i < 5 ; i++) {
        ltc6813_cmd_read(CMD_RDCVs[i], voltageData+280);
        for(int j = 0 ; j < NUM_BMS_ICS ; j++) {
            for(int k = 0 ; k < 6 ; k++) {
                voltageData[i*60+j*6+k] = voltageData[280+j*6+k];
            }
        }
    }

    // Reads in 91 temps
    for(int i = 0 ; i < 91 ; i++) {
        // store data

        // check for min and max temp of each data point
        if(maxTemp < tempData[i]) maxTemp = tempData[i];
        if(minTemp > tempData[i]) minTemp = tempData[i];
    }

    // Writes voltage values into CanOutboxes
    for(int i = 0 ; i < 35 ; i++) {
        for(int j = 0 ; j < 8 ; j++) {
            can_writeBytes(cellVoltages[i].data, j, j, voltageData[i*8+j]);
        }
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

    for(int i = 0 ; i < 4; i++) {
        auto value = (uint16_t) (voltageData[i+offset] / 0.001f);
        auto byte1 = static_cast<uint8_t>((value & 0xFF00) >> 8);
        auto byte2 = static_cast<uint8_t>(value & 0x00FF);

        voltages[i*2] = byte1;
        voltages[i*2+1] = byte2;
    }

    // sends data over can and increments canID
    can_send(idTracker, 8, voltages);
    idTracker = idTracker + 8;
}

float getSoC() {

}

float getMaxTemp() {
    return maxTemp;
}

float getMinTemp() {
    return minTemp;
}