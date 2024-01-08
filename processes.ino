
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

gameState_t g_configTimeState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &configTimeProcess,
};


static gameObject_t g_gameObject = {
    .m_players = 2,
    .m_playerSelect = 0,
    .m_mode = GAME_MODE_DOWN_COUNT,
    // .m_playerTimer = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},
    // .m_rewardTime = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},
    // .m_delayTime = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U},
};

void initProcess(void){
    // Inits game object
    for(int i = 0; i < g_gameObject.m_players; i++){
        g_gameObject.m_playerTimer[i] = CLOCK_FORMAT_INIT;
        g_gameObject.m_rewardTime[i] = CLOCK_FORMAT_INIT;
        g_gameObject.m_delayTime[i] = CLOCK_FORMAT_INIT;
    }
}

stateStatus_t splashScreenProcess(gameState_t* state, stateStatus_t stateStatus){
    static timestamp_ms timeStamp = getTimeStamp();
    timestamp_ms timeDelta;
    static uint8_t counter = 0;

    switch(stateStatus){
        case STATE_START:
            timeStamp = getTimeStamp();
            counter = 0;
            stateStatus = STATE_RUNNING;
            break;

        case STATE_RUNNING:
            timeDelta = timeSince(timeStamp);
            if(timeDelta >= SPASH_SCREEN_UPDATE_PERIOD_MS){
                timeStamp += SPASH_SCREEN_UPDATE_PERIOD_MS;
                clearDisplay();
                if(counter > 6){
                    stateStatus = STATE_EXIT;
                    state->m_next = &g_numberPlayersState;
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
    static timestamp_ms timeStamp = getTimeStamp();
    timestamp_ms timeDelta;
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;
    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            setDisplay(LEFT_CHAR_0, DISPLAY_P);
            setDisplay(LEFT_CHAR_1, DISPLAY_L);
            setDisplay(LEFT_CHAR_2, DISPLAY_4);
            setDisplay(LEFT_CHAR_3, DISPLAY_5);

            setDisplay(RIGHT_CHAR_3, DISPLAY_2);
            stateStatus = STATE_RUNNING;
            timeStamp = getTimeStamp();
            break;

        case STATE_RUNNING:{
            timeDelta = timeSince(timeStamp);

            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    stateStatus = STATE_EXIT;
                    state->m_next = &g_modeState;
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(--g_gameObject.m_players < MIN_NUMBER_PLAYERS){
                        g_gameObject.m_players = MIN_NUMBER_PLAYERS;
                    }
                    timeStamp = getTimeStamp();
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(++g_gameObject.m_players > MAX_NUMBER_PLAYERS){
                        g_gameObject.m_players = MAX_NUMBER_PLAYERS;
                    }
                    timeStamp = getTimeStamp();
                }
            }

            if(timeDelta >= 700 && timeDelta < 1000){
                setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK);
            }
            else{
                if(timeDelta >= 1000){
                    timeStamp += 1000;
                }
                setDisplay(RIGHT_CHAR_3, intToDisplayDigit(g_gameObject.m_players));
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

stateStatus_t gameModeProcess(gameState_t* state, stateStatus_t stateStatus){
    static timestamp_ms timeStamp = getTimeStamp();
    timestamp_ms timeDelta;
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;

    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            updated = BUTTON_UPDATED;
            stateStatus = STATE_RUNNING;
            timeStamp = getTimeStamp();
            break;

        case STATE_RUNNING:{
            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    // Held, go back a state
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                    }
                    else{
                        state->m_next = &g_configTimeState;
                    }
                    stateStatus = STATE_EXIT;
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(g_gameObject.m_mode == GAME_MODE_UP_COUNT){
                        g_gameObject.m_mode = GAME_MODE_DOWN_COUNT;
                    }
                    else{
                        g_gameObject.m_mode = GAME_MODE_UP_COUNT;
                    }
                    timeStamp = getTimeStamp();
                }
            }

            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(g_gameObject.m_mode == GAME_MODE_UP_COUNT){
                        g_gameObject.m_mode = GAME_MODE_DOWN_COUNT;
                    }
                    else{
                        g_gameObject.m_mode = GAME_MODE_UP_COUNT;
                    }

                    timeStamp = getTimeStamp();
                }
            }

            if(timeSince(timeStamp) > 90000){
                timeStamp = getTimeStamp();
            }
            if(g_gameObject.m_mode == GAME_MODE_DOWN_COUNT){
                setTimeToDisplay(90000 - timeSince(timeStamp), DISPLAY_SIDE_RIGHT);
                setDisplay(LEFT_CHAR_2, DISPLAY_DASH);
                setDisplay(LEFT_CHAR_3, DISPLAY_DASH);
            }
            else{
                setTimeToDisplay(timeSince(timeStamp), DISPLAY_SIDE_RIGHT);
                setDisplay(LEFT_CHAR_2, DISPLAY_BLANK);
                setDisplay(LEFT_CHAR_3, DISPLAY_A);
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

stateStatus_t configTimeProcess(gameState_t* state, stateStatus_t stateStatus){
    static timestamp_ms timeStamp = getTimeStamp();
    timestamp_ms timeDelta;

    static uint8_t digitSelect = 0;
    uint8_t digitValue[4];
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;
    timestamp_ms *timeRef;
    clockFormat_t *clockFormatRef;

    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            digitSelect = 0;
            g_gameObject.m_playerSelect = 0;
            stateStatus = STATE_RUNNING;
            timeStamp = getTimeStamp();
            break;

        case STATE_RUNNING:{
            if(g_gameObject.m_playerSelect == 0){
                setDisplay(LEFT_CHAR_0, DISPLAY_A);
                setDisplay(LEFT_CHAR_1, DISPLAY_L);
                setDisplay(LEFT_CHAR_2, DISPLAY_L);

                clockFormatRef = &g_gameObject.m_playerTimer[0];
            }
            else{
                setDisplay(LEFT_CHAR_0, DISPLAY_BLANK);
                setDisplay(LEFT_CHAR_1, DISPLAY_P);
                setDisplay(LEFT_CHAR_2, intToDisplayDigit(g_gameObject.m_playerSelect));
                clockFormatRef = &g_gameObject.m_playerTimer[g_gameObject.m_playerSelect - 1];
            }

            digitValue[0] = clockFormatRef->m_m / 10; // 10s of minutes
            digitValue[1] = clockFormatRef->m_m % 10;
            digitValue[2] = clockFormatRef->m_s / 10;
            digitValue[3] = clockFormatRef->m_s % 10;

            //setTimeToDisplay(*timeRef, DISPLAY_SIDE_RIGHT);

            // Go back or next state
            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        stateStatus = STATE_EXIT;
                    }
                    else{
                        state->m_next = &g_splashScreenState;
                        stateStatus = STATE_EXIT;
                    }
                }
            }

            // Next digit, or previous player
            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        if(g_gameObject.m_playerSelect <= 1){
                            g_gameObject.m_playerSelect = g_gameObject.m_players;
                        }
                        else{
                            g_gameObject.m_playerSelect--;
                        }
                    }
                    else{
                        if(++digitSelect >= 4){
                            digitSelect = 0;
                        }
                    }
                }
            }

            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        if(++g_gameObject.m_playerSelect > g_gameObject.m_players){
                            g_gameObject.m_playerSelect = 1;
                        }
                    }
                    else{
                        switch(digitSelect){
                            case 0:
                                *timeRef += 3600000;
                                break;

                            case 1:
                                *timeRef += 60000;
                                break;

                            case 2:
                                *timeRef += 10000;
                                break;

                            case 3:
                                *timeRef += 1000;
                                break;

                            default:
                            break;
                        }
                    }
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