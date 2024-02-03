#ifndef __PROCESSES_1P_H__
#define __PROCESSES_1P_H__

#include "states.h"
#include "timer.h"
#include "processes.h"
#include "buzzer.h"
#include "blinkTimer.h"

stateStatus_t configTimeOnePlayerProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t gameOnePlayerUpCountProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t gameOnePlayerDownCountProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t gameOnePlayerRecursiveProcess(gameState_t* state, stateStatus_t stateStatus);

#endif /* __PROCESSES_1P_H__ */
