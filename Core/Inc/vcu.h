//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_VCU_H
#define HVC_FIRMWARE_2024_VCU_H

void vcuInit();

/**
 * gather IMU and pack data, send packets periodically
 */
void vcuPeriodic();

#endif //HVC_FIRMWARE_2024_VCU_H
