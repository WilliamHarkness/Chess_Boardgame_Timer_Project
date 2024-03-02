#include "processesOnePlayer.h"

extern gameState_t g_splashScreenState;

/////////////////// UPCOUNT STATES //////////////////////

gameState_t g_gameOnePlayerUpCountState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gameOnePlayerUpCountProcess,
};

/////////////////// DOWNCOUNT STATES //////////////////////

gameState_t g_gameOnePlayerDownCountState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gameOnePlayerDownCountProcess,
};

gameState_t g_gameOnePlayerRecursiveState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gameOnePlayerRecursiveProcess,
};

/////////////////// TIME CONFIG STATES //////////////////////

gameState_t g_configTimeOnePlayerState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &configTimeOnePlayerProcess,
};

extern gameObject_t g_gameObject;

extern timestamp_us g_timeStamp;
extern uint8_t g_isPaused;

stateStatus_t gameOnePlayerUpCountProcess(gameState_t* state, stateStatus_t stateStatus){
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
            g_gameObject.m_playerMode[0] = GAME_MODE_UP_COUNT;
            g_gameObject.m_playerTimer[0] = CLOCK_FORMAT_INIT;
            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();
            break;

        case STATE_RUNNING:{
            timeDelta = timeSinceSeemless(&g_timeStamp);
            if(!g_isPaused){
                // Update Timer
                addTimeStamp(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], timeDelta);
            }

            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(g_isPaused == 0){
                        g_isPaused = 1;
                    }
                    else{
                        g_gameObject.m_playerTimer[0] = CLOCK_FORMAT_INIT;
                    }
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        stateStatus = STATE_EXIT;
                    }
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    g_isPaused = !g_isPaused;
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    g_isPaused = !g_isPaused;
                }
            }
            setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_HOURS_MIN, DISPLAY_SIDE_LEFT);
            setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}

stateStatus_t configTimeOnePlayerProcess(gameState_t* state, stateStatus_t stateStatus){
    timestamp_us timeDelta;

    static uint8_t digitSelect = 2;
    const uint8_t digitSelectSize = 8;
    const digitSelect_t digitToDisplay[8] = {LEFT_CHAR_0, LEFT_CHAR_1, LEFT_CHAR_2, LEFT_CHAR_3, RIGHT_CHAR_0, RIGHT_CHAR_1, RIGHT_CHAR_2, RIGHT_CHAR_3};
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;
    
    clockFormat_t *clockFormatRef = &g_gameObject.m_playerStoredTime[0];

    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            digitSelect = 2;
            stateStatus = STATE_RUNNING;
            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
            break;

        case STATE_RUNNING:{
            setClockToDisplay(clockFormatRef, CLOCK_DISPLAY_HOURS_MIN, DISPLAY_SIDE_LEFT);
            setClockToDisplay(clockFormatRef, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);

            // Go back or next state
            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        stateStatus = STATE_EXIT;
                    }
                    else{
                        if(g_gameObject.m_mode == GAME_MODE_DOWN_COUNT){
                            state->m_next = &g_gameOnePlayerDownCountState;
                        }
                        else if(g_gameObject.m_mode == GAME_MODE_RECURSIVE){
                            state->m_next = &g_gameOnePlayerRecursiveState;
                        }
                        else{
                            state->m_next = &g_splashScreenState;
                        }

                        stateStatus = STATE_EXIT;
                        break;
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

                        case 6:
                            if(((clockFormatRef->m_us % 1000000) / 100000) == 9){
                                clockFormatRef->m_us -= 900000;
                            }
                            else{
                                clockFormatRef->m_us += 100000;
                            }
                            break;

                        case 7:
                            if(((clockFormatRef->m_us % 100000) / 10000) == 9){
                                clockFormatRef->m_us -= 90000;
                            }
                            else{
                                clockFormatRef->m_us += 10000;
                            }
                            break;

                        default:
                        break;
                    }
                }
            }

            if(isBlinkOn(0) == BLINK_ON){
                setDisplay(digitToDisplay[digitSelect], DISPLAY_BLANK);
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

stateStatus_t gameOnePlayerDownCountProcess(gameState_t* state, stateStatus_t stateStatus){
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
            g_gameObject.m_playerTimer[0] = g_gameObject.m_playerStoredTime[0];
            g_gameObject.m_playerMode[0] = GAME_MODE_DOWN_COUNT;
            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();
            break;

        case STATE_RUNNING:{
            timeDelta = timeSinceSeemless(&g_timeStamp);
            if(!g_isPaused){
                // Update Timer
                subTimeStamp(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], timeDelta);
                if(isTimeOverFlowed(&g_gameObject.m_playerTimer[0]) == CLOCK_OVERFLOWED || 
                        isTimeZero(&g_gameObject.m_playerTimer[0])){
                    setBuzzer();
                    g_isPaused = 1;
                    g_gameObject.m_playerTimer[0] = CLOCK_FORMAT_INIT;
                }
            }

            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(g_isPaused == 0){
                        g_isPaused = 1;
                    }
                    else{
                        g_gameObject.m_playerTimer[0] = g_gameObject.m_playerStoredTime[0];
                    }
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        stateStatus = STATE_EXIT;
                    }
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(!isTimeZero(&g_gameObject.m_playerTimer[0])){
                        g_isPaused = !g_isPaused;   
                    }
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(!isTimeZero(&g_gameObject.m_playerTimer[0])){
                        g_isPaused = !g_isPaused;   
                    }
                }
            }
            setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_HOURS_MIN, DISPLAY_SIDE_LEFT);
            setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}

stateStatus_t gameOnePlayerRecursiveProcess(gameState_t* state, stateStatus_t stateStatus){
    timestamp_us timeDelta;

    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;
    static uint8_t isSet = 0;

    uint8_t nextPlayer = 0;
    
    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();

            g_isPaused = 1;
            g_gameObject.m_playerSelect = 0;
            g_gameObject.m_playerTimer[0] = g_gameObject.m_playerStoredTime[0];
            g_gameObject.m_playerMode[0] = GAME_MODE_RECURSIVE;
            stateStatus = STATE_RUNNING;
            isSet = 0;
            g_timeStamp = getTimeStamp();
            break;

        case STATE_RUNNING:{
            timeDelta = timeSinceSeemless(&g_timeStamp);
            if(!g_isPaused){
                // Update Timer
                subTimeStamp(&g_gameObject.m_playerTimer[0], timeDelta);
                isSet = 0;
                if(isTimeOverFlowed(&g_gameObject.m_playerTimer[0]) == CLOCK_OVERFLOWED || 
                        isTimeZero(&g_gameObject.m_playerTimer[0])){
                    setBuzzer();
                    g_isPaused = 1;
                    g_gameObject.m_playerTimer[0] = CLOCK_FORMAT_INIT;
                    
                }
            }

            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        stateStatus = STATE_EXIT;
                    }
                    if(g_isPaused){
                        g_isPaused = 0;
                    }
                    else{
                        g_isPaused = 1;
                    }
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(g_isPaused){
                        if(isSet){
                            g_isPaused = 0;
                        }
                    }
                    g_gameObject.m_playerTimer[0] = g_gameObject.m_playerStoredTime[0];
                    isSet = 1;
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(g_isPaused){
                        if(isSet){
                            g_isPaused = 0;
                        }
                    }
                    g_gameObject.m_playerTimer[0] = g_gameObject.m_playerStoredTime[0];
                    isSet = 1;
                }
            }
            setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_HOURS_MIN, DISPLAY_SIDE_LEFT);
            setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}