#include "charging.h"
#include "fdcan.h"
#include "adc.h"
#include "gpio.h"
#include "angel_can_ids.h"
#include "faults.h"

static FDCAN_HandleTypeDef* ccs_can_handle;
static float accum_time;
static ProxState charger_state = NOT_CONNECTED;
static PilotState pilot_state = STANDBY;

static float max_charger_current = 0.0; //Max current that charger will provide



static CanOutbox ccs_outbox;

void chargingInit() {
    int error = HAL_FDCAN_Start(&hfdcan2);
    if(error != HAL_OK) {
        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS);
    }
    ccs_can_handle = &hfdcan2;
    accum_time = 0;
    can_addOutbox(HVC_VCU_CCS_INFO, 1.0f, &ccs_outbox);
}

bool isChargingConnected() {
    return charger_state == SWITCH_RELEASED;
}

static bool isDistinct(float released_v, float pressed_v){
  float TOLERANCE = 0.2;
  float delta = abs(released_v - pressed_v);
  return delta > TOLERANCE;
}

ProxState getProxState(){

  static float switch_pressed_voltage = 0;
  static float switch_released_voltage = 0;

  auto proximityVoltage = getProximity(); // Samples ADC value from Proximity
    // FSM for the charger state:
    // NOT_CONNECTED --(if prox < 2.68)--> SWITCH_PRESSED --(another drop is detected)--> SWITCH_RELEASED --(rise is detected)--> SWITCHED_PRESSED --(voltage rises greater than 2.68)--> NOT_CONNECTED
  switch(charger_state){
    case NOT_CONNECTED:
      if(proximityVoltage > 2.68){
        charger_state = NOT_CONNECTED;
      }
      else {
        charger_state = SWITCH_PRESSED;
        switch_pressed_voltage = proximityVoltage;
      }
      break;
    case SWITCH_PRESSED:
      if(proximityVoltage > 2.68){
        charger_state = NOT_CONNECTED;
        break;
      }
      if(isDistinct(proximityVoltage, switch_pressed_voltage) &&
         switch_pressed_voltage > proximityVoltage){
        // If the new voltage is significantly lower than the old voltage, then the switch is released
        charger_state = SWITCH_RELEASED;
        switch_released_voltage = proximityVoltage;
      }
      else{
        charger_state = SWITCH_PRESSED;
      }
      break;
    case SWITCH_RELEASED:
      if(isDistinct(proximityVoltage, switch_released_voltage) &&
         switch_released_voltage < proximityVoltage){
        // If the new voltage is significantly higher than the old voltage, then the switch is pressed again
        charger_state = SWITCH_PRESSED;
        switch_pressed_voltage = proximityVoltage;
      }
      else{
        charger_state = SWITCH_RELEASED;
      }
      break;
  }
  return charger_state;
}

static void getPilotParams(uint8_t& duty_cycle, float& pilotVoltage){

  static float currPilotVoltage = 0.0;
  static float prevPilotVoltage = 0.0;
  static uint8_t incomplete_duty_cycle = 0;
  static unsigned int highCount = 0, lowCount = 0;

  currPilotVoltage = getPilot(); // Samples ADC value from Pilot
  // Checks whether it's positive edge to start new sampling and end the previous sampling
  if(currPilotVoltage - prevPilotVoltage > PILOT_TOLERANCE && prevPilotVoltage < PILOT_TOLERANCE){

    duty_cycle = incomplete_duty_cycle; // Incomplete duty cycle is calculated below
    highCount = 0;
    lowCount = 0;

  }
  if(currPilotVoltage > PILOT_TOLERANCE) { // If voltage is beyond a doubt higher than 0.0V
    pilotVoltage = currPilotVoltage; // Records the height of the wave for detecting charger status
    highCount++;
  }
  else{
    lowCount++;
  }
  incomplete_duty_cycle = highCount / (highCount + lowCount) * 100; // High samples / Total Samples is duty cycle
  prevPilotVoltage = currPilotVoltage; // Saves the previous voltage for the next iteration
}

PilotState getPilotState(){
  float pilotVoltage = 0.0;
  uint8_t duty_cycle = 0;
  getPilotParams(duty_cycle, pilotVoltage); // CalcuLates the duty cycle and the high pilot voltage

  // Calculates the maximum current allowed by the charger
  if(duty_cycle > 85 && duty_cycle <= 100){
    max_charger_current = ((float) duty_cycle - 64.0f)  * 2.5f;
  }
  else if(duty_cycle >= 10 && duty_cycle <= 85){
    max_charger_current = (float) duty_cycle * 0.6f;
  }
  else{ // If duty cycle is between 0 and 10, or is invalid
    max_charger_current = 0.0f;
  }

  // Determines the state of the charging from the Pilot Voltage (set by us)
  if(pilotVoltage > 2.75 && pilotVoltage < 3.3){
    return STANDBY;
  }
  else if(pilotVoltage < 2.75 && pilotVoltage > 2.2){
    return DETECTION;
  }
  else if(pilotVoltage < 1.925 && pilotVoltage > 1.375){
    return READY;
  }
  else if(pilotVoltage == 0) {
    return NOPOWER;
  }
  else {
    return CHARGE_ERROR;
  }
}

