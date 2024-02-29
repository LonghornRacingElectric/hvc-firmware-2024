
#include "vcu.h"
#include "angel_can.h"
#include "isense.h"
#include "vsense.h"
#include "cells.h"
#include "imu.h"
#include "fans.h"
#include "tsense.h"
#include "soc_estimation.h"

static CanInbox parameterInbox;
static CanOutbox packStatus;
static CanOutbox imuAccel;
static CanOutbox imuGyro;
static CanOutbox indicatorStatus;
static CanOutbox fanRPMs;
static CanOutbox ccsInfo;
static CanOutbox contactorStatus;

void vcuInit() {
    can_addInbox(VCU_HVC_PARAMS, &parameterInbox);
    can_addOutbox(HVC_VCU_PACK_STATUS, 0.003f, &packStatus);
    can_addOutbox(HVC_VCU_IMU_ACCEL, 0.003f, &imuAccel);
    can_addOutbox(HVC_VCU_IMU_GYRO, 0.003f, &imuGyro);
    can_addOutbox(HVC_VCU_AMS_IMD, 0.003f, &indicatorStatus);
    can_addOutbox(HVC_VCU_FAN_RPM, 0.1f, &fanRPMs);
    can_addOutbox(HVC_VCU_CCS_INFO, 1.0f, &ccsInfo);
    can_addOutbox(HVC_VCU_CONTACTOR_STATUS, .003f, &contactorStatus);
}

/**
 *  Get pack current, voltage, SoC, max temp -> store in data array (2 bytes each)
 *  Get imu accel and gyro data
 * */
void vcuPeriodic(bool amsIndicator, bool imdIndicator, int state, float deltaTime) {

    // Battery Pack and IMU Data
    can_writeFloat(uint16_t, &packStatus, 0, getPackVoltageFromCells(), 0.01f);
    can_writeFloat(int16_t, &packStatus, 2, getPackCurrent(), 0.01f);
    can_writeFloat(uint16_t, &packStatus, 4, getSoC(deltaTime), 0.01f);
    can_writeFloat(uint8_t, &packStatus, 6, getMaxTemp(), 1.0f);
    can_writeFloat(uint8_t, &packStatus, 7, getAmbientTemp(), 1.0f);

    can_writeFloat(int16_t, &imuAccel, 0, accelData.x, 0.01f);
    can_writeFloat(int16_t, &imuAccel, 2, accelData.y, 0.01f);
    can_writeFloat(int16_t, &imuAccel, 4, accelData.z, 0.01f);

    can_writeFloat(int16_t, &imuGyro, 0, gyroData.x, 0.01f);
    can_writeFloat(int16_t, &imuGyro, 2, gyroData.y, 0.01f);
    can_writeFloat(int16_t, &imuGyro, 4, gyroData.z, 0.01f);

    can_writeFloat(int16_t, &fanRPMs, 0, trueRpmMain, 1.0f);
    can_writeFloat(int16_t, &fanRPMs, 2, trueRpmUnique, 1.0f);

    // Indicator Status
    can_writeInt(uint8_t, &indicatorStatus, 0, amsIndicator);
    can_writeInt(uint8_t, &indicatorStatus, 1, imdIndicator);

    // CCS Info
    // TODO implement

    // Contactor Status
    can_writeInt(uint8_t, &contactorStatus, 0, state);

    // Check VCU->HVC Params Inbox
    if(parameterInbox.isRecent) {
        parameterInbox.isRecent = false;
        auto minTempParam = (float) can_readInt(uint8_t, &parameterInbox, 0);
        auto maxTempParam = (float) can_readInt(uint8_t, &parameterInbox, 1);
        updateTempParameters(minTempParam, maxTempParam);
    }
}
