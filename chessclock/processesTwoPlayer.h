#ifndef __PROCESSES_2P_H__
#define __PROCESSES_2P_H__

#include "states.h"
#include "timer.h"
#include "processes.h"
#include "buzzer.h"
#include "blinkTimer.h"

stateStatus_t configTimeTwoPlayerProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t configBonusTimeTwoPlayerProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t gameTwoPlayerUpCountProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t gameTwoPlayerDownCountProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t gameTwoPlayerRecursiveProcess(gameState_t* state, stateStatus_t stateStatus);


#endif /* __PROCESSES_2P_H__ */
