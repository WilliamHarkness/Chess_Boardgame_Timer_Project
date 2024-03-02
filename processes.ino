#include "processes.h"

gameState_t g_splashScreenState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &splashScreenProcess,
};

gameState_t g_numberPlayersState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &numberPlayersProcess,
};

gameState_t g_modeState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gameModeProcess,
};

/////////////////// UPCOUNT STATES //////////////////////

gameState_t g_gameUpCountState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gameUpCountProcess,
};

extern gameState_t g_gameOnePlayerUpCountState;
extern gameState_t g_gameTwoPlayerUpCountState;

/////////////////// DOWNCOUNT STATES //////////////////////

extern gameState_t g_gameOnePlayerDownCountState;
extern gameState_t g_gameTwoPlayerDownCountState;

gameState_t g_gameDownCountState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gameDownCountProcess,
};

gameState_t g_configRecursiveState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gameRecursiveProcess,
};

/////////////////// TIME CONFIG STATES //////////////////////

extern gameState_t g_configTimeOnePlayerState;
extern gameState_t g_configTimeTwoPlayerState;

gameState_t g_configTimeState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &configTimeProcess,
};

gameState_t g_configBonusTimeState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &configBonusTimeProcess,
};

gameObject_t g_gameObject = {
    .m_players = 2,
    .m_playerSelect = 0,
    .m_mode = GAME_MODE_DOWN_COUNT,
    // .m_playerTimer = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},
    // .m_rewardTime = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},
};

/////////////////// POKER STATES //////////////////////

extern gameState_t g_gamePokerState;


void initProcess(void){
    // Inits game object
    for(int i = 0; i < MAX_NUMBER_PLAYERS; i++){
        g_gameObject.m_playerTimer[i] = CLOCK_FORMAT_INIT;
        g_gameObject.m_playerMode[i] = GAME_MODE_UP_COUNT;
        g_gameObject.m_playerStoredTime[i] = CLOCK_FORMAT_INIT;
        g_gameObject.m_rewardTime[i] = CLOCK_FORMAT_INIT;
    }
    g_gameObject.m_mode = GAME_MODE_DOWN_COUNT;
    g_gameObject.m_playerSelect = 0;
    g_gameObject.m_players = 2;

}

timestamp_us g_timeStamp = 0U;
static uint8_t g_isPaused = 1;
static clockFormat_t g_clock = CLOCK_FORMAT_INIT;

stateStatus_t splashScreenProcess(gameState_t* state, stateStatus_t stateStatus){
    g_timeStamp = getTimeStamp();
    static uint8_t counter = 0;

    switch(stateStatus){
        case STATE_START:
            initProcess();
            counter = 0;
            stateStatus = STATE_RUNNING;
            resetBlinkTimer(0, SPLASH_BLINK_PERIOD, SPLASH_BLINK_DUTY_CYCLE, BLINK_ON);
            break;

        case STATE_RUNNING:
            if(isBlinkOn(0) == BLINK_OFF){
                clearDisplay();
                if(counter > 6){
                    stateStatus = STATE_EXIT;
                    state->m_next = &g_modeState;
                }
                else{
                    switch(counter++){
                        case 0:
                            setDisplay(LEFT_CHAR_0, DISPLAY_P);
                            setDisplay(LEFT_CHAR_1, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_2, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_0, DISPLAY_P);
                            setDisplay(RIGHT_CHAR_1, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_2, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK);
                            break;

                        case 1:
                            setDisplay(LEFT_CHAR_0, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_1, DISPLAY_L);
                            setDisplay(LEFT_CHAR_2, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_0, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_1, DISPLAY_L);
                            setDisplay(RIGHT_CHAR_2, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK);
                            break;


                        case 2:
                            setDisplay(LEFT_CHAR_0, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_1, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_2, DISPLAY_A);
                            setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_0, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_1, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_2, DISPLAY_A);
                            setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK);
                            break;


                        case 3:
                            setDisplay(LEFT_CHAR_0, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_1, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_2, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_3, DISPLAY_4);
                            setDisplay(RIGHT_CHAR_0, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_1, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_2, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_3, DISPLAY_4);
                            break;

                        case 4:
                            setDisplay(LEFT_CHAR_0, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_1, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_2, DISPLAY_A);
                            setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_0, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_1, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_2, DISPLAY_A);
                            setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK);
                            break;

                        case 5:
                            setDisplay(LEFT_CHAR_0, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_1, DISPLAY_L);
                            setDisplay(LEFT_CHAR_2, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_0, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_1, DISPLAY_L);
                            setDisplay(RIGHT_CHAR_2, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK);
                            break;

                        case 6:
                            setDisplay(LEFT_CHAR_0, DISPLAY_P);
                            setDisplay(LEFT_CHAR_1, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_2, DISPLAY_BLANK);
                            setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_0, DISPLAY_P);
                            setDisplay(RIGHT_CHAR_1, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_2, DISPLAY_BLANK);
                            setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK);
                            break;
                        

                        default:
                            break;
                    }
                }
            }
            break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}

