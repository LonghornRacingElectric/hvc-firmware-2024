#include "cells.h"
#include "angel_can.h"
#include "adbms.h"
#include "usb.h"

// 0-4 is read cell voltages, 5-8 is read cell temperatures
static LTC6813_Command_t CMD_RDCs[9] = {CMD_RDCVA, CMD_RDCVB, CMD_RDCVC, CMD_RDCVD, CMD_RDCVE,
                                        CMD_RDAUXA, CMD_RDAUXB, CMD_RDAUXC, CMD_RDAUXD};
static CanOutbox cellVoltages[35];
static CanOutbox cellTemps[23];

/**
 * Initializes CAN outboxes to send voltage and temp data
 **/
void cellsInit() {
  // Period set to update all values at a frequency of 1 Hz
  can_addOutboxes(HVC_VCU_CELL_VOLTAGES_START, HVC_VCU_CELL_VOLTAGES_END, 1.0f, cellVoltages);
  can_addOutboxes(HVC_VCU_CELL_TEMPS_START, HVC_VCU_CELL_TEMPS_END, 1.0f, cellTemps);
}


/**
 * Reads voltage and temperature data from BMS
 * Converts and writes data into respective CanOutboxes
 **/
void cellsPeriodic() {
  static int cmd_ID = 0; // Used to track and send only 1 LTC cmd per iteration
  static uint16_t value = 0;

  // LTC read cmd 0-4 for voltages
  if (cmd_ID < 5) {
    ltc6813_adcv(cmd_ID + 1);
    ltc6813_cmd_read(CMD_RDCs[cmd_ID], rawData);
    for (int j = 0; j < NUM_BMS_ICS; j++) {
      if (cmd_ID == 4) {
        for (int k = 0; k < 2; k++) {
          value = (rawData[j * 6 + k * 2 + 1] << 8) | rawData[j * 6 + k * 2];
          voltageData[cmd_ID * 3 + j * 14 + k] = convertVoltage(value);
        }
      } else {
        for (int k = 0; k < 3; k++) {
          value = (rawData[j * 6 + k * 2 + 1] << 8) | rawData[j * 6 + k * 2];
          voltageData[cmd_ID * 3 + j * 14 + k] = convertVoltage(value);
        }
      }

    }
  }

  // Writes voltage values into CanOutboxes
  for (int i = 0; i < 35; i++) {
    for (int j = 0; j < 4; j++) {
      can_writeFloat(uint16_t, &cellVoltages[i], j * 2, voltageData[i * 4 + j], 0.0001f);
    }
  }

  // LTC read cmd 5-8 for temperatures
  if (cmd_ID > 4 && cmd_ID < 9) {
    if(cmd_ID == 5) {
      ltc6813_adax();
    }
    ltc6813_cmd_read(CMD_RDCs[cmd_ID], rawData);
    for (int j = 0; j < NUM_BMS_ICS; j++) {
      // For Auxiliary Register Group A and C, there are 3 temp values
      if (cmd_ID == 5 || cmd_ID == 7) {
        for (int k = 0; k < 3; k++) {
          value = (rawData[j * 6 + k * 2 + 1] << 8) | rawData[j * 6 + k * 2];
          tempData[(cmd_ID - 5) / 2 * 5 + j * 9 + k] = convertTemp(value);
        }
      }
      // For Auxiliary Register Group B, there are 2 temp values
      if (cmd_ID == 6) {
        for (int k = 0; k < 2; k++) {
          value = (rawData[j * 6 + k * 2 + 1] << 8) | rawData[j * 6 + k * 2];
          tempData[3 + j * 9 + k] = convertTemp(value);
        }
      }
      // For Auxiliary Register Group D, there is 1 temp value
      if (cmd_ID == 8) {
        value = (rawData[j * 6 + 1] << 8) | rawData[j * 6];
        tempData[8 + j * 9] = convertTemp(value);
      }
    }
  }

  // Writes temperature values into CanOutboxes
  for (int i = 0; i < 23; i++) {
    for (int j = 0; j < 4; j++) {
      if (i == 22 && j > 1) break;
      can_writeFloat(uint16_t, &cellTemps[i], j * 2, tempData[i * 4 + j], 0.1f);
      checkMinMaxTemps(tempData[i * 4 + j]);
    }
  }

  cmd_ID = (cmd_ID + 1) % 9;

  if (cmd_ID == 0) {
    maxTemp = currentMaxTemp;
    minTemp = currentMinTemp;
    currentMaxTemp = -999.0f;
    currentMinTemp = 999.0f;
  }
}

void checkMinMaxTemps(float temp) {
  if (currentMaxTemp < temp) currentMaxTemp = temp;
  if (currentMinTemp > temp) currentMinTemp = temp;
}

bool areCellVoltagesWithinBounds() {
  for (int i = 0; i < numCells; i++) {
    if (voltageData[i] > 4.2f || voltageData[i] < 2.5f) {
      // TODO send fault message: specify which cell is bad
      return false;
    }
  }
  return true;
}

bool isPackVoltageWithinBounds() {
  packVoltage = getPackVoltageFromCells();
//  println(packVoltage);
  if (packVoltage > 420.0f || packVoltage < 250.0f) {
    // TODO send fault message
    return false;
  }
  return true;
}

float getPackVoltageFromCells() {
  packVoltage = 0.0f;
  for (float value: voltageData) {
    packVoltage += value;
  }
  return packVoltage;
}

bool isTempWithinBounds() {
  return getMinTemp() >= minAllowedTemp && getMaxTemp() <= maxAllowedTemp;
}

float getMaxTemp() {
  return maxTemp;
}

float getMinTemp() {
  return minTemp;
}

float convertTemp(uint16_t Vout) {
  float temp = (Vout/10.0f - Voc) / Tc;
  println(temp);
  return temp;
}

float convertVoltage(uint16_t v) {
  float voltage = v * 0.0001f;
//  println(voltage);
  return voltage;
}

void updateTempParameters(float newMinTemp, float newMaxTemp) {
  minAllowedTemp = newMinTemp;
  maxAllowedTemp = newMaxTemp;
}