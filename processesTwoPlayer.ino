#include "processesTwoPlayer.h"

extern gameState_t g_splashScreenState;

/////////////////// UPCOUNT STATES //////////////////////

gameState_t g_gameTwoPlayerUpCountState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gameTwoPlayerUpCountProcess,
};

/////////////////// DOWNCOUNT STATES //////////////////////

gameState_t g_gameTwoPlayerDownCountState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gameTwoPlayerDownCountProcess,
};

gameState_t g_gameTwoPlayerRecursiveState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gameTwoPlayerRecursiveProcess,
};
/////////////////// TIME CONFIG STATES //////////////////////

gameState_t g_configTimeTwoPlayerState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &configTimeTwoPlayerProcess,
};

gameState_t g_configBonusTimeTwoPlayerState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &configBonusTimeTwoPlayerProcess,
};

extern gameObject_t g_gameObject;

extern timestamp_us g_timeStamp;
extern uint8_t g_isPaused;

stateStatus_t gameTwoPlayerUpCountProcess(gameState_t* state, stateStatus_t stateStatus){
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

            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();

            for (int i = 0; i < g_gameObject.m_players; i++){
                g_gameObject.m_playerTimer[i] = CLOCK_FORMAT_INIT;
                g_gameObject.m_playerMode[i] = GAME_MODE_UP_COUNT;
            }

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
                    // On pause selects itself, else change player
                    if(!g_isPaused){
                        if(g_gameObject.m_playerSelect == 0){ 
                            g_gameObject.m_playerSelect = 1;
                        }
                    }
                    else{
                        if(g_gameObject.m_playerSelect != 0){
                            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                        }
                        g_gameObject.m_playerSelect = 0;
                    }
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    // On pause selects itself, else change player
                    if(!g_isPaused){
                        if(g_gameObject.m_playerSelect == 1){ 
                            g_gameObject.m_playerSelect = 0;
                        }
                    }
                    else{
                        if(g_gameObject.m_playerSelect != 1){
                            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                        }
                        g_gameObject.m_playerSelect = 1;
                    }
                }
            }

            clockFormat_t *tmpClock;

            // Check Slow Blink
            if(isBlinkOn(1) == BLINK_ON){
                switch(getSimplifiedClockType(&g_gameObject.m_playerTimer[0])){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                        setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_LEFT);
                        break;

                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_LEFT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_LEFT);
                    default:
                        break;
                }
                switch(getSimplifiedClockType(&g_gameObject.m_playerTimer[1])){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                        setClockToDisplay(&g_gameObject.m_playerTimer[1], CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[1], CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[1], CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
                    default:
                        break;
                }
            }
            else{
                tmpClock = &g_gameObject.m_playerTimer[1];
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
                tmpClock = &g_gameObject.m_playerTimer[0];
                switch(getSimplifiedClockType(tmpClock)){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_LEFT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                    default:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_LEFT);
                        break;
                }
            }
            
            if(g_isPaused){
                // Check Quick Blink
                if(isBlinkOn(0) == BLINK_ON){
                    if(g_gameObject.m_playerSelect == 1){
                        setDisplay(RIGHT_CHAR_0, DISPLAY_BLANK); 
                        setDisplay(RIGHT_CHAR_1, DISPLAY_BLANK); 
                        setDisplay(RIGHT_CHAR_2, DISPLAY_BLANK); 
                        setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK); 
                        setDisplay(RIGHT_PUNCT, PUNCT_BLANK);                        
                    }
                    else if(g_gameObject.m_playerSelect == 0){
                        setDisplay(LEFT_CHAR_0, DISPLAY_BLANK); 
                        setDisplay(LEFT_CHAR_1, DISPLAY_BLANK); 
                        setDisplay(LEFT_CHAR_2, DISPLAY_BLANK); 
                        setDisplay(LEFT_CHAR_3, DISPLAY_BLANK); 
                        setDisplay(LEFT_PUNCT, PUNCT_BLANK);   
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

stateStatus_t configTimeTwoPlayerProcess(gameState_t* state, stateStatus_t stateStatus){
    timestamp_us timeDelta;

    static uint8_t digitSelect = 2;
    const uint8_t digitSelectSize = 6;
    const digitSelect_t digitToDisplayRight[6] = {RIGHT_CHAR_0, RIGHT_CHAR_1, RIGHT_CHAR_0, RIGHT_CHAR_1, RIGHT_CHAR_2, RIGHT_CHAR_3};
    const digitSelect_t digitToDisplayLeft[6] = {LEFT_CHAR_0, LEFT_CHAR_1, LEFT_CHAR_0, LEFT_CHAR_1, LEFT_CHAR_2, LEFT_CHAR_3};
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;
    
    clockFormat_t *clockLeftRef =  &g_gameObject.m_playerStoredTime[0];
    clockFormat_t *clockRightRef =  &g_gameObject.m_playerStoredTime[1];

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
                                state->m_next = &g_gameTwoPlayerRecursiveState;
                            }
                            else{
                                state->m_next = &g_configBonusTimeTwoPlayerState;
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

            clockFormat_t *clockFormatRef;
            if( g_gameObject.m_playerSelect == 0){
                clockFormatRef = clockLeftRef;

                setDisplay(RIGHT_CHAR_0, DISPLAY_DASH);
                setDisplay(RIGHT_CHAR_1, DISPLAY_DASH);
                setDisplay(RIGHT_CHAR_2, DISPLAY_DASH);
                setDisplay(RIGHT_CHAR_3, DISPLAY_DASH);
                setDisplay(RIGHT_PUNCT, PUNCT_BLANK);

                
                if(digitSelect < 2){
                    setClockToDisplay(clockLeftRef, CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_LEFT);
                }
                else{
                    setClockToDisplay(clockLeftRef, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_LEFT);
                }
                
                if(isBlinkOn(0) == BLINK_ON){
                    setDisplay(digitToDisplayLeft[digitSelect], DISPLAY_BLANK);
                }

            }
            else{
                clockFormatRef = clockRightRef;

                setDisplay(LEFT_CHAR_0, DISPLAY_DASH);
                setDisplay(LEFT_CHAR_1, DISPLAY_DASH);
                setDisplay(LEFT_CHAR_2, DISPLAY_DASH);
                setDisplay(LEFT_CHAR_3, DISPLAY_DASH);
                setDisplay(LEFT_PUNCT, PUNCT_BLANK);

                if(digitSelect < 2){
                    setClockToDisplay(clockRightRef, CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_RIGHT);
                }
                else{
                    setClockToDisplay(clockRightRef, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                }

                if(isBlinkOn(0) == BLINK_ON){
                    setDisplay(digitToDisplayRight[digitSelect], DISPLAY_BLANK);
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
        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}

stateStatus_t configBonusTimeTwoPlayerProcess(gameState_t* state, stateStatus_t stateStatus){
    timestamp_us timeDelta;

    static uint8_t digitSelect = 2;
    const uint8_t digitSelectSize = 6;
    const digitSelect_t digitToDisplayRight[6] = {RIGHT_CHAR_0, RIGHT_CHAR_1, RIGHT_CHAR_0, RIGHT_CHAR_1, RIGHT_CHAR_2, RIGHT_CHAR_3};
    const digitSelect_t digitToDisplayLeft[6] = {LEFT_CHAR_0, LEFT_CHAR_1, LEFT_CHAR_0, LEFT_CHAR_1, LEFT_CHAR_2, LEFT_CHAR_3};
    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;
    
    clockFormat_t *clockLeftRef =  &g_gameObject.m_rewardTime[0];
    clockFormat_t *clockRightRef =  &g_gameObject.m_rewardTime[1];

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
                            state->m_next = &g_gameTwoPlayerDownCountState;
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

            clockFormat_t *clockFormatRef;
            if( g_gameObject.m_playerSelect == 0){
                clockFormatRef = clockLeftRef;

                setDisplay(RIGHT_CHAR_0, DISPLAY_DASH);
                setDisplay(RIGHT_CHAR_1, DISPLAY_DASH);
                setDisplay(RIGHT_CHAR_2, DISPLAY_DASH);
                setDisplay(RIGHT_CHAR_3, DISPLAY_A);
                setDisplay(RIGHT_PUNCT, PUNCT_BLANK);

                
                if(digitSelect < 2){
                    setClockToDisplay(clockLeftRef, CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_LEFT);
                }
                else{
                    setClockToDisplay(clockLeftRef, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_LEFT);
                }
                
                if(isBlinkOn(0) == BLINK_ON){
                    setDisplay(digitToDisplayLeft[digitSelect], DISPLAY_BLANK);
                }

            }
            else{
                clockFormatRef = clockRightRef;

                setDisplay(LEFT_CHAR_0, DISPLAY_DASH);
                setDisplay(LEFT_CHAR_1, DISPLAY_DASH);
                setDisplay(LEFT_CHAR_2, DISPLAY_DASH);
                setDisplay(LEFT_CHAR_3, DISPLAY_A);
                setDisplay(LEFT_PUNCT, PUNCT_BLANK);

                if(digitSelect < 2){
                    setClockToDisplay(clockRightRef, CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_RIGHT);
                }
                else{
                    setClockToDisplay(clockRightRef, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                }

                if(isBlinkOn(0) == BLINK_ON){
                    setDisplay(digitToDisplayRight[digitSelect], DISPLAY_BLANK);
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
        } break;
        
        case STATE_EXIT:
        case STATE_ERROR:
        default:
            stateStatus = STATE_ERROR;
            break;
    }
   return stateStatus;
}

stateStatus_t gameTwoPlayerDownCountProcess(gameState_t* state, stateStatus_t stateStatus){
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

            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();
            for (int i = 0; i < g_gameObject.m_players; i++){
                g_gameObject.m_playerTimer[i] = g_gameObject.m_playerStoredTime[i];
                g_gameObject.m_playerMode[i] = GAME_MODE_DOWN_COUNT;
            }

            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
            resetBlinkTimer(1, SLOW_BLINK_PERIOD, SLOW_BLINK_DUTY_CYCLE, BLINK_ON);
            break;

        case STATE_RUNNING:{
            timeDelta = timeSinceSeemless(&g_timeStamp);
            if(!g_isPaused){
                // Update Timer
                subTimeStamp(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], timeDelta);
                if(isTimeOverFlowed(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect])){
                    setBuzzer();
                    g_isPaused = 1;
                }
            }

            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        stateStatus = STATE_EXIT;
                        break;
                    }
                    g_isPaused = !g_isPaused;
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    // On pause selects itself, else change player
                    if(!g_isPaused){
                        if(g_gameObject.m_playerSelect == 0){ 
                            addClocks(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], &g_gameObject.m_rewardTime[g_gameObject.m_playerSelect]);
                            g_gameObject.m_playerSelect = 1;
                        }
                    }
                    else{
                        if(g_gameObject.m_playerSelect != 0){
                            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                        }
                        g_gameObject.m_playerSelect = 0;
                    }
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    // On pause selects itself, else change player
                    if(!g_isPaused){
                        if(g_gameObject.m_playerSelect == 1){ 
                            addClocks(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], &g_gameObject.m_rewardTime[g_gameObject.m_playerSelect]);
                            g_gameObject.m_playerSelect = 0;
                        }
                    }
                    else{
                        if(g_gameObject.m_playerSelect != 1){
                            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                        }
                        g_gameObject.m_playerSelect = 1;
                    }
                }
            }

            clockFormat_t *tmpClock;

            // Check Slow Blink
            if(isBlinkOn(1) == BLINK_ON){
                switch(getSimplifiedClockType(&g_gameObject.m_playerTimer[0])){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                        setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_LEFT);
                        break;

                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_LEFT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_LEFT);
                    default:
                        break;
                }
                switch(getSimplifiedClockType(&g_gameObject.m_playerTimer[1])){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                        setClockToDisplay(&g_gameObject.m_playerTimer[1], CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[1], CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[1], CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
                    default:
                        break;
                }
            }
            else{
                tmpClock = &g_gameObject.m_playerTimer[1];
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
                tmpClock = &g_gameObject.m_playerTimer[0];
                switch(getSimplifiedClockType(tmpClock)){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_LEFT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                    default:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_LEFT);
                        break;
                }
            }
            
            if(g_isPaused){
                // Check Quick Blink
                if(isBlinkOn(0) == BLINK_ON){
                    if(g_gameObject.m_playerSelect == 1){
                        setDisplay(RIGHT_CHAR_0, DISPLAY_BLANK); 
                        setDisplay(RIGHT_CHAR_1, DISPLAY_BLANK); 
                        setDisplay(RIGHT_CHAR_2, DISPLAY_BLANK); 
                        setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK); 
                        setDisplay(RIGHT_PUNCT, PUNCT_BLANK);                        
                    }
                    else if(g_gameObject.m_playerSelect == 0){
                        setDisplay(LEFT_CHAR_0, DISPLAY_BLANK); 
                        setDisplay(LEFT_CHAR_1, DISPLAY_BLANK); 
                        setDisplay(LEFT_CHAR_2, DISPLAY_BLANK); 
                        setDisplay(LEFT_CHAR_3, DISPLAY_BLANK); 
                        setDisplay(LEFT_PUNCT, PUNCT_BLANK);   
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

stateStatus_t gameTwoPlayerRecursiveProcess(gameState_t* state, stateStatus_t stateStatus){
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
                if(g_gameObject.m_playerMode[g_gameObject.m_playerSelect] == GAME_MODE_DOWN_COUNT){
                    subTimeStamp(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect], timeDelta);
                    if(isTimeOverFlowed(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect]) == CLOCK_OVERFLOWED || 
                        isTimeZero(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect])){
                        // Multiplayer don't pause. Start countup instead.
                        setBuzzer();
                        g_isPaused = 1;
                        g_gameObject.m_playerTimer[g_gameObject.m_playerSelect] = CLOCK_FORMAT_INIT;
                    }
                }
            }

            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(g_isPaused){
                        if(isTimeZero(&g_gameObject.m_playerTimer[g_gameObject.m_playerSelect])){
                            g_gameObject.m_playerMode[g_gameObject.m_playerSelect] = GAME_MODE_DOWN_COUNT;
                            g_gameObject.m_playerTimer[g_gameObject.m_playerSelect] = g_gameObject.m_playerStoredTime[g_gameObject.m_playerSelect];
                        }
                        else{
                            g_isPaused = 0;
                        }
                    }
                    else{
                        g_isPaused = 1;
                    }

                    if(prevBtnState == BUTTON_HELD){
                        state->m_next = state->m_prev;
                        stateStatus = STATE_EXIT;
                    }
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(g_isPaused){
                        if(g_gameObject.m_playerSelect != 0){
                            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                        }
                        g_gameObject.m_playerSelect = 0;
                    }
                    else{
                        if(g_gameObject.m_playerSelect == 0){ 
                            g_gameObject.m_playerMode[g_gameObject.m_playerSelect] = GAME_MODE_DOWN_COUNT;
                            g_gameObject.m_playerTimer[g_gameObject.m_playerSelect] = g_gameObject.m_playerStoredTime[g_gameObject.m_playerSelect];
                            g_gameObject.m_playerSelect = 1;
                            g_gameObject.m_playerMode[g_gameObject.m_playerSelect] = GAME_MODE_DOWN_COUNT;
                            g_gameObject.m_playerTimer[g_gameObject.m_playerSelect] = g_gameObject.m_playerStoredTime[g_gameObject.m_playerSelect];
                        }
                    }
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(g_isPaused){
                        if(g_gameObject.m_playerSelect != 1){
                            resetBlinkTimer(0, QUICK_BLINK_PERIOD, QUICK_BLINK_DUTY_CYCLE, BLINK_ON);
                        }
                        g_gameObject.m_playerSelect = 1;
                    }
                    else{
                        if(g_gameObject.m_playerSelect == 1){
                            g_gameObject.m_playerMode[g_gameObject.m_playerSelect] = GAME_MODE_DOWN_COUNT;
                            g_gameObject.m_playerTimer[g_gameObject.m_playerSelect] = g_gameObject.m_playerStoredTime[g_gameObject.m_playerSelect];
                            g_gameObject.m_playerSelect = 0;
                            g_gameObject.m_playerMode[g_gameObject.m_playerSelect] = GAME_MODE_DOWN_COUNT;
                            g_gameObject.m_playerTimer[g_gameObject.m_playerSelect] = g_gameObject.m_playerStoredTime[g_gameObject.m_playerSelect];
                        }
                    }
                }
            }

            clockFormat_t *tmpClock;
            // Check Slow Blink
            if(isBlinkOn(1) == BLINK_OFF){
                switch(getSimplifiedClockType(&g_gameObject.m_playerTimer[0])){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                        setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_LEFT);
                        break;

                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_LEFT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[0], CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_LEFT);
                    default:
                        break;
                }
                switch(getSimplifiedClockType(&g_gameObject.m_playerTimer[1])){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                        setClockToDisplay(&g_gameObject.m_playerTimer[1], CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[1], CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                        setClockToDisplay(&g_gameObject.m_playerTimer[1], CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
                    default:
                        break;
                }
            }
            else{
                tmpClock = &g_gameObject.m_playerTimer[1];
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
                tmpClock = &g_gameObject.m_playerTimer[0];
                switch(getSimplifiedClockType(tmpClock)){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_LEFT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                    default:
                        setClockToDisplay(tmpClock, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_LEFT);
                        break;
                }
            }

            if(g_isPaused){
                // Check Quick Blink
                if(isBlinkOn(0) == BLINK_ON){
                    if(g_gameObject.m_playerSelect == 1){
                        setDisplay(RIGHT_CHAR_0, DISPLAY_BLANK); 
                        setDisplay(RIGHT_CHAR_1, DISPLAY_BLANK); 
                        setDisplay(RIGHT_CHAR_2, DISPLAY_BLANK); 
                        setDisplay(RIGHT_CHAR_3, DISPLAY_BLANK); 
                        setDisplay(RIGHT_PUNCT, PUNCT_BLANK);                        
                    }
                    else if(g_gameObject.m_playerSelect == 0){
                        setDisplay(LEFT_CHAR_0, DISPLAY_BLANK); 
                        setDisplay(LEFT_CHAR_1, DISPLAY_BLANK); 
                        setDisplay(LEFT_CHAR_2, DISPLAY_BLANK); 
                        setDisplay(LEFT_CHAR_3, DISPLAY_BLANK); 
                        setDisplay(LEFT_PUNCT, PUNCT_BLANK);   
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