#ifndef __INPUTS_H__
#define __INPUTS_H__

#include "timer.h"

/////////////////////// Macros /////////////////////

#define DEBOUNCE_TIME_US 25000

#define BUTTON_HOLD_TIME_US 300000

#define BUTTON_PRESSED_VALUE LOW

/////////////////////// Struct and Enum Definitions /////////////////////

typedef enum buttonReturnType {
    BUTTON_OK = 0,
    BUTTON_ERROR,
} buttonReturnType_t;

typedef enum buttonUpdated {
    BUTTON_UPDATED = 0,
    BUTTON_STALE,
} buttonUpdated_t;

typedef enum buttonState {
    BUTTON_PRESSED,
    BUTTON_HELD,
    BUTTON_RELEASED,
} buttonState_t;

typedef struct buttonObj{
    buttonState_t m_state;
    buttonState_t m_prevState;
    buttonState_t m_momentaryState;
    buttonUpdated_t m_updated;
    timestamp_us m_timeStampPressed;
    timestamp_us m_timeStampReleased;
} buttonObj_t;

/////////////////////// Function Definitions /////////////////////


buttonReturnType_t initInputs(void);

buttonReturnType_t processInputs(void);

buttonReturnType_t clearInputs(void);

buttonReturnType_t getLeftButtonState(buttonState_t *state, buttonState_t *prevState, buttonUpdated_t* updated);

buttonReturnType_t getRightButtonState(buttonState_t *state, buttonState_t *prevState, buttonUpdated_t* updated);

buttonReturnType_t getCtrlButtonState(buttonState_t *state, buttonState_t *prevState, buttonUpdated_t* updated);

#endif /* __INPUTS_H__ */
