
#include "inputs.h"
#include "pinout.h"

static buttonReturnType_t g_buttonInit = BUTTON_ERROR;
static buttonObj_t g_ctrlButton;
static buttonObj_t g_leftButton;
static buttonObj_t g_rightButton;

buttonReturnType_t initInputs(void){
    // Setup inputs
    if(g_buttonInit == BUTTON_ERROR){
        pinMode(TIMER0_PIN, INPUT_PULLUP);
        pinMode(TIMER1_PIN, INPUT_PULLUP);
        pinMode(CTRL_PIN, INPUT_PULLUP);
        g_buttonInit = BUTTON_OK;
        clearInputs();
    }
    return g_buttonInit;
}

buttonReturnType_t processInputs(void){
    if(g_buttonInit == BUTTON_ERROR){
        return BUTTON_ERROR;
    }

    buttonObj_t* obj[3] = {&g_ctrlButton, &g_leftButton, &g_rightButton};
    uint8_t pins[3] = {CTRL_PIN, TIMER0_PIN, TIMER1_PIN};
    timestamp_ms time = getTimeStamp();

    for(int i = 0; i < 3 ; i++){
        // Update Timer
        if(digitalRead(pins[i]) == BUTTON_PRESSED_VALUE){
            if(obj[i]->m_momentaryState != BUTTON_PRESSED){
                obj[i]->m_momentaryState = BUTTON_PRESSED;
                if(obj[i]->m_state == BUTTON_RELEASED){
                    obj[i]->m_timeStampPressed = time;
                }
            }
        }
        else{
            if(obj[i]->m_momentaryState != BUTTON_RELEASED){
                obj[i]->m_momentaryState = BUTTON_RELEASED;
                if(obj[i]->m_state != BUTTON_RELEASED){
                    obj[i]->m_timeStampReleased = time;
                }
            }
        }

        // Check debouce
        if(obj[i]->m_momentaryState == BUTTON_RELEASED){
            if(obj[i]->m_state != BUTTON_RELEASED){
                if(timeSince(obj[i]->m_timeStampReleased) >= DEBOUNCE_TIME_MS){
                    obj[i]->m_prevState = obj[i]->m_state;
                    obj[i]->m_state = BUTTON_RELEASED;
                    obj[i]->m_updated = BUTTON_UPDATED;
                }
            }
        }
        else{
            if(obj[i]->m_state == BUTTON_RELEASED){
                if(timeSince(obj[i]->m_timeStampPressed) >= DEBOUNCE_TIME_MS){
                    obj[i]->m_prevState = obj[i]->m_state;
                    obj[i]->m_state = BUTTON_PRESSED;
                    obj[i]->m_updated = BUTTON_UPDATED;
                }
            }
            else if(obj[i]->m_state == BUTTON_PRESSED){
                if(timeSince(obj[i]->m_timeStampPressed) >= BUTTON_HOLD_TIME_MS){
                    obj[i]->m_prevState = obj[i]->m_state;
                    obj[i]->m_state = BUTTON_HELD;
                    obj[i]->m_updated = BUTTON_UPDATED;
                }
            }
        }
    }

    return BUTTON_OK;
}

buttonReturnType_t clearInputs(void){
    if(g_buttonInit == BUTTON_ERROR){
        return BUTTON_ERROR;
    }
    timestamp_ms time = getTimeStamp();
    g_ctrlButton.m_state = BUTTON_RELEASED;
    g_ctrlButton.m_prevState = g_ctrlButton.m_state;
    g_ctrlButton.m_momentaryState = BUTTON_RELEASED;
    g_ctrlButton.m_updated = BUTTON_STALE;
    g_ctrlButton.m_timeStampPressed = time;
    g_ctrlButton.m_timeStampReleased = time;

    g_leftButton.m_state = BUTTON_RELEASED;
    g_leftButton.m_prevState = g_leftButton.m_state;
    g_leftButton.m_momentaryState = BUTTON_RELEASED;
    g_leftButton.m_updated = BUTTON_STALE;
    g_leftButton.m_timeStampPressed = time;
    g_leftButton.m_timeStampReleased = time;

    g_rightButton.m_state = BUTTON_RELEASED;
    g_rightButton.m_prevState = g_rightButton.m_state;
    g_rightButton.m_momentaryState = BUTTON_RELEASED;
    g_rightButton.m_updated = BUTTON_STALE;
    g_rightButton.m_timeStampPressed = time;
    g_rightButton.m_timeStampReleased = time;
}

buttonReturnType_t getCtrlButtonState(buttonState_t *state, buttonState_t *prevState, buttonUpdated_t* updated){
    if(g_buttonInit == BUTTON_ERROR){
        return BUTTON_ERROR;
    }

    *state = g_ctrlButton.m_state;
    *prevState = g_ctrlButton.m_prevState;
    *updated = g_ctrlButton.m_updated;
    g_ctrlButton.m_updated = BUTTON_STALE;
    return BUTTON_OK;
}

buttonReturnType_t getLeftButtonState(buttonState_t *state, buttonState_t *prevState, buttonUpdated_t* updated){
    if(g_buttonInit == BUTTON_ERROR){
        return BUTTON_ERROR;
    }
    *state = g_leftButton.m_state;
    *prevState = g_leftButton.m_prevState;
    *updated = g_leftButton.m_updated;
    g_leftButton.m_updated = BUTTON_STALE;
    return BUTTON_OK;
}

buttonReturnType_t getRightButtonState(buttonState_t *state, buttonState_t *prevState, buttonUpdated_t* updated){
    if(g_buttonInit == BUTTON_ERROR){
        return BUTTON_ERROR;
    }
    *state = g_rightButton.m_state;
    *prevState = g_rightButton.m_prevState;
    *updated = g_rightButton.m_updated;
    g_rightButton.m_updated = BUTTON_STALE;
    return BUTTON_OK;
}
