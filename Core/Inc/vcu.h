#ifndef HVC_FIRMWARE_2024_VCU_H
#define HVC_FIRMWARE_2024_VCU_H

/**
 * Initialize CAN communication with vcu.
 */
void vcuInit();

/**
 * Send IMU data over CAN. Precision is 0.01 m/s^2 for accel and 0.01 rad/s.
 * @param accel_x
 * @param accel_y
 * @param accel_z
 * @param gyro_x
 * @param gyro_y
 * @param gyro_z
 */
void vcuSendImu(float accel_x, float accel_y, float accel_z, float gyro_x, float gyro_y, float gyro_z);

/**
 * Get status data from VCU.
 */
void vcuPeriodic();

#endif //HVC_FIRMWARE_2024_VCU_H
