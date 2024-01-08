#ifndef __MENU_PROCESS_H__
#define __MENU_PROCESS_H__

#include "states.h"
#include "timer.h"

#define SPASH_SCREEN_UPDATE_PERIOD_MS 250

#define MIN_NUMBER_PLAYERS 1
#define MAX_NUMBER_PLAYERS 9

typedef enum gameMode{
    GAME_MODE_UP_COUNT = 0,
    GAME_MODE_DOWN_COUNT,
} gameMode_t;

typedef struct gameObject {
    uint8_t m_players;
    uint8_t m_playerSelect;
    gameMode_t m_mode;
    clockFormat_t m_playerTimer[MAX_NUMBER_PLAYERS];
    clockFormat_t m_rewardTime[MAX_NUMBER_PLAYERS];
    clockFormat_t m_delayTime[MAX_NUMBER_PLAYERS];
} gameObject_t;

void initProcess(void);

stateStatus_t splashScreenProcess(gameState_t* state, stateStatus_t status);

stateStatus_t numberPlayersProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t gameModeProcess(gameState_t* state, stateStatus_t stateStatus);

stateStatus_t configTimeProcess(gameState_t* state, stateStatus_t stateStatus);

#endif /* __MENU_PROCESS_H__ */
