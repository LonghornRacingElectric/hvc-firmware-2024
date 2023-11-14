//
// Created by rolandwang on 11/12/2023.
//

#include "../Inc/contactors.h"
#include "main.h"

void setPrechargeContactor(bool on) {
    if(on) {
        HAL_GPIO_WritePin(Precharge_Enable_GPIO_Port, Precharge_Enable_Pin, GPIO_PIN_SET);
    }
    else {
        HAL_GPIO_WritePin(Precharge_Enable_GPIO_Port, Precharge_Enable_Pin, GPIO_PIN_RESET);
    }
}

void setDriveContactor(bool on) {
    if(on) {
        HAL_GPIO_WritePin(Close_HV__Signal_GPIO_Port, Close_HV__Signal_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Close_HV__SignalC5_GPIO_Port, Close_HV__SignalC5_Pin, GPIO_PIN_SET);
    }
    else {
        HAL_GPIO_WritePin(Close_HV__Signal_GPIO_Port, Close_HV__Signal_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Close_HV__SignalC5_GPIO_Port, Close_HV__SignalC5_Pin, GPIO_PIN_RESET);
    }
}

void setDischargeDisable(bool disable) {
    if(disable) {
        HAL_GPIO_WritePin(Discharge_Disable_GPIO_Port, Discharge_Disable_Pin, GPIO_PIN_RESET);
    }
    else {
        HAL_GPIO_WritePin(Discharge_Disable_GPIO_Port, Discharge_Disable_Pin, GPIO_PIN_SET);
    }
}