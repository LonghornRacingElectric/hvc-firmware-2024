//
// Created by rolandwang on 11/12/2023.
//

#ifndef HVC_FIRMWARE_2024_STATE_MACHINE_H
#define HVC_FIRMWARE_2024_STATE_MACHINE_H

#define STATE_NOT_ENERGIZED 1 // idle mode, if PGood signal -> 2
#define STATE_START_PRECHARGE 2 // sample car voltage, once done -> 3
#define STATE_PRECHARGING 3 // set contactors, once current V > 0.9 sample V -> 4
#define STATE_ENERGIZED 4 // disable pcharge, enable drive contactors, if shutdown -> 1
#define STATE_CHARGING 5

int updateStateMachine(bool hvOk, bool chargerPresent);

#endif //HVC_FIRMWARE_2024_STATE_MACHINE_H
