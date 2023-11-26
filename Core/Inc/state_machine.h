//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_STATE_MACHINE_H
#define HVC_FIRMWARE_2024_STATE_MACHINE_H

#define STATE_NOT_ENERGIZED 1
#define STATE_PRECHARGING 2
#define STATE_ENERGIZED 3
#define STATE_CHARGING 4

int updateStateMachine(bool hvOk, bool chargerPresent);

#endif //HVC_FIRMWARE_2024_STATE_MACHINE_H
