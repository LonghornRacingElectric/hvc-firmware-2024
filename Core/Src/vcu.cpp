
#include "vcu.h"
#include "angel_can.h"
#include "isense.h"
#include "vsense.h"
#include "cells.h"
#include "imu.h"
#include "clock.h"
#include "thermal.h"

static CanRx parameterMailbox;
static uint8_t packData[6];
static uint8_t imuAccel[6];
static uint8_t imuGyro[6];

void vcuInit() {
    can_addMailbox(VCU_HVC_PARAMS, &parameterMailbox);
}

/**
 *  Get pack current, voltage, SoC, max temp -> store in data array (2 bytes each)
 *  Get imu accel and gyro data
 * */
void vcuPeriodic(float deltaTime, bool amsIndicator, bool imdIndicator) {
    static float timer = 0.0f;
    static float timer2 = 0.0f; // Used for Indicator Lights

    timer += deltaTime;
    timer2 += deltaTime;

    if(timer >= 0.01f) {
        timer = 0.0f;
        can_writeBytes(packData, 0, 1, (uint16_t) (getPackVoltage() / 0.01f));
        can_writeBytes(packData, 2, 3, (uint16_t) (getPackCurrent() / 0.1f));
        can_writeBytes(packData, 4, 4, (uint16_t) getSoC());
        can_writeBytes(packData, 5, 5, (uint8_t) getMaxTemp());

        can_writeBytes(imuAccel, 0, 1, (uint16_t) accelData.x);
        can_writeBytes(imuAccel, 2, 2, (uint16_t) accelData.y);
        can_writeBytes(imuAccel, 4, 5, (uint16_t) accelData.z);

        can_writeBytes(imuGyro, 0, 1, (uint16_t) gyroData.x);
        can_writeBytes(imuGyro, 2, 3, (uint16_t) gyroData.y);
        can_writeBytes(imuGyro, 4, 5, (uint16_t) gyroData.z);

        can_send(HVC_VCU_PACK_STATUS, 6, packData);
        can_send(HVC_VCU_IMU_ACCEL, 6, imuAccel);
        can_send(HVC_VCU_IMU_GYRO, 6, imuGyro);
    }

    if(timer2 >= 0.1f) {
            timer2 = 0.0f;
            uint8_t data[2] = {amsIndicator, imdIndicator};
            can_send(HVC_VCU_AMS_IMD, 2, data);
    }

    // Check VCU->HVC Params Mailbox
    if(parameterMailbox.isRecent) {
        auto minTempParam = (float) can_readBytes(parameterMailbox.data, 0, 0);
        auto maxTempParam = (float) can_readBytes(parameterMailbox.data, 1, 1);
        updateParameters(minTempParam, maxTempParam);
    }
}