stateStatus_t numberPlayersProcess(gameState_t* state, stateStatus_t stateStatus){
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;
    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();

            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();
            break;

        case STATE_RUNNING:{

            setDisplay(LEFT_CHAR_0, DISPLAY_P);
            setDisplay(LEFT_CHAR_1, DISPLAY_L);
            setDisplay(LEFT_CHAR_2, DISPLAY_5);
            setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                    }
                    else if(g_gameObject.m_mode == GAME_MODE_UP_COUNT){
                        switch(g_gameObject.m_players){
                            case 1:
                                state->m_next = &g_gameOnePlayerUpCountState;
                                break;

                            case 2:
                                state->m_next = &g_gameTwoPlayerUpCountState;
                                break;

                            default:
                                state->m_next = &g_gameUpCountState;
                                break;
                        }
                    }
                    else if((g_gameObject.m_mode == GAME_MODE_DOWN_COUNT) || (g_gameObject.m_mode == GAME_MODE_RECURSIVE)){
                        switch(g_gameObject.m_players){
                            case 1:
                                state->m_next = &g_configTimeOnePlayerState;
                                break;

                            case 2:
                                state->m_next = &g_configTimeTwoPlayerState;
                                break;

                            default:
                                state->m_next = &g_configTimeState;
                                break;
                        }
                    }
                    else{
                        state->m_next = &g_splashScreenState;
                    }

                    stateStatus = STATE_EXIT;
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(--g_gameObject.m_players < MIN_NUMBER_PLAYERS){
                        g_gameObject.m_players = MIN_NUMBER_PLAYERS;
                    }
                    resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(++g_gameObject.m_players > MAX_NUMBER_PLAYERS){
                        g_gameObject.m_players = MAX_NUMBER_PLAYERS;
                    }
                    resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                }
            }
            setDisplay(RIGHT_CHAR_3, intToDisplayDigit(g_gameObject.m_players));
        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}

