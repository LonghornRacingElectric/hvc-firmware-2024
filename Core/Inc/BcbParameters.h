#ifndef BCB_FIRMWARE_2024_BCBPARAMETERS_H
#define BCB_FIRMWARE_2024_BCBPARAMETERS_H

/*
 * These are the tunable values and other variables for the BCB.
 */


typedef struct BcbParameters {

    // ======== TEMP PARAMETERS ========
    float tempMin; // temperature when vehicle is off (Celsius)
    float tempMax; // max temperature before shutdown (Celsius)

    // ======== VOLTAGE PARAMETERS ========



} BcbParameters;


#endif //BCB_FIRMWARE_2024_BCBPARAMETERS_H
