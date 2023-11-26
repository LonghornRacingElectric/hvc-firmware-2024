//
// Created by rolandwang on 11/12/2023.
//

#include "vcu.h"
#include "can.h"

static CanRx parameterMailbox;

void vcuInit() {
    can_addMailbox(VCU_HVC_PARAMS, &parameterMailbox);
}

void vcuPeriodic() {
    // TODO implement
}