stateStatus_t gameModeProcess(gameState_t* state, stateStatus_t stateStatus){
    timestamp_us deltaTime;
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;
    // Set clock to 1 minute
    static uint8_t modeSelect = 0;
    const uint8_t modeSize = 4;
    gameMode_t modes[4] = {GAME_MODE_UP_COUNT, GAME_MODE_DOWN_COUNT, GAME_MODE_RECURSIVE, GAME_MODE_POKER};

    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            updated = BUTTON_UPDATED;
            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();
            modeSelect = 0;
            g_clock = CLOCK_FORMAT_INIT;
            break;

        case STATE_RUNNING:{
            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    g_gameObject.m_mode = modes[modeSelect];
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                    }
                    else{
                        switch(g_gameObject.m_mode){
                            case GAME_MODE_UP_COUNT:
                            case GAME_MODE_DOWN_COUNT:
                            case GAME_MODE_RECURSIVE:
                                state->m_next = &g_numberPlayersState;
                                break;

                            case GAME_MODE_POKER:
                            default:
                                state->m_next = &g_gamePokerState;
                                break;
                        }
                    }
                    stateStatus = STATE_EXIT;
                    break;
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(modeSelect == 0){
                        modeSelect = modeSize - 1;
                    }
                    else{
                        modeSelect--;
                    }
                }
            }

            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(++modeSelect >= modeSize){
                        modeSelect = 0;
                    }
                }
            }

            deltaTime = timeSinceSeemless(&g_timeStamp);
            switch(modes[modeSelect]){
                case GAME_MODE_UP_COUNT:{
                    addTimeStamp(&g_clock, deltaTime);
                    clockFormat_t clockRef = (clockFormat_t){CLOCK_NOT_OVERFLOWED, 0U, 30000000};
                    subClocks(&clockRef, &g_clock);
                    if(isTimeOverFlowed(&clockRef) == CLOCK_OVERFLOWED || 
                        isTimeZero(&clockRef)){
                        g_clock = CLOCK_FORMAT_INIT;
                    }
                    setClockToDisplay(&g_clock, getSimplifiedClockType(&g_clock), DISPLAY_SIDE_RIGHT);
                    setDisplay(LEFT_CHAR_0, DISPLAY_A);
                    setDisplay(LEFT_CHAR_1, DISPLAY_0);
                    setDisplay(LEFT_CHAR_2, DISPLAY_0);
                    setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
                    break;
                }

                case GAME_MODE_DOWN_COUNT:
                    subTimeStamp(&g_clock, deltaTime);
                    if(isTimeOverFlowed(&g_clock) == CLOCK_OVERFLOWED || 
                        isTimeZero(&g_clock)){
                        g_clock.m_m = 0;
                        g_clock.m_us = 30000000;
                        g_clock.m_overflow = CLOCK_NOT_OVERFLOWED;
                    }
                    setClockToDisplay(&g_clock, getSimplifiedClockType(&g_clock), DISPLAY_SIDE_RIGHT);
                    setDisplay(LEFT_CHAR_0, DISPLAY_DASH);
                    setDisplay(LEFT_CHAR_1, DISPLAY_DASH);
                    setDisplay(LEFT_CHAR_2, DISPLAY_DASH);
                    setDisplay(LEFT_CHAR_3, DISPLAY_DASH);
                    break;

                case GAME_MODE_RECURSIVE:
                    subTimeStamp(&g_clock, deltaTime);
                    if(isTimeOverFlowed(&g_clock) == CLOCK_OVERFLOWED || 
                        isTimeZero(&g_clock)){
                        g_clock.m_m = 0;
                        g_clock.m_us = 30000000;
                        g_clock.m_overflow = CLOCK_NOT_OVERFLOWED;
                    }
                    setClockToDisplay(&g_clock, getSimplifiedClockType(&g_clock), DISPLAY_SIDE_RIGHT);
                    setDisplay(LEFT_CHAR_0, DISPLAY_L);
                    setDisplay(LEFT_CHAR_1, DISPLAY_0);
                    setDisplay(LEFT_CHAR_2, DISPLAY_0);
                    setDisplay(LEFT_CHAR_3, DISPLAY_P);
                    break;

                case GAME_MODE_POKER:
                    setDisplay(RIGHT_CHAR_0, DISPLAY_P);
                    setDisplay(RIGHT_CHAR_1, DISPLAY_0);
                    setDisplay(RIGHT_CHAR_2, DISPLAY_H);
                    setDisplay(RIGHT_CHAR_3, DISPLAY_3);
                    setDisplay(RIGHT_PUNCT, PUNCT_BLANK);
                    setDisplay(LEFT_CHAR_0, DISPLAY_P);
                    setDisplay(LEFT_CHAR_1, DISPLAY_0);
                    setDisplay(LEFT_CHAR_2, DISPLAY_H);
                    setDisplay(LEFT_CHAR_3, DISPLAY_3);
                    setDisplay(LEFT_PUNCT, PUNCT_BLANK);
                    break;

                default:
                    state->m_next = &g_splashScreenState;
                    break;
            }
        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}

