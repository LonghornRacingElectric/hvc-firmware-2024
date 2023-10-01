#ifndef BCB_FIRMWARE_2024_TEMPMONITORING_H
#define BCB_FIRMWARE_2024_TEMPMONITORING_H

typedef struct TempMonitoringInput {
    float temp1; // temp sensor 1
    float temp2; // temp sensor 2
} TempMonitoringInput;

typedef struct TempMonitoringOutput {
    float tempAvg;
    bool safeTemp;
} TempMonitoringOutput;


class TempMonitoring {
public:
    void evaluate(TempMonitoringInput* input, TempMonitoringOutput* output);

};


#endif //BCB_FIRMWARE_2024_TEMPMONITORING_H
