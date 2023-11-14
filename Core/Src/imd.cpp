//
// Created by rolandwang on 11/12/2023.
//

#include "../Inc/imd.h"
#include "main.h"

bool isImdOk() {
    if(HAL_GPIO_ReadPin(IMD_Data_GPIO_Port, IMD_Data_Pin)) {
        return true;
    }
    else {
        return false;
    }
}