stateStatus_t gameUpCountProcess(gameState_t* state, stateStatus_t stateStatus){
    timestamp_us timeDelta;

    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;

    uint8_t nextPlayer = 0;
    
    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            g_isPaused = 1;
            g_gameObject.m_playerSelect = 0;

            for (int i = 0; i < g_gameObject.m_players; i++){
                g_gameObject.m_playerTimer[i] = CLOCK_FORMAT_INIT;
                g_gameObject.m_playerMode[i] = GAME_MODE_UP_COUNT;
            }

            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();
            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
            resetBlinkTimer(1, SLOW_BLINK_PERIOD, SLOW_BLINK_DUTY_CYCLE, BLINK_ON);
            break;

        case STATE_RUNNING:{
            timeDelta = timeSinceSeemless(&g_timeStamp);
            if(!g_isPaused){
                // Update Timer
                addTimeStamp(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], timeDelta);
            }

            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        stateStatus = STATE_EXIT;
                    }
                    g_isPaused = !g_isPaused;
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(g_isPaused){
                        if(g_gameObject.m_playerSelect == 0){ 
                            g_gameObject.m_playerSelect = g_gameObject.m_players;
                        }
                        g_gameObject.m_playerSelect--;
                    }
                    else{
                        if(++g_gameObject.m_playerSelect >= g_gameObject.m_players){ 
                            g_gameObject.m_playerSelect = 0;
                        }
                    }
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(++g_gameObject.m_playerSelect >= g_gameObject.m_players){ 
                        g_gameObject.m_playerSelect = 0;
                    }
                }
            }

            clockFormat_t *tmpClock;
            // Check Slow Blink
            tmpClock = &g_gameObject.m_playerTimer[g_gameObject.m_playerSelect];
            if(isBlinkOn(1) == BLINK_OFF){
                switch(getSimplifiedClockType(tmpClock)){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
                    default:
                        break;
                }
            }
            else{
                switch(getSimplifiedClockType(tmpClock)){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                    default:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
                        break;

                }
            }
            setDisplay(LEFT_CHAR_0, DISPLAY_BLANK);
            setDisplay(LEFT_CHAR_1, DISPLAY_P);
            setDisplay(LEFT_CHAR_2, intToDisplayDigit(g_gameObject.m_playerSelect) + 1);
            setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
            setDisplay(LEFT_PUNCT, PUNCT_BLANK);
        

        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}

