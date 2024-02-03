#ifndef __PROCESSES_H__
#define __PROCESSES_H__

#include "states.h"
#include "timer.h"
#include "processesOnePlayer.h"
#include "processesTwoPlayer.h"
#include "blinkTimer.h"
#include "buzzer.h"

#define SPASH_SCREEN_UPDATE_PERIOD_US 250000

#define MIN_NUMBER_PLAYERS 1
#define MAX_NUMBER_PLAYERS 9

typedef enum gameMode{
    GAME_MODE_UP_COUNT = 0,
    GAME_MODE_DOWN_COUNT,
    GAME_MODE_RECURSIVE,
    GAME_MODE_POKER,
} gameMode_t;

typedef struct gameObject {
    uint8_t m_players;
    uint8_t m_playerSelect;
    gameMode_t m_mode;
    clockFormat_t m_playerTimer[MAX_NUMBER_PLAYERS];
    gameMode_t m_playerMode[MAX_NUMBER_PLAYERS];
    clockFormat_t m_playerStoredTime[MAX_NUMBER_PLAYERS];
    clockFormat_t m_rewardTime[MAX_NUMBER_PLAYERS];
} gameObject_t;

void initProcess(void);

stateStatus_t splashScreenProcess(gameState_t* state, stateStatus_t status);

stateStatus_t numberPlayersProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t gameModeProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t configTimeProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t configBonusTimeProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t gameUpCountProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t gameDownCountProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t gameRecursiveProcess(gameState_t* state, stateStatus_t stateStatus);

#endif /* __PROCESSES_H__ */
