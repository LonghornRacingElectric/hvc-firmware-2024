//
// Created by rolandwang on 11/12/2023.
// Developed by angelasrsh on 3/3/24.
//

#include "vsense.h"
#include "adc.h"
static float MAX_VOLTAGE = 588.0f;
float getTractiveVoltage() {
    //insert calculations needed
    float VSenseVoltage = getVSense();

    /* theory of operation:
     * HV_inputV * (R2/R2+R1) = V_isoAmp
     * V_isoAmp * 8.2 = V_mcu = VSenseVoltage
     *
     * Note: at 588V, output should be 1.6318 V give or take a few mV
     */

    // this code outputs 585.49V at 1.6318V
    return (float) (VSenseVoltage / 8.2f) * (2942.176473f) ; // 2942.176473 is voltage divider ratio

}