stateStatus_t configTimeProcess(gameState_t* state, stateStatus_t stateStatus){
    timestamp_us timeDelta;

    static uint8_t digitSelect = 2;
    const uint8_t digitSelectSize = 6;
    const digitSelect_t digitToDisplay[6] = {RIGHT_CHAR_0, RIGHT_CHAR_1, RIGHT_CHAR_0, RIGHT_CHAR_1, RIGHT_CHAR_2, RIGHT_CHAR_3};
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;
    
    clockFormat_t *clockFormatRef;

    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            digitSelect = 2;
            g_gameObject.m_playerSelect = 0;
            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();
            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
            break;

        case STATE_RUNNING:{
            setDisplay(LEFT_CHAR_0, DISPLAY_P);
            setDisplay(LEFT_CHAR_1, intToDisplayDigit(g_gameObject.m_playerSelect + 1));
            setDisplay(LEFT_CHAR_2, DISPLAY_BLANK);
            setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
            clockFormatRef = &g_gameObject.m_playerStoredTime[g_gameObject.m_playerSelect];   

            // Hours Config
            if(digitSelect < 2){
                setClockToDisplay(clockFormatRef, CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_RIGHT);
            }
            // Minute Second config
            else{
                setClockToDisplay(clockFormatRef, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
            }

            // Go back or next state
            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        stateStatus = STATE_EXIT;
                    }
                    else{
                        digitSelect = 2;
                        if(++g_gameObject.m_playerSelect >= g_gameObject.m_players){
                            g_gameObject.m_playerSelect = 0;
                            if(g_gameObject.m_mode == GAME_MODE_RECURSIVE){
                                state->m_next = &g_configRecursiveState;
                            }
                            else{
                                state->m_next = &g_configBonusTimeState;
                            }
                            stateStatus = STATE_EXIT;
                            break;
                        }
                    }
                }
            }

            // Next digit, or previous player
            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                    if(++digitSelect >= digitSelectSize){
                        digitSelect = 0;
                    }
                }
            }

            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                    switch(digitSelect){
                        case 0:
                            if((clockFormatRef->m_m / 600) == 9){
                                clockFormatRef->m_m -= 5400;
                            }
                            else{
                                clockFormatRef->m_m += 600;
                            }
                            break;

                        case 1:
                            if(((clockFormatRef->m_m / 60) % 10) == 9){
                                clockFormatRef->m_m -= 540;
                            }
                            else{
                                clockFormatRef->m_m += 60;
                            }
                            break;

                        case 2:
                            if(((clockFormatRef->m_m % 60) / 10) == 5){
                                clockFormatRef->m_m -= 50;
                            }
                            else{
                                clockFormatRef->m_m += 10;
                            }
                            break;

                        case 3:
                            if((clockFormatRef->m_m % 10) == 9){
                                clockFormatRef->m_m -= 9;
                            }
                            else{
                                clockFormatRef->m_m += 1;
                            }
                            break;

                        case 4:
                            if(((clockFormatRef->m_us % 60000000) / 10000000) == 5){
                                clockFormatRef->m_us -= 50000000;
                            }
                            else{
                                clockFormatRef->m_us += 10000000;
                            }
                            break;

                        case 5:
                            if(((clockFormatRef->m_us % 10000000) / 1000000) == 9){
                                clockFormatRef->m_us -= 9000000;
                            }
                            else{
                                clockFormatRef->m_us += 1000000;
                            }
                            break;

                        default:
                        break;
                    }
                }
            }

            if(isBlinkOn(0) == BLINK_ON){
                switch(digitSelect){
                    case 0:
                    case 2:
                        setDisplay(RIGHT_CHAR_0, DISPLAY_BLANK); 
                        break;

                    case 1:
                    case 3:
                        setDisplay(RIGHT_CHAR_1, DISPLAY_BLANK); 
                        break;

                    case 4:
                        setDisplay(RIGHT_CHAR_2, DISPLAY_BLANK); 
                        break;

                    case 5:
                        setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK); 
                        break;

                    default:
                    break;
                }
            }
        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}