uint32_t charging_can_send(uint16_t volt_adj, uint16_t curr_adj) {

    uint8_t data[8] = {0};
//    can_writeBytes(data, 0, 1, volt_adj);
//    can_writeBytes(data, 2, 3, curr_adj);
//    can_writeBytes(data, 4, 7, 0);

    static FDCAN_TxHeaderTypeDef TxHeader;
    TxHeader.Identifier = HVC_CCS_LIMIT;
    TxHeader.IdType = FDCAN_EXTENDED_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = FDCAN_DLC_BYTES_8;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;

    uint32_t error = HAL_FDCAN_AddMessageToTxFifoQ(ccs_can_handle, &TxHeader, data);
    if (error != HAL_OK) {
        if(error & HAL_FDCAN_ERROR_FIFO_FULL) {
            // TODO raise fault
        } else if(error & 0xFF) {
            return ccs_can_handle->ErrorCode;
        }
    }

  // Sends out CCS data to VCU -> Texas Tune
//    can_writeBytes(ccs_outbox.data, 0, 1, volt_adj * 10);
//    can_writeBytes(ccs_outbox.data, 2, 3, curr_adj * 10);
    return 0;
}

uint32_t charging_can_receive(float& voltage_given, float& current_given) {
    static FDCAN_RxHeaderTypeDef RxHeader;
    uint8_t data[8] = {0};
    uint32_t error = HAL_FDCAN_GetRxMessage(ccs_can_handle, FDCAN_RX_FIFO0, &RxHeader, data);
    uint32_t id = RxHeader.Identifier;
    // If error code is something other than the fifo being empty or full, return error
    if(error != HAL_OK) {
        if ((ccs_can_handle->ErrorCode & 0xFF) != HAL_FDCAN_ERROR_NONE) {
            return ccs_can_handle->ErrorCode;
        }
    }
    if(id != CCS_HVC_OUTPUT && id != HVC_CCS_LIMIT) return 0; //If this happens, there is a collision between Charging CAN and our CAN

//    voltage_given = ((float) can_readBytes(data, 0, 1)) / 10.0f;
//    current_given = ((float) can_readBytes(data, 2, 3)) / 10.0f;
//    uint8_t status = can_readBytes(data, 5, 5);
//    if(status & 0b00000001) {
//        // Set CCS Hardware Failure Fault
//        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS_HARDWARE);
//    }
//    if(status & 0b00000010) {
//        // Set CCS Over Temperature Protection Fault
//        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS_OVERTEMP);
//    }
//    if(status & 0b00000100) {
//        // Set CCS Incorrect Voltage Fault
//        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS_INCORRECT_VOLTAGE);
//    }
//    if(status & 0b00001000) {
//        // Set Charger is Off Fault
//        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS_CHARGER_OFF);
//    }
//    if(status & 0b00010000) {
//        // Set CCS Communication TimeOut Fault
//        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS);
//    }
    return 0;
}


int chargingPeriodic(float delta) {

    float voltage_limit = 0.0f;
    float current_limit = 0.0f;
    float voltage_given = 0.0f;
    float current_given = 0.0f;

    charger_state = getProxState();
    if(!isChargingConnected()) {
      writePilotCtrl(0); // Turn off the pilot signal
      return -1;
    }
    writePilotCtrl(1); // Turn on the pilot signal

    pilot_state = getPilotState();
    if(pilot_state == STANDBY || pilot_state == NOPOWER || pilot_state == CHARGE_ERROR){
        return -1;
    }

    if(pilot_state == DETECTION){
      voltage_limit = 0;
      current_limit = 0;
    }
    else { // IS READY TO BE CHARGED
      voltage_limit = MAX_VOLTAGE;
      current_limit = max_charger_current > MAX_CURRENT ? MAX_CURRENT : max_charger_current;
    }

    accum_time += delta;
    if(accum_time < 1.0f){
      return -1;
    }
    else{
      accum_time = 0;
    }
    uint32_t error_tx = charging_can_send(static_cast<uint16_t>(voltage_limit * 10), static_cast<uint16_t>(current_limit * 10));

    uint32_t error_rx = charging_can_receive(voltage_given, current_given);

    return (error_tx + error_rx);
}
