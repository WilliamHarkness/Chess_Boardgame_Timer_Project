#include "processes.h"
#include "blinkTimer.h"

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

gameState_t g_gameUpCountState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gameUpCountProcess,
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
};

void initProcess(void){
    // Inits game object
    for(int i = 0; i < g_gameObject.m_players; i++){
        g_gameObject.m_playerTimer[i] = CLOCK_FORMAT_INIT;
        g_gameObject.m_rewardTime[i] = CLOCK_FORMAT_INIT;
    }
}

timestamp_ms g_timeStamp = 0U;

stateStatus_t splashScreenProcess(gameState_t* state, stateStatus_t stateStatus){
    static timestamp_ms timeStamp = getTimeStamp();
    timestamp_ms timeDelta;
    static uint8_t counter = 0;

    switch(stateStatus){
        case STATE_START:
            initProcess();
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
                        if(g_gameObject.m_mode == GAME_MODE_UP_COUNT){
                            state->m_next = &g_gameUpCountState;
                        }
                        else{
                            state->m_next = &g_configTimeState;
                        }
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

            // Set clock to 1 minute
            clockFormat_t clock = CLOCK_FORMAT_INIT;
            clock.m_m = 1;
            clock.m_ms = 30000;
            timestamp_ms dt = timeSince(timeStamp);

            if(dt > 90000U){
                timeStamp = getTimeStamp();
                dt = 0U;
            }
            if(g_gameObject.m_mode == GAME_MODE_DOWN_COUNT){
                subTimeStamp(&clock, dt);
                setClockToDisplay(&clock, getSimplifiedClockType(&clock), DISPLAY_SIDE_RIGHT);
                setDisplay(LEFT_CHAR_2, DISPLAY_DASH);
                setDisplay(LEFT_CHAR_3, DISPLAY_DASH);
            }
            else{
                timeStampToClockFormat(&clock, dt);
                setClockToDisplay(&clock, getSimplifiedClockType(&clock), DISPLAY_SIDE_RIGHT);
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

stateStatus_t gameUpCountProcess(gameState_t* state, stateStatus_t stateStatus){
    static uint8_t isPaused = 0;
    timestamp_ms timeDelta;

    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;

    uint8_t nextPlayer = 0;
    
    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            isPaused = 1;

            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();
            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
            resetBlinkTimer(1, SLOW_BLINK_PERIOD, SLOW_BLINK_DUTY_CYCLE, BLINK_ON);
            break;

        case STATE_RUNNING:{
            timeDelta = timeSinceSeemless(&g_timeStamp);
            if(!isPaused){
                // Update Timer
                addTimeStamp(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], timeDelta);
            }

            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = &g_splashScreenState;
                        return STATE_EXIT;
                    }
                    isPaused = !isPaused;
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(g_gameObject.m_players == 2){
                        if(isPaused){
                            if(g_gameObject.m_playerSelect == 0){
                                g_gameObject.m_playerSelect = g_gameObject.m_players;
                            }
                            g_gameObject.m_playerSelect--;
                        }
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
                    if(g_gameObject.m_players == 2){
                        if(isPaused){
                            if(++g_gameObject.m_playerSelect >= g_gameObject.m_players){
                                g_gameObject.m_playerSelect = 0;
                            }
                        }
                    }
                    else{
                        if(++g_gameObject.m_playerSelect >= g_gameObject.m_players){ 
                            g_gameObject.m_playerSelect = 0;
                        }
                    }
                }
            }

            clockFormat_t *tmpClock;
            if(g_gameObject.m_players == 2){
                // Check Slow Blink
                if(isBlinkOn(1) == BLINK_OFF){
                    setClockToDisplay(&g_gameObject.m_playerTimer[1], getSimplifiedClockType(&g_gameObject.m_playerTimer[1]), DISPLAY_SIDE_RIGHT);
                    setClockToDisplay(&g_gameObject.m_playerTimer[0], getSimplifiedClockType(&g_gameObject.m_playerTimer[0]), DISPLAY_SIDE_LEFT);
                }
                else{
                    tmpClock = &g_gameObject.m_playerTimer[1];
                    switch(getSimplifiedClockType(tmpClock)){
                        case CLOCK_DISPLAY_HOURS:
                        case CLOCK_DISPLAY_MINUTES:
                            setClockToDisplay(tmpClock, CLOCK_DISPLAY_MINUTES, DISPLAY_SIDE_RIGHT);
                            break;

                        case CLOCK_DISPLAY_SECONDS:
                        default:
                            setClockToDisplay(tmpClock, CLOCK_DISPLAY_SECONDS, DISPLAY_SIDE_RIGHT);
                            break;

                    }
                    tmpClock = &g_gameObject.m_playerTimer[0];
                    switch(getSimplifiedClockType(tmpClock)){
                        case CLOCK_DISPLAY_HOURS:
                        case CLOCK_DISPLAY_MINUTES:
                            setClockToDisplay(tmpClock, CLOCK_DISPLAY_MINUTES, DISPLAY_SIDE_LEFT);
                            break;

                        case CLOCK_DISPLAY_SECONDS:
                        default:
                            setClockToDisplay(tmpClock, CLOCK_DISPLAY_SECONDS, DISPLAY_SIDE_LEFT);
                            break;
                    }
                }
                
                if(isPaused){
                    // Check Quick Blink
                    if(isBlinkOn(0) == BLINK_OFF){
                        if(g_gameObject.m_playerSelect == 1){
                            setDisplay(RIGHT_PUNCT, DISPLAY_BLANK);                        
                        }
                        else if(g_gameObject.m_playerSelect == 0){
                            setDisplay(LEFT_PUNCT, DISPLAY_BLANK);
                        }
                    }
                }
            }
            else{
                // Check Slow Blink
                tmpClock = &g_gameObject.m_playerTimer[g_gameObject.m_playerSelect];
                if(isBlinkOn(1) == BLINK_OFF){
                    setClockToDisplay(tmpClock, getSimplifiedClockType(tmpClock), DISPLAY_SIDE_RIGHT);
                }
                else{
                    switch(getSimplifiedClockType(tmpClock)){
                        case CLOCK_DISPLAY_HOURS:
                        case CLOCK_DISPLAY_MINUTES:
                            setClockToDisplay(tmpClock, CLOCK_DISPLAY_MINUTES, DISPLAY_SIDE_RIGHT);
                            break;

                        case CLOCK_DISPLAY_SECONDS:
                        default:
                            setClockToDisplay(tmpClock, CLOCK_DISPLAY_SECONDS, DISPLAY_SIDE_RIGHT);
                            break;

                    }
                }
                setDisplay(LEFT_CHAR_0, DISPLAY_BLANK);
                setDisplay(LEFT_CHAR_1, DISPLAY_P);
                setDisplay(LEFT_CHAR_2, intToDisplayDigit(g_gameObject.m_playerSelect));
                setDisplay(LEFT_CHAR_3, DISPLAY_BLANK);
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
            timeStamp = getTimeStamp();
            timeDelta = timeSince(timeStamp);
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

            // Hours Config
            if(digitSelect < 2){
                setClockToDisplay(clockFormatRef, CLOCK_DISPLAY_HOURS, DISPLAY_SIDE_RIGHT);
            }
            // Minute Second config
            else{
                setClockToDisplay(clockFormatRef, CLOCK_DISPLAY_MINUTES, DISPLAY_SIDE_RIGHT);
            }

            timeDelta = timeSince(timeStamp);
            if(timeDelta >= 700){
                setDisplay(digitToDisplay[digitSelect], DISPLAY_BLANK);
                if(timeDelta >= 1000){
                    timeStamp += 1000; 
                }
            }

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
                        if(++g_gameObject.m_playerSelect > g_gameObject.m_players){
                            g_gameObject.m_playerSelect = 1;
                        }
                    }
                    else{
                        if(++digitSelect >= digitSelectSize){
                            digitSelect = 0;
                        }
                    }
                }
            }

            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
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
                            if(((clockFormatRef->m_ms % 60000) / 10000) == 5){
                                clockFormatRef->m_ms -= 50000;
                            }
                            else{
                                clockFormatRef->m_ms += 10000;
                            }
                            break;

                        case 5:
                            if(((clockFormatRef->m_ms % 10000) / 1000) == 9){
                                clockFormatRef->m_ms -= 9000;
                            }
                            else{
                                clockFormatRef->m_ms += 1000;
                            }
                            break;

                        default:
                        break;
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
