
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
    static float pulseTimes[10];
    static float timeTotal = 0;
    static float numPulses = 0;
    static int index = 0;
    static bool prevTach = false; // true is HIGH, false is LOW
    static bool currTach = false;


    // detects when tach high falls to low
    currTach = HAL_GPIO_ReadPin(Tach_from_Fan_GPIO_Port, Tach_from_Fan_Pin) == GPIO_PIN_SET;
    time = time + deltaTime;

    if(prevTach > currTach) {
        pulseTimes[index] = time;
        time = 0;
        index++;
        index %= 10;

        numPulses = 0;
        timeTotal = 0;
        for(float t : pulseTimes) {
            if(t > 0.0f) numPulses += 1;
            timeTotal += t;
        }
        trueRpm = numPulses / timeTotal * 60.0f;

        pwmDutyCycle += (rpm > trueRpm) ? 0.01f : -0.01f;
        if(pwmDutyCycle > 1.0f) pwmDutyCycle = 1.0f;
        if(pwmDutyCycle < 0.0f) pwmDutyCycle = 0.0f;
    }
    prevTach = currTach;


    // update fan based on requested rpm
    TIM3->CCR4 = (uint8_t)(pwmDutyCycle * 255.0f);

}