stateStatus_t configBonusTimeProcess(gameState_t* state, stateStatus_t stateStatus){
    timestamp_us timeDelta;

    static uint8_t digitSelect = 2;
    const uint8_t digitSelectSize = 6;
    const digitSelect_t digitToDisplay[6] = {RIGHT_CHAR_0, RIGHT_CHAR_1, RIGHT_CHAR_0, RIGHT_CHAR_1, RIGHT_CHAR_2, RIGHT_CHAR_3};
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;
    
    clockFormat_t *clockFormatRef;

    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            digitSelect = 2;
            g_gameObject.m_playerSelect = 0;
            stateStatus = STATE_RUNNING;
            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
            resetBlinkTimer(1, ALT_BLINK_PERIOD, ALT_BLINK_DUTY_CYCLE, BLINK_ON);
            break;

        case STATE_RUNNING:{
            setDisplay(LEFT_CHAR_0, DISPLAY_P);
            setDisplay(LEFT_CHAR_1, intToDisplayDigit(g_gameObject.m_playerSelect + 1));
            setDisplay(LEFT_CHAR_2, DISPLAY_BLANK);
            setDisplay(LEFT_CHAR_3, DISPLAY_A);
            clockFormatRef = &g_gameObject.m_rewardTime[g_gameObject.m_playerSelect];
            

            // Hours Config
            if(digitSelect < 2){
                setClockToDisplay(clockFormatRef, CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_RIGHT);
            }
            // Minute Second config
            else{
                setClockToDisplay(clockFormatRef, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
            }

            // Go back or next state
            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        stateStatus = STATE_EXIT;
                    }
                    else{
                        digitSelect = 2;
                        if(++g_gameObject.m_playerSelect >= g_gameObject.m_players){
                            g_gameObject.m_playerSelect = 0;
                            state->m_next = &g_gameDownCountState;
                            stateStatus = STATE_EXIT;
                            break;
                        }
                    }
                }
            }

            // Next digit, or previous player
            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                    if(++digitSelect >= digitSelectSize){
                        digitSelect = 0;
                    }
                }
            }

            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                    switch(digitSelect){
                        case 0:
                            if((clockFormatRef->m_m / 600) == 9){
                                clockFormatRef->m_m -= 5400;
                            }
                            else{
                                clockFormatRef->m_m += 600;
                            }
                            break;

                        case 1:
                            if(((clockFormatRef->m_m / 60) % 10) == 9){
                                clockFormatRef->m_m -= 540;
                            }
                            else{
                                clockFormatRef->m_m += 60;
                            }
                            break;

                        case 2:
                            if(((clockFormatRef->m_m % 60) / 10) == 5){
                                clockFormatRef->m_m -= 50;
                            }
                            else{
                                clockFormatRef->m_m += 10;
                            }
                            break;

                        case 3:
                            if((clockFormatRef->m_m % 10) == 9){
                                clockFormatRef->m_m -= 9;
                            }
                            else{
                                clockFormatRef->m_m += 1;
                            }
                            break;

                        case 4:
                            if(((clockFormatRef->m_us % 60000000) / 10000000) == 5){
                                clockFormatRef->m_us -= 50000000;
                            }
                            else{
                                clockFormatRef->m_us += 10000000;
                            }
                            break;

                        case 5:
                            if(((clockFormatRef->m_us % 10000000) / 1000000) == 9){
                                clockFormatRef->m_us -= 9000000;
                            }
                            else{
                                clockFormatRef->m_us += 1000000;
                            }
                            break;

                        default:
                        break;
                    }
                }
            }

            if(isBlinkOn(0) == BLINK_ON){
                switch(digitSelect){
                    case 0:
                    case 2:
                        setDisplay(RIGHT_CHAR_0, DISPLAY_BLANK); 
                        break;

                    case 1:
                    case 3:
                        setDisplay(RIGHT_CHAR_1, DISPLAY_BLANK); 
                        break;

                    case 4:
                        setDisplay(RIGHT_CHAR_2, DISPLAY_BLANK); 
                        break;

                    case 5:
                        setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK); 
                        break;

                    default:
                    break;
                }
            }
        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}

