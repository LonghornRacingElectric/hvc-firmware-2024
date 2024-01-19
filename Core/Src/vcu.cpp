#include "vcu.h"
#include "angel_can.h"

static CanInbox parameterInbox;
static CanOutbox imuAccelOutbox;
static CanOutbox imuGyroOutbox;

void vcuInit() {
    can_addInbox(VCU_HVC_PARAMS, &parameterInbox);
    can_addOutbox(HVC_VCU_IMU_ACCEL, 0.250f, &imuAccelOutbox);
    can_addOutbox(HVC_VCU_IMU_GYRO, 0.250f, &imuGyroOutbox);
}

void vcuSendImu(float accel_x, float accel_y, float accel_z, float gyro_x, float gyro_y, float gyro_z){
    can_writeBytes(imuAccelOutbox.data, 0, 1, (int16_t) (accel_x * 100));
    can_writeBytes(imuAccelOutbox.data, 2, 3, (int16_t) (accel_y * 100));
    can_writeBytes(imuAccelOutbox.data, 4, 5, (int16_t) (accel_z * 100));
    imuAccelOutbox.isRecent = true;

    can_writeBytes(imuGyroOutbox.data, 0, 1, (int16_t) (gyro_x * 100));
    can_writeBytes(imuGyroOutbox.data, 2, 3, (int16_t) (gyro_y * 100));
    can_writeBytes(imuGyroOutbox.data, 4, 5, (int16_t) (gyro_z * 100));
    imuGyroOutbox.isRecent = true;
}

void vcuPeriodic(){
    // TODO implement
    vcuSendImu(0, 0, 0, 0, 0, 0);
    if(parameterInbox.isRecent){
        parameterInbox.isRecent = false;
        // TODO implement
    }
}
