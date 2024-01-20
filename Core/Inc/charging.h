#ifndef HVC_FIRMWARE_2024_CHARGING_H
#define HVC_FIRMWARE_2024_CHARGING_H
#include "angel_can.h"
/**
 * Initialize the charging control system (CCS) CAN communication.
 */
void chargingInit();

/**
 * Check if the CCS is connected
 * @return true if connected
 */
bool isChargingConnected();

/**
 * Send a CCS limit command, with the parameters being multiplied by 10 to reflect fixed-point system that CCS uses
 * @param volt_adj
 * @param curr_adj
 * @return CAN error code
 */
uint32_t charging_can_send(float volt_adj, float curr_adj);

/**
 * Receive a CCS output, with the parameters being divided by 10 to reflect fixed-point system that CCS uses
 * @param voltage_given
 * @param current_given
 * @return status byte
 */
uint32_t charging_can_receive(float& voltage_given, float& current_given);


/**
 * Send a CCS limit command and read the CCS output. Sets fault according the the status byte
 * @param voltage_limit
 * @param current_limit
 * @param voltage_given
 * @param current_given
 * @param delta
 * @return CCS status byte
 */
int chargingPeriodic(float voltage_limit, float current_limit, float& voltage_given, float& current_given, float delta);

#endif //HVC_FIRMWARE_2024_CHARGING_H
