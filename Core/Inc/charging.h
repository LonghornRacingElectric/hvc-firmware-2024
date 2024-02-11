#ifndef HVC_FIRMWARE_2024_CHARGING_H
#define HVC_FIRMWARE_2024_CHARGING_H
#include "angel_can.h"


#define MAX_CURRENT 13.5f // Max current that the battery can charge with
#define MAX_VOLTAGE 595.0f // Max voltage that the battery can charge with
#define PILOT_TOLERANCE 0.5f // Tolerance between the PWM high's and low's

typedef enum ChargerCxnState {
    NOT_CONNECTED, // Bruv you ain't even connected
    SWITCH_PRESSED, // You're connected but the button is pressed, which releases S3, adding 330 ohms to the circuit
    SWITCH_RELEASED, // You're connected and the button is released, which shorts S3, which removes 330 ohms from the circuit
} ProxState;

typedef enum ChargerStatus {
    STANDBY,
    DETECTION,
    READY,
    NOPOWER,
    CHARGE_ERROR
} PilotState;

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
 * Get the state of the proximity signal
 * States are NOT_CONNECTED, SWITCH_PRESSED, SWITCH_RELEASED
 * @return the state of the proximity signal
 */
ProxState getProxState();

/**
 * Get the state of the pilot signal
 * States are STANDBY, DETECTION, READY, NOPOWER, CHARGE_ERROR
 * @return the state of the pilot signal
 */
PilotState getPilotState();

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
 * @param delta
 * @return CCS status byte
 */
int chargingPeriodic(float delta);

#endif //HVC_FIRMWARE_2024_CHARGING_H
