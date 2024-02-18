
#ifndef HVC_FIRMWARE_2024_FANS_H
#define HVC_FIRMWARE_2024_FANS_H

/* Variables */
// Main Fan
static float time;
static float pwmTimer = 0.0f;
static float trueRpm;
static float pwmDutyCycle = 0.0f;
static float pulseTimes[10];
static float timeTotal = 0.0f;
static float numPulses = 0.0f;
static int index = 0;
static bool prevTach = false; // true is HIGH, false is LOW
static bool currTach = false;

//Unique Fan
static float time2;
static float trueRpm2;
static float pwmDutyCycle2 = 0.0f;
static float pulseTimes2[10];
static float timeTotal2 = 0.0f;
static float numPulses2 = 0.0f;
static int index2 = 0;
static bool prevTach2 = false; // true is HIGH, false is LOW
static bool currTach2 = false;

/* Functions */
void setFanRpm(float rpm);
void fansInit();
void fansPeriodic(float deltaTime);
#endif //HVC_FIRMWARE_2024_FANS_H
