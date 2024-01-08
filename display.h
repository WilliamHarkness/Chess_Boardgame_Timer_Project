#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "timer.h"
#include "stdint.h"

/////////////////////// Struct and Enum Definitions /////////////////////

typedef enum clockDisplayType{
    CLOCK_DISPLAY_HOURS,
    CLOCK_DISPLAY_MINUTES,
    CLOCK_DISPLAY_SECONDS,
} clockDisplayType_t;

typedef enum displaySelect{
    LEFT_CHAR_0 = 0,
    LEFT_CHAR_1 = 1,
    LEFT_CHAR_2 = 2,
    LEFT_CHAR_3 = 3,
    LEFT_PUNCT = 4,
    RIGHT_CHAR_0 = 5,
    RIGHT_CHAR_1 = 6,
    RIGHT_CHAR_2 = 7,
    RIGHT_CHAR_3 = 8,
    RIGHT_PUNCT = 9,
} digitSelect_t;

typedef enum digitType{
    TYPE_PUNCT = 0,
    TYPE_DIGIT = 1,
} digitType_t;

typedef enum displaySide{
    DISPLAY_SIDE_RIGHT = 0,
    DISPLAY_SIDE_LEFT = 1,
} displaySide_t;

typedef enum displayValue{
    // For digits
    DISPLAY_0 = 0,
    DISPLAY_1 = 1,
    DISPLAY_2 = 2,
    DISPLAY_3 = 3,
    DISPLAY_4 = 4,
    DISPLAY_5 = 5,
    DISPLAY_6 = 6,
    DISPLAY_7 = 7,
    DISPLAY_8 = 8,
    DISPLAY_9 = 9,
    DISPLAY_L = 10,
    DISPLAY_H = 11,
    DISPLAY_P = 12,
    DISPLAY_A = 13,
    DISPLAY_DASH = 14,
    DISPLAY_BLANK = 15,

    // For punctuation
    PUNCT_COLON = 0,
    PUNCT_DOT = 4,
    PUNCT_BLANK = 15,

    DISPLAY_INVALID,
}displayValue_t;

typedef struct displayObj {
    digitType_t m_type;
    displayValue_t m_setVal;
    displayValue_t m_val;
    digitSelect_t m_sel;
}displayObj_t;

typedef enum displayStatus{
    DISPLAY_ERROR = 0,
    DISPLAY_OK = 1,
}displayStatus_t;

/////////////////////// Function Definitions /////////////////////

displayStatus_t initDisplay();

displayStatus_t clearDisplay();

displayStatus_t setDisplay(digitSelect_t sel, displayValue_t val);

displayStatus_t updateDisplay(void);

displayValue_t intToDisplayDigit(uint8_t value);

clockDisplayType_t getSimplifiedClockType(clockFormat_t* time);

displayStatus_t setClockToDisplay(clockFormat_t time, clockDisplayType_t displayType, displaySide_t side);

#endif /* __DISPLAY_H__ */