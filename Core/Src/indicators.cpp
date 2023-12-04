//
// Created by rolandwang on 11/12/2023.
//

#include "indicators.h"
#include "stm32h7xx_hal.h"
#include "angel_can.h"

void setIndicatorLights(bool amsIndicator, bool imdIndicator)
{
    // Runs code every 100 cycles
    static int i = 0;
    if(++i < 100) return;
    i = 0;

    uint8_t data[2] = {amsIndicator, imdIndicator};
    can_send(HVC_VCU_AMS_IMD, 2, data);
}