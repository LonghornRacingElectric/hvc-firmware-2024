//
// Created by rolandwang on 11/12/2023.
//

#include "state_machine.h"
#include "contactors.h"
#include "vsense.h"
#include "cells.h"

int updateStateMachine(bool shutdownClosed, bool hvOk, bool chargerPresent) {
    switch(currentState) {
        case STATE_NOT_ENERGIZED:
            setDriveContactor(false);
            setPrechargeContactor(false);
            if(shutdownClosed) {
                if(!chargerPresent) {
                    currentState = STATE_PRECHARGING;
                }
                else {
                    currentState = STATE_CHARGING_PRECHARGING;
                }
            }
            break;

        case STATE_PRECHARGING:
            setPrechargeContactor(true);
            if(!shutdownClosed || !hvOk) {
                verifyVoltage = 0;
                setPrechargeContactor(false);
                setDriveContactor(false);
                currentState = STATE_NOT_ENERGIZED;
            }
            if(getTractiveVoltage() > 0.9f * getPackVoltageFromCells()) {
                verifyVoltage = (verifyVoltage + 1) % 11;
                if(verifyVoltage == 10) {
                    verifyVoltage = 0;
                    setDriveContactor(true);
                    setPrechargeContactor(false);
                    currentState = STATE_ENERGIZED;
                }
            }
            break;

        case STATE_ENERGIZED:
            if(!shutdownClosed || !hvOk) {
                setDriveContactor(false);
                currentState = STATE_NOT_ENERGIZED;
            }
            break;

        case STATE_CHARGING_PRECHARGING:
            setPrechargeContactor(true);
            if(!shutdownClosed || !hvOk || !chargerPresent) {
                verifyVoltage = 0;
                setDriveContactor(false);
                setPrechargeContactor(false);
                currentState = STATE_NOT_ENERGIZED;
            }
            if(getTractiveVoltage() > 0.9f * getPackVoltageFromCells()) {
                verifyVoltage = (verifyVoltage + 1) % 11;
                if(verifyVoltage == 10) {
                    verifyVoltage = 0;
                    setDriveContactor(true);
                    setPrechargeContactor(false);
                    currentState = STATE_ENERGIZED;
                }
            }
            break;

        case STATE_CHARGING:
            if(!shutdownClosed || !hvOk || !chargerPresent) {
                setDriveContactor(false);
                currentState = STATE_NOT_ENERGIZED;
            }
            break;

        default:
            setDriveContactor(false);
            currentState = STATE_NOT_ENERGIZED;
    }
    return currentState;
}
