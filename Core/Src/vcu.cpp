
#include "vcu.h"
#include "angel_can.h"
#include "isense.h"
#include "vsense.h"
#include "cells.h"
#include "imu.h"
#include "clock.h"

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
void vcuPeriodic() {
    can_writeBytes(packData, 0, 2, (uint16_t) (getPackVoltage() / 0.001f));
    can_writeBytes(packData, 2, 4, (uint16_t) (getPackCurrent() / 0.1f));
    can_writeBytes(packData, 4, 5, (uint16_t) getSoC());
    can_writeBytes(packData, 5, 6, (uint8_t) getMaxTemp());

    can_writeBytes(imuAccel, 0, 2, (uint16_t) (imu_getAccel()));
    can_writeBytes(imuAccel, 2, 4, (uint16_t) (imu_getAccel()));
    can_writeBytes(imuAccel, 4, 6, (uint16_t) (imu_getAccel()));

    can_writeBytes(imuGyro, 0, 2, (uint16_t) (imu_getGyro()));
    can_writeBytes(imuGyro, 2, 4, (uint16_t) (imu_getGyro()));
    can_writeBytes(imuGyro, 4, 6, (uint16_t) (imu_getGyro()));

    static float timer = 0.0f;
    timer += clock_getDeltaTime();
    if(timer >= 0.01f) {
        timer = 0.0f;
        can_send(HVC_VCU_PACK_STATUS, 6, packData);
        can_send(HVC_VCU_IMU_ACCEL, 6, imuAccel);
        can_send(HVC_VCU_IMU_GYRO, 6, imuGyro);
    }
}

/**
 *
 */
 void receiveParams() {

 }
