
#include "cells.h"
#include "angel_can.h"
#include "adbms.h"


static LTC6813_Command_t CMD_RDCVs[5] = {CMD_RDCVA,CMD_RDCVB,CMD_RDCVC,CMD_RDCVD,CMD_RDCVE};
static LTC6813_Command_t CMD_RDCTs[4] = {CMD_RDAUXA,CMD_RDAUXB,CMD_RDAUXC,CMD_RDAUXD};
static CanOutbox cellVoltages[35];
static CanOutbox cellTemps[12];

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

    // Reads in 30 voltages per read loop into indexes 280-339, then sorts them into correct order from 0-279 (28 bytes/chip, 10 LTC chips)
    for(int i = 0 ; i < 5 ; i++) {
        ltc6813_cmd_read(CMD_RDCVs[i], voltageData+280);
        for(int j = 0 ; j < NUM_BMS_ICS ; j++) {
            for(int k = 0 ; k < 6 ; k++) {
                voltageData[i*6+j*28+k] = voltageData[280+j*6+k];
            }
        }
    }

    // Writes voltage values into CanOutboxes
    for(int i = 0 ; i < 35 ; i++) {
        for(int j = 0 ; j < 8 ; j++) {
            can_writeBytes(cellVoltages[i].data, j, j, voltageData[i*8+j]);
        }
    }

    // Reads in 60 bytes of info into buffer, # of temp data values will differ based on which read cmd
    for(int i = 0 ; i < 4 ; i++) {
        ltc6813_cmd_read(CMD_RDCTs[i], tempData+180);

        for(int j = 0 ; j < NUM_BMS_ICS ; j++) {
            // For Auxiliary Register Group A and C, there are 3 temp values
            if(i == 0 || i == 2) {
                for(int k = 0 ; k < 6 ; k++) {
                    tempData[i*5+j*6+k] = tempData[180+j*6+k];
                }
            }

            // For Auxiliary Register Group B, there are 2 temp values
            if(i == 1) {
                for(int k = 0 ; k < 4 ; k++) {
                    tempData[6+j*6+k] = tempData[180+j*6+k];
                }
            }

            // For Auxiliary Register Group D, there is 1 temp value
            if(i == 3) {
                for(int k = 0 ; k < 2 ; k++) {
                    tempData[16+j*6+k] = tempData[180+j*6+k];
                }
            }
        }



        // check for min and max temp of each data point
        if(maxTemp < (float) tempData[i]) maxTemp = tempData[i];
        if(minTemp > (float) tempData[i]) minTemp = tempData[i];
    }

}

float getSoC() {

}

float getMaxTemp() {
    return maxTemp;
}

float getMinTemp() {
    return minTemp;
}