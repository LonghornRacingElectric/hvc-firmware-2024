#include "TempMonitoring.h"

/*
 * Temperatures BELOW the lower of the two: MAX cell temp in datasheet OR 60°C.
 * If there are missing or interrupted temperature measurements.
 * */

void TempMonitoring::evaluate(BcbParameters* params, TempMonitoringInput* input,
                              TempMonitoringOutput* output) {

    // Implement a low pass filter to get rid of outlier data points.

    // Check if temperature is in the safe range.
    output->safeTemp = true;


    if(input->temp < params->tempMin || input->temp > params->tempMax)
    {
        output->safeTemp = false;
    }

    // Check that there are no missing measurements.
    if(input->temp == 0) {
        // write logic when there is more info about data from temp sensors
        output->safeTemp = false;
    }

    // If it is outside the temp bounds, trigger shutdown state.
    if(!output->safeTemp)
    {
        // add code.
        return;
    }

}
