
#include "poker.h"

gameState_t g_gamePokerState = {
    .m_next = NULL,
    .m_prev = NULL,
    .m_process = &gamePoker,
};

static timestamp_us g_handCount = 0;
static uint8_t g_timerPaused = 0;
static uint8_t g_pokerState;
extern uint8_t g_isPaused;
extern timestamp_us g_timeStamp;
extern gameObject_t g_gameObject;
static uint8_t g_funCounter = 0;

stateStatus_t gamePoker(gameState_t* state, stateStatus_t stateStatus){
    timestamp_us timeDelta;

    buttonState_t btnState = BUTTON_RELEASED;
    buttonState_t prevBtnState = BUTTON_RELEASED;
    buttonUpdated_t updated = BUTTON_STALE;
    clockFormat_t * clock = &g_gameObject.m_playerTimer[0];
    clockFormat_t * timer = &g_gameObject.m_playerTimer[1];
    
    switch(stateStatus){
        case STATE_START:
            clearDisplay();
            clearInputs();
            g_isPaused = 1;
            g_handCount = 0;
            g_pokerState = POKER_STATE_CLOCK_HANDS;
            g_timerPaused = 1;
            *clock = CLOCK_FORMAT_INIT;
            *timer = CLOCK_FORMAT_INIT;
            stateStatus = STATE_RUNNING;
            g_timeStamp = getTimeStamp();
            resetBlinkTimer(0, SLOW_BLINK_PERIOD, SLOW_BLINK_DUTY_CYCLE, BLINK_ON);
            resetBlinkTimer(1, SPLASH_BLINK_PERIOD, SPLASH_BLINK_DUTY_CYCLE, BLINK_ON);
            break;

        case STATE_RUNNING:{
            timeDelta = timeSinceSeemless(&g_timeStamp);
            if(!g_isPaused){
                if(g_pokerState == POKER_STATE_TIMER){
                    if(!g_timerPaused){
                        subTimeStamp(timer, timeDelta);
                        if(isTimeOverFlowed(timer) == CLOCK_OVERFLOWED || 
                            isTimeZero(timer)){
                            setBuzzer();
                            g_timerPaused = 1;
                            *timer = CLOCK_FORMAT_INIT;
                        }
                    }
                }
                addTimeStamp(clock, timeDelta);
            }

            if(getCtrlButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_RELEASED && updated == BUTTON_UPDATED){
                    if(prevBtnState == BUTTON_HELD){
                        if(g_isPaused){
                            state->m_next = state->m_prev;
                            stateStatus = STATE_EXIT;
                            break;
                        }
                        g_isPaused = 1;
                    }
                    else{
                        if(g_isPaused){
                            g_isPaused = 0;
                        }
                        else if(g_pokerState == POKER_STATE_TIMER) {
                            if(isTimeZero(timer) || !g_timerPaused){
                                g_pokerState = POKER_STATE_CLOCK_HANDS;
                                g_timerPaused = 1;
                            }
                            else if(g_timerPaused){
                                g_timerPaused = 0;
                            }
                        }
                        else{
                            g_pokerState = POKER_STATE_TIMER;
                            *timer = POKER_TIMER_DEFAULT_INIT;
                            g_timerPaused = 1;
                            g_funCounter = 0;
                            resetBlinkTimer(1, SPLASH_BLINK_PERIOD, SPLASH_BLINK_DUTY_CYCLE, BLINK_ON);
                        }
                    }
                }
            }

            if(getLeftButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(!g_isPaused){
                        if(g_pokerState == POKER_STATE_TIMER){
                            subTimeStamp(timer, POKER_TIMER_DEFAULT_INCREMENT);
                            if(isTimeOverFlowed(timer) == CLOCK_OVERFLOWED){
                                *timer = CLOCK_FORMAT_INIT;
                            }
                        }
                        else{
                            if(g_handCount != 0){
                                g_handCount--;
                            }
                        }
                    }
                }
            }
            if(getRightButtonState(&btnState, &prevBtnState, &updated) == BUTTON_OK){
                if(btnState == BUTTON_PRESSED && updated == BUTTON_UPDATED){
                    if(!g_isPaused){
                        g_isPaused = 0;
                        if(g_pokerState == POKER_STATE_TIMER){
                            addTimeStamp(timer, POKER_TIMER_DEFAULT_INCREMENT);
                        }
                        else{
                            g_handCount++;
                        }
                    }
                }
            }

            setDisplay(LEFT_CHAR_0, intToDisplayDigit((g_handCount / 1000) % 10)); 
            setDisplay(LEFT_CHAR_1, intToDisplayDigit((g_handCount / 100) % 10)); 
            setDisplay(LEFT_CHAR_2, intToDisplayDigit((g_handCount / 10) % 10)); 
            setDisplay(LEFT_CHAR_3, intToDisplayDigit(g_handCount % 10)); 
            setDisplay(LEFT_PUNCT, PUNCT_BLANK);   

            clockFormat_t * cRef = clock;
            if(g_pokerState == POKER_STATE_TIMER){
                cRef = timer;
                if (isBlinkOn(1) == BLINK_OFF){
                    resetBlinkTimer(1, SPLASH_BLINK_PERIOD, SPLASH_BLINK_DUTY_CYCLE, BLINK_ON);
                    if(++g_funCounter >= 4){
                        g_funCounter = 0;
                    }
                }
                setDisplay(LEFT_CHAR_0, DISPLAY_BLANK); 
                setDisplay(LEFT_CHAR_1, DISPLAY_BLANK); 
                setDisplay(LEFT_CHAR_2, DISPLAY_BLANK); 
                setDisplay(LEFT_CHAR_3, DISPLAY_BLANK); 
                setDisplay(LEFT_PUNCT, PUNCT_BLANK); 
                const digitSelect_t funArray[4] = {LEFT_CHAR_0, LEFT_CHAR_1, LEFT_CHAR_2, LEFT_CHAR_3};
                setDisplay(funArray[g_funCounter], DISPLAY_DASH); 

            }
            else{
                setDisplay(LEFT_CHAR_0, intToDisplayDigit((g_handCount / 1000) % 10)); 
                setDisplay(LEFT_CHAR_1, intToDisplayDigit((g_handCount / 100) % 10)); 
                setDisplay(LEFT_CHAR_2, intToDisplayDigit((g_handCount / 10) % 10)); 
                setDisplay(LEFT_CHAR_3, intToDisplayDigit(g_handCount % 10)); 
                setDisplay(LEFT_PUNCT, PUNCT_BLANK);   
            }
            
            if(isBlinkOn(0) == BLINK_OFF){
                switch(getSimplifiedClockType(cRef)){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                        setClockToDisplay(cRef, CLOCK_DISPLAY_HOURS_ONLY, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(cRef, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                        setClockToDisplay(cRef, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
                    default:
                        break;
                }
            }
            else{
                switch(getSimplifiedClockType(cRef)){
                    case CLOCK_DISPLAY_HOURS_ONLY:
                    case CLOCK_DISPLAY_HOURS_MIN:
                    case CLOCK_DISPLAY_MIN_SEC:
                        setClockToDisplay(cRef, CLOCK_DISPLAY_MIN_SEC, DISPLAY_SIDE_RIGHT);
                        break;

                    case CLOCK_DISPLAY_SEC_MSEC:
                    default:
                        setClockToDisplay(cRef, CLOCK_DISPLAY_SEC_MSEC, DISPLAY_SIDE_RIGHT);
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