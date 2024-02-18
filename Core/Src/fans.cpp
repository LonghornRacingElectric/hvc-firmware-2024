
#include "fans.h"
#include "tim.h"
#include "main.h"
#include "angel_can.h"

static CanInbox rpmInbox;

/**
 * Reads Tach and Controls Fan PWM
 */
void setFanRpm(float rpm, float deltaTime) {
    currTach = HAL_GPIO_ReadPin(Tach_from_Main_GPIO_Port, Tach_from_Main_Pin) == GPIO_PIN_SET;
    currTach2 = HAL_GPIO_ReadPin(Tach_from_Unique_GPIO_Port, Tach_from_Unique_Pin) == GPIO_PIN_SET;

    time += deltaTime;
    time2 += deltaTime;

    // Detects when tach falls from high to low, and stores pulse time
    if(prevTach > currTach) {
        if(time > 0.1f) {
            time = 0.0f;
        }
        pulseTimes[index] = time;
        time = 0.0f;
        index++;
        index %= 10;
    }
    prevTach = currTach;

    if(prevTach2 > currTach2) {
        if(time2 > 0.1f) {
            time2 = 0.0f;
        }
        pulseTimes2[index2] = time2;
        time2 = 0.0f;
        index2++;
        index2 %= 10;
    }
    prevTach2 = currTach2;

    // Adds up total pulse time and counts num of pulses to calculate true rpm
    numPulses = 0;
    timeTotal = 0;
    for(float t : pulseTimes) {
        if(t > 0.0f) numPulses += 1;
        timeTotal += t;
    }
    trueRpm = (timeTotal != 0.0f) ? (numPulses / timeTotal * 60.0f) : 0.0f;

    numPulses2 = 0;
    timeTotal2 = 0;
    for(float t : pulseTimes2) {
        if(t > 0.0f) numPulses2 += 1;
        timeTotal2 += t;
    }
    trueRpm2 = (timeTotal2 != 0.0f) ? (numPulses2 / timeTotal2 * 60.0f) : 0.0f;

    // Adjusts pwmDutyCycle based on requested rpm compared to true rpm
    pwmTimer += deltaTime;
    if(pwmTimer >= 0.02f) {
        pwmTimer = 0.0f;

        pwmDutyCycle += (rpm > trueRpm) ? 0.01f : -0.01f;
        if(pwmDutyCycle > 1.0f) pwmDutyCycle = 1.0f;
        if(pwmDutyCycle < 0.0f) pwmDutyCycle = 0.0f;

        pwmDutyCycle2 += (rpm > trueRpm2) ? 0.01f : -0.01f;
        if(pwmDutyCycle2 > 1.0f) pwmDutyCycle2 = 1.0f;
        if(pwmDutyCycle2 < 0.0f) pwmDutyCycle2 = 0.0f;

        // update fans based on requested rpm
        TIM3->CCR4 = (uint8_t)(pwmDutyCycle * 255.0f);
        TIM3->CCR1 = (uint8_t)(pwmDutyCycle2 * 255.0f);
    }
}

void fansInit() {
    can_addInbox(VCU_HVC_COOLING, &rpmInbox);
}

void fansPeriodic(float deltaTime) {
    auto rpm = (float) can_readBytes(rpmInbox.data, 0, 0);
    setFanRpm(rpm, deltaTime);
}