stateStatus_t gameDownCountProcess(gameState_t* state, stateStatus_t stateStatus){
    timestamp_us timeDelta;
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;

    uint8_t nextPlayer = 0;
    
    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            g_isPaused = 1;
            g_gameObject.m_playerSelect = 0;

            for (int i = 0; i < g_gameObject.m_players; i++){
                g_gameObject.m_playerTimer[i] = g_gameObject.m_playerStoredTime[i];
                g_gameObject.m_playerMode[i] = GAME_MODE_DOWN_COUNT;
            }

            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();
            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
            resetBlinkTimer(1, SLOW_BLINK_PERIOD, SLOW_BLINK_DUTY_CYCLE, BLINK_ON);
            break;

        case STATE_RUNNING:{
            timeDelta = timeSinceSeemless(&g_timeStamp);
            if(!g_isPaused){
                // Update Timer
                if(g_gameObject.m_playerMode[g_gameObject.m_playerSelect] == GAME_MODE_UP_COUNT){
                    addTimeStamp(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], timeDelta);
                }
                else if(g_gameObject.m_playerMode[g_gameObject.m_playerSelect] == GAME_MODE_DOWN_COUNT){
                    subTimeStamp(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], timeDelta);
                    if(isTimeOverFlowed(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect]) == CLOCK_OVERFLOWED || 
                        isTimeZero(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect])){
                        // Multiplayer don't pause. Start countup instead.
                        setBuzzer();
                        g_gameObject.m_playerMode[g_gameObject.m_playerSelect] = GAME_MODE_UP_COUNT;
                    }
                }
            }

            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        return STATE_EXIT;
                    }
                    g_isPaused = !g_isPaused;
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(g_isPaused){
                        if(g_gameObject.m_playerSelect == 0){ 
                            g_gameObject.m_playerSelect = g_gameObject.m_players;
                        }
                        g_gameObject.m_playerSelect--;
                    }
                    else{
                        if(g_gameObject.m_playerMode[g_gameObject.m_playerSelect] == GAME_MODE_DOWN_COUNT){
                            addClocks(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], &g_gameObject.m_rewardTime[g_gameObject.m_playerSelect]);
                        }
                        if(++g_gameObject.m_playerSelect >= g_gameObject.m_players){ 
                            g_gameObject.m_playerSelect = 0;
                        }
                    }
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(!g_isPaused &&
                        g_gameObject.m_playerMode[g_gameObject.m_playerSelect] == GAME_MODE_DOWN_COUNT){
                        addClocks(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], &g_gameObject.m_rewardTime[g_gameObject.m_playerSelect]);
                    }
                    if(++g_gameObject.m_playerSelect >= g_gameObject.m_players){ 
                        g_gameObject.m_playerSelect = 0;
                    }
                }
            }

            clockFormat_t *tmpClock;
            // Check Slow Blink
            tmpClock = &g_gameObject.m_playerTimer[g_gameObject.m_playerSelect];
            if(isBlinkOn(1) == BLINK_OFF){
                switch(getSimplifiedClockType(tmpClock)){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
                    default:
                        break;
                }
            }
            else{
                switch(getSimplifiedClockType(tmpClock)){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                    default:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
                        break;

                }
            }

            // Overflow Indication
            if(g_gameObject.m_playerMode[g_gameObject.m_playerSelect] == GAME_MODE_UP_COUNT){
                setDisplay(LEFT_CHAR_0, DISPLAY_P);
                setDisplay(LEFT_CHAR_1, intToDisplayDigit(g_gameObject.m_playerSelect) + 1);
                setDisplay(LEFT_CHAR_2, DISPLAY_DASH);
                setDisplay(LEFT_CHAR_3, DISPLAY_L);
                setDisplay(LEFT_PUNCT, PUNCT_BLANK);
            }
            else{
                setDisplay(LEFT_CHAR_0, DISPLAY_BLANK);
                setDisplay(LEFT_CHAR_1, DISPLAY_P);
                setDisplay(LEFT_CHAR_2, intToDisplayDigit(g_gameObject.m_playerSelect) + 1);
                setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
                setDisplay(LEFT_PUNCT, PUNCT_BLANK);
            }        

        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}

