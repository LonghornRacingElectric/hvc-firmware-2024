#include "charging.h"
#include "fdcan.h"
#include "angel_can_ids.h"
#include "faults.h"

static FDCAN_HandleTypeDef* ccs_can_handle;
static float accum_time;
static bool connected = false;

static CanOutbox ccs_outbox;

void chargingInit() {
    int error = HAL_FDCAN_Start(&hfdcan2);
    if(error != HAL_OK) {
        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS);
    }
    else{
        ccs_can_handle = &hfdcan2;
        connected = true;
        accum_time = 0;
    }
    can_addOutbox(HVC_VCU_CCS_INFO, 1.0f, &ccs_outbox);
}

bool isChargingConnected() {
    return connected;
}

uint32_t charging_can_send(uint16_t volt_adj, uint16_t curr_adj) {
    uint8_t data[8] = {0};
    can_writeBytes(data, 0, 1, volt_adj);
    can_writeBytes(data, 2, 3, curr_adj);
    can_writeBytes(data, 4, 7, 0);

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

    voltage_given = ((float) can_readBytes(data, 0, 1)) / 10.0f;
    current_given = ((float) can_readBytes(data, 2, 3)) / 10.0f;
    uint8_t status = can_readBytes(data, 5, 5);
    if(status & 0b00000001) {
        // Set CCS Hardware Failure Fault
        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS_HARDWARE);
    }
    if(status & 0b00000010) {
        // Set CCS Over Temperature Protection Fault
        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS_OVERTEMP);
    }
    if(status & 0b00000100) {
        // Set CCS Incorrect Voltage Fault
        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS_INCORRECT_VOLTAGE);
    }
    if(status & 0b00001000) {
        // Set Charger is Off Fault
        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS_CHARGER_OFF);
    }
    if(status & 0b00010000) {
        // Set CCS Communication TimeOut Fault
        FAULT_SET(&hvc_fault_vector, FAULT_HVC_CCS);
    }
    return 0;
}


int chargingPeriodic(float voltage_limit, float current_limit, float& voltage_given, float& current_given, float delta) {

    accum_time += delta;
    if(accum_time < 1.0f) {
        return -1;
    }
    else{
        accum_time = 0;
    }

    uint32_t error_tx = charging_can_send(static_cast<uint16_t>(voltage_limit * 10), static_cast<uint16_t>(current_limit * 10));

    uint32_t error_rx = charging_can_receive(voltage_given, current_given);

    return (error_tx + error_rx);
}
