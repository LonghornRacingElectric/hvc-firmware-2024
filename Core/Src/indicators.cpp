//
// Created by rolandwang on 11/12/2023.
//

#include "indicators.h"
#include "stm32h7xx_hal.h"
#include "angel_can.h"
#include "clock.h"

void setIndicatorLights(bool amsIndicator, bool imdIndicator)
{
    // 10 Hz timer
    static float timer = 0.0f;
    timer += clock_getDeltaTime();
    if(timer >= 10.0f) {
        timer = 0.0f;
        uint8_t data[2] = {amsIndicator, imdIndicator};
        can_send(HVC_VCU_AMS_IMD, 2, data);
    }
}