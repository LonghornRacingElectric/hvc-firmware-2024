
#include "vcu.h"
#include "angel_can.h"
#include "isense.h"
#include "vsense.h"
#include "cells.h"
#include "imu.h"
#include "fans.h"

static CanInbox parameterInbox;
static CanOutbox packStatus;
static CanOutbox imuAccel;
static CanOutbox imuGyro;
static CanOutbox indicatorStatus;
static CanOutbox fanRPMs;

void vcuInit() {
    can_addInbox(VCU_HVC_PARAMS, &parameterInbox);
    can_addOutbox(HVC_VCU_PACK_STATUS, 0.01f, &packStatus);
    can_addOutbox(HVC_VCU_IMU_ACCEL, 0.01f, &imuAccel);
    can_addOutbox(HVC_VCU_IMU_GYRO, 0.01f, &imuGyro);
    can_addOutbox(HVC_VCU_AMS_IMD, 0.1f, &indicatorStatus);
    can_addOutbox(HVC_VCU_FAN_RPM, 0.1f, &fanRPMs);
}

/**
 *  Get pack current, voltage, SoC, max temp -> store in data array (2 bytes each)
 *  Get imu accel and gyro data
 * */
void vcuPeriodic(bool amsIndicator, bool imdIndicator) {

    // Battery Pack and IMU Data
    can_writeBytes(packStatus.data, 0, 1, (uint16_t) (getPackVoltageFromCells() / 0.01f));
    can_writeBytes(packStatus.data, 2, 3, (uint16_t) (getPackCurrent() / 0.1f));
    can_writeBytes(packStatus.data, 4, 4, (uint8_t) getSoC());
    can_writeBytes(packStatus.data, 5, 5, (uint8_t) getMaxTemp());

    can_writeBytes(imuAccel.data, 0, 1, (int16_t) accelData.x);
    can_writeBytes(imuAccel.data, 2, 3, (int16_t) accelData.y);
    can_writeBytes(imuAccel.data, 4, 5, (int16_t) accelData.z);

    can_writeBytes(imuGyro.data, 0, 1, (int16_t) gyroData.x);
    can_writeBytes(imuGyro.data, 2, 3, (int16_t) gyroData.y);
    can_writeBytes(imuGyro.data, 4, 5, (int16_t) gyroData.z);

    can_writeBytes(fanRPMs.data, 0, 1, (uint16_t) trueRpmMain);
    can_writeBytes(fanRPMs.data, 2, 3, (uint16_t) trueRpmUnique);

    // Indicator Status
    can_writeBytes(indicatorStatus.data, 0, 0, (uint8_t) amsIndicator);
    can_writeBytes(indicatorStatus.data, 1, 1, (uint8_t) imdIndicator);

    // Check VCU->HVC Params Inbox
    if(parameterInbox.isRecent) {
        parameterInbox.isRecent = false;
        auto minTempParam = (float) can_readBytes(parameterInbox.data, 0, 0);
        auto maxTempParam = (float) can_readBytes(parameterInbox.data, 1, 1);
        updateTempParameters(minTempParam, maxTempParam);
    }
}
