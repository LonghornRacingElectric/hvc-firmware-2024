//
// Created by rolandwang on 11/12/2023.
//

#include "../Inc/contactors.h"
#include "main.h"

void setPrechargeContactor(bool on) {
    HAL_GPIO_WritePin(Precharge_Enable_GPIO_Port, Precharge_Enable_Pin, (GPIO_PinState) on);
}

void setDriveContactor(bool on) {
    HAL_GPIO_WritePin(Close_HV__Signal_GPIO_Port, Close_HV__Signal_Pin, (GPIO_PinState) on);
    HAL_GPIO_WritePin(Close_HV__SignalC5_GPIO_Port, Close_HV__SignalC5_Pin, (GPIO_PinState) on);
}

void setDischargeDisable(bool disable) {
    HAL_GPIO_WritePin(Discharge_Disable_GPIO_Port, Discharge_Disable_Pin, (GPIO_PinState) disable);
}