stateStatus_t gameRecursiveProcess(gameState_t* state, stateStatus_t stateStatus){
    timestamp_us timeDelta;
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;

    uint8_t nextPlayer = 0;
    
    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            g_isPaused = 1;
            g_gameObject.m_playerSelect = 0;

            for (int i = 0; i < g_gameObject.m_players; i++){
                g_gameObject.m_playerTimer[i] = g_gameObject.m_playerStoredTime[i];
                g_gameObject.m_playerMode[i] = GAME_MODE_DOWN_COUNT;
            }

            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();
            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
            resetBlinkTimer(1, SLOW_BLINK_PERIOD, SLOW_BLINK_DUTY_CYCLE, BLINK_ON);
            break;

        case STATE_RUNNING:{
            timeDelta = timeSinceSeemless(&g_timeStamp);

            if(!g_isPaused){
                // Update Timer
                if(g_gameObject.m_playerMode[g_gameObject.m_playerSelect] == GAME_MODE_UP_COUNT){
                    addTimeStamp(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], timeDelta);
                }
                else if(g_gameObject.m_playerMode[g_gameObject.m_playerSelect] == GAME_MODE_DOWN_COUNT){
                    subTimeStamp(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], timeDelta);
                    if(isTimeOverFlowed(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect]) == CLOCK_OVERFLOWED || 
                        isTimeZero(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect])){
                        // Multiplayer don't pause. Start countup instead.
                        setBuzzer();
                        g_gameObject.m_playerMode[g_gameObject.m_playerSelect] = GAME_MODE_UP_COUNT;
                    }
                }
            }

            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    g_isPaused = !g_isPaused;
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        stateStatus = STATE_EXIT;
                    }
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(g_isPaused){
                        if(g_gameObject.m_playerSelect == 0){ 
                            g_gameObject.m_playerSelect = g_gameObject.m_players - 1;
                        }
                        else{
                            g_gameObject.m_playerSelect--;
                        }
                    }
                    else{
                        g_gameObject.m_playerTimer[g_gameObject.m_playerSelect] = g_gameObject.m_playerStoredTime[g_gameObject.m_playerSelect];
                        g_gameObject.m_playerMode[g_gameObject.m_playerSelect] = GAME_MODE_DOWN_COUNT;
                        if(++g_gameObject.m_playerSelect >= g_gameObject.m_players){ 
                            g_gameObject.m_playerSelect = 0;
                        }
                        g_gameObject.m_playerTimer[g_gameObject.m_playerSelect] = g_gameObject.m_playerStoredTime[g_gameObject.m_playerSelect];
                        g_gameObject.m_playerMode[g_gameObject.m_playerSelect] = GAME_MODE_DOWN_COUNT;
                    }
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(!g_isPaused){
                        g_gameObject.m_playerTimer[g_gameObject.m_playerSelect] = g_gameObject.m_playerStoredTime[g_gameObject.m_playerSelect];
                        g_gameObject.m_playerMode[g_gameObject.m_playerSelect] = GAME_MODE_DOWN_COUNT;
                        if(++g_gameObject.m_playerSelect >= g_gameObject.m_players){ 
                            g_gameObject.m_playerSelect = 0;
                        }
                        g_gameObject.m_playerTimer[g_gameObject.m_playerSelect] = g_gameObject.m_playerStoredTime[g_gameObject.m_playerSelect];
                        g_gameObject.m_playerMode[g_gameObject.m_playerSelect] = GAME_MODE_DOWN_COUNT;
                    }
                    else{
                        if(++g_gameObject.m_playerSelect >= g_gameObject.m_players){ 
                            g_gameObject.m_playerSelect = 0;
                        }
                    }
                }
            }

            clockFormat_t *tmpClock;
            // Check Slow Blink
            tmpClock = &g_gameObject.m_playerTimer[g_gameObject.m_playerSelect];
            if(isBlinkOn(1) == BLINK_OFF){
                switch(getSimplifiedClockType(tmpClock)){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
                    default:
                        break;
                }
            }
            else{
                switch(getSimplifiedClockType(tmpClock)){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                    default:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
                        break;

                }
            }

            setDisplay(LEFT_CHAR_0, DISPLAY_BLANK);
            setDisplay(LEFT_CHAR_1, DISPLAY_P);
            setDisplay(LEFT_CHAR_2, intToDisplayDigit(g_gameObject.m_playerSelect) + 1);
            setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
            setDisplay(LEFT_PUNCT, PUNCT_BLANK);

        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}