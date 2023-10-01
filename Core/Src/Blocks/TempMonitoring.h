#ifndef BCB_FIRMWARE_2024_TEMPMONITORING_H
#define BCB_FIRMWARE_2024_TEMPMONITORING_H

#include "../BcbParameters.h"

typedef struct SegmentData {
    float thermistorData[18];
} SegmentData;

typedef struct TempMonitoringInput {
    // Battery has 5 segments, 18 thermistors per segment
    SegmentData segmentData[5];
    float temp;
} TempMonitoringInput;

typedef struct TempMonitoringOutput {
    float tempAvg;
    bool safeTemp;
} TempMonitoringOutput;


class TempMonitoring {
public:
    void setParameters(BcbParameters* params) {};
    void evaluate(BcbParameters* params, TempMonitoringInput* input, TempMonitoringOutput* output);
};


#endif //BCB_FIRMWARE_2024_TEMPMONITORING_H
