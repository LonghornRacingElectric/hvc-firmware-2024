
#ifndef HVC_FIRMWARE_2024_VCU_H
#define HVC_FIRMWARE_2024_VCU_H

void vcuInit();

/**
 * gather IMU and pack data, send packets periodically
 */
void vcuPeriodic();

/**
 * receive adjustable param updates from vcu
 */
void receiveParams();

#endif //HVC_FIRMWARE_2024_VCU_H
