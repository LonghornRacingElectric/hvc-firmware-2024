
#include "../Inc/fan.h"
#include "tim.h"
#include "main.h"

/**
 * Control loop for the battery fan
 */
void setFanRpm(float rpm, float deltaTime) {

    static float time;
    static float trueRpm;
    static float pwmDutyCycle = 0;
    static bool prevTach = false; // true is HIGH, false is LOW
    static bool currTach = false;


    // detects when tach high falls to low
    currTach = HAL_GPIO_ReadPin(Tach_from_Fan_GPIO_Port, Tach_from_Fan_Pin) == GPIO_PIN_SET;
    time = time + deltaTime;

    if(prevTach > currTach) {
        trueRpm = (1.0f/time) * 60.0f;
        time = 0;
        pwmDutyCycle += (rpm > trueRpm) ? 0.01f : -0.01f;

    }
    prevTach = currTach;


    // update fan based on requested rpm
    TIM3->CCR4 = (uint8_t)(pwmDutyCycle * 255.0f);

}
