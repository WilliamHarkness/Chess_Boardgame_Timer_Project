
#include "display.h"
#include "pinout.h"

#define DISPLAY_SIZE 10
#define DISPLAY_PERIOD_MS 10

typedef struct displayController{
    uint8_t m_isInit;
    uint8_t m_size;
    uint8_t m_selVal;
    uint8_t m_phase;
    timestamp_ms m_timeStamp;
    displayObj_t m_selObjMap[DISPLAY_SIZE];
} displayController_t;

static displayController_t g_dispCont = {
    .m_isInit = 0,
    .m_size = DISPLAY_SIZE,
    .m_selVal = 0,
    .m_phase = 0,
    .m_timeStamp = getTimeStamp(),
    .m_selObjMap = {
        {
            .m_type = TYPE_DIGIT,
            .m_setVal = DISPLAY_BLANK,
            .m_val = DISPLAY_INVALID,
            .m_sel = LEFT_CHAR_0,
        },
        {
            .m_type = TYPE_DIGIT,
            .m_setVal = DISPLAY_BLANK,
            .m_val = DISPLAY_INVALID,
            .m_sel = LEFT_CHAR_1,
        },
        {
            .m_type = TYPE_DIGIT,
            .m_setVal = DISPLAY_BLANK,
            .m_val = DISPLAY_INVALID,
            .m_sel = LEFT_CHAR_2,
        },
        {
            .m_type = TYPE_DIGIT,
            .m_setVal = DISPLAY_BLANK,
            .m_val = DISPLAY_INVALID,
            .m_sel = LEFT_CHAR_3,
        },
        {
            .m_type = TYPE_PUNCT,
            .m_setVal = PUNCT_BLANK,
            .m_val = DISPLAY_INVALID,
            .m_sel = LEFT_PUNCT,
        },
        {
            .m_type = TYPE_DIGIT,
            .m_setVal = DISPLAY_BLANK,
            .m_val = DISPLAY_INVALID,
            .m_sel = RIGHT_CHAR_0,
        },
        {
            .m_type = TYPE_DIGIT,
            .m_setVal = DISPLAY_BLANK,
            .m_val = DISPLAY_INVALID,
            .m_sel = RIGHT_CHAR_1,
        },
        {
            .m_type = TYPE_DIGIT,
            .m_setVal = DISPLAY_BLANK,
            .m_val = DISPLAY_INVALID,
            .m_sel = RIGHT_CHAR_2,
        },
        {
            .m_type = TYPE_DIGIT,
            .m_setVal = DISPLAY_BLANK,
            .m_val = DISPLAY_INVALID,
            .m_sel = RIGHT_CHAR_3,
        },
        {
            .m_type = TYPE_PUNCT,
            .m_setVal = PUNCT_BLANK,
            .m_val = DISPLAY_INVALID,
            .m_sel = RIGHT_PUNCT,
        },
    }
};


displayStatus_t initDisplay(){
    if (g_dispCont.m_isInit == 0){

        g_dispCont.m_phase = 0;
        g_dispCont.m_timeStamp = getTimeStamp();
        pinMode(PHASE_PIN, OUTPUT);
        digitalWrite(PHASE_PIN, g_dispCont.m_phase);

        pinMode(SEL0_PIN, OUTPUT);
        pinMode(SEL1_PIN, OUTPUT);
        pinMode(SEL2_PIN, OUTPUT);
        pinMode(SEL3_PIN, OUTPUT);
        digitalWrite(SEL0_PIN, 0);
        digitalWrite(SEL1_PIN, 0);
        digitalWrite(SEL2_PIN, 0);
        digitalWrite(SEL3_PIN, 0);
        
        pinMode(DATA0_PIN, OUTPUT);
        pinMode(DATA1_PIN, OUTPUT);
        pinMode(DATA2_PIN, OUTPUT);
        pinMode(DATA3_PIN, OUTPUT);
        digitalWrite(DATA0_PIN, 0);
        digitalWrite(DATA1_PIN, 0);
        digitalWrite(DATA2_PIN, 0);
        digitalWrite(DATA3_PIN, 0);

        g_dispCont.m_isInit = 1;
        g_dispCont.m_selVal = 0;
        clearDisplay();
    }
    return DISPLAY_OK;
}

displayStatus_t clearDisplay(){
    if (g_dispCont.m_isInit == 0){
        return DISPLAY_ERROR;
    }

    for(int i = 0; i < DISPLAY_SIZE; i++){
        displayObj_t * obj = &g_dispCont.m_selObjMap[i];
        if ( obj->m_type == TYPE_DIGIT){
            obj->m_setVal = DISPLAY_BLANK;
        }
        else if(obj->m_type == TYPE_PUNCT){
            obj->m_setVal = PUNCT_BLANK;
        }
    }
    
    return DISPLAY_OK;
}

displayStatus_t setDisplay(digitSelect_t sel, displayValue_t val){
    if (g_dispCont.m_isInit == 0){
        return DISPLAY_ERROR;
    }

    if(sel < 0 || sel >= DISPLAY_SIZE){
        return DISPLAY_ERROR;
    }

    g_dispCont.m_selObjMap[sel].m_setVal = val;
    return DISPLAY_OK;
}

displayStatus_t updateDisplay(void){
    if (g_dispCont.m_isInit == 0){
        return DISPLAY_ERROR;
    }
    if(timeSince(g_dispCont.m_timeStamp) >= DISPLAY_PERIOD_MS){
        g_dispCont.m_timeStamp = getTimeStamp();
        g_dispCont.m_phase = !g_dispCont.m_phase;
        digitalWrite(PHASE_PIN, g_dispCont.m_phase);
        
        for(int sel = 0; sel < DISPLAY_SIZE; sel++){
            if (g_dispCont.m_selObjMap[sel].m_val == g_dispCont.m_selObjMap[sel].m_setVal){
                continue;
            }

            uint8_t pinArray[4] = {SEL3_PIN, SEL2_PIN, SEL1_PIN, SEL0_PIN};
            for(uint8_t i = 0;  i < 4; i++){
                uint8_t mask = 0x08 >> i;
                if((sel & mask) != (g_dispCont.m_selVal & mask)){
                    if(sel & mask){
                        g_dispCont.m_selVal = g_dispCont.m_selVal | mask;
                    }
                    else{
                        g_dispCont.m_selVal = g_dispCont.m_selVal & ~(mask);
                    }
                    digitalWrite(pinArray[i], (g_dispCont.m_selVal & mask));

                    if(g_dispCont.m_selVal < DISPLAY_SIZE){
                        displayObj_t * obj = &g_dispCont.m_selObjMap[g_dispCont.m_selVal];
                        digitalWrite(DATA0_PIN, obj->m_setVal & 0x01);
                        digitalWrite(DATA1_PIN, obj->m_setVal & 0x02);
                        digitalWrite(DATA2_PIN, obj->m_setVal & 0x04);
                        digitalWrite(DATA3_PIN, obj->m_setVal & 0x08);
                        obj->m_val = obj->m_setVal;
                    }   
                }
            }
            if(g_dispCont.m_selVal < DISPLAY_SIZE){
                displayObj_t * obj = &g_dispCont.m_selObjMap[g_dispCont.m_selVal];
                digitalWrite(DATA0_PIN, obj->m_setVal & 0x01);
                digitalWrite(DATA1_PIN, obj->m_setVal & 0x02);
                digitalWrite(DATA2_PIN, obj->m_setVal & 0x04);
                digitalWrite(DATA3_PIN, obj->m_setVal & 0x08);
                obj->m_val = obj->m_setVal;
            }   
        }
    }
    return DISPLAY_OK;
}

displayValue_t intToDisplayDigit(uint8_t value){
    if(value <= 9){
        return (displayValue_t)value;
    }
    return DISPLAY_BLANK;
}

clockDisplayType_t getSimplifiedClockType(clockFormat_t* time){
    if(time->m_m > 0){
        if(time->m_m >= 60){
            return CLOCK_DISPLAY_HOURS;
        }
        return CLOCK_DISPLAY_MINUTES;
    }
    return CLOCK_DISPLAY_SECONDS;
}

displayStatus_t setClockToDisplay(clockFormat_t* time, clockDisplayType_t displayType, displaySide_t side){
    digitSelect_t d0;
    digitSelect_t d1;
    digitSelect_t d2;
    digitSelect_t d3;
    digitSelect_t dx;
    if(side == DISPLAY_SIDE_LEFT){
        d0 = LEFT_CHAR_0;
        d1 = LEFT_CHAR_1;
        d2 = LEFT_CHAR_2;
        d3 = LEFT_CHAR_3;
        dx = LEFT_PUNCT;
    }
    else{
        d0 = RIGHT_CHAR_0;
        d1 = RIGHT_CHAR_1;
        d2 = RIGHT_CHAR_2;
        d3 = RIGHT_CHAR_3;
        dx = RIGHT_PUNCT;
    }

    switch(displayType){
        case CLOCK_DISPLAY_HOURS:
                setDisplay(d0, intToDisplayDigit(time->m_m / 600U));
                setDisplay(d1, intToDisplayDigit((time->m_m / 60U) % 10));
                setDisplay(d2, DISPLAY_H);
                setDisplay(d3, DISPLAY_BLANK);
                setDisplay(dx, PUNCT_BLANK);
            break;

        case CLOCK_DISPLAY_MINUTES:
                setDisplay(d0, intToDisplayDigit((time->m_m % 60) / 10));
                setDisplay(d1, intToDisplayDigit(time->m_m % 10U));
                setDisplay(d2, intToDisplayDigit(time->m_ms / 10000U));
                setDisplay(d3, intToDisplayDigit((time->m_ms % 10000U) / 1000U));
                setDisplay(dx, PUNCT_COLON);
            break;

        case CLOCK_DISPLAY_SECONDS:
            setDisplay(d0, intToDisplayDigit(time->m_ms / 10000U));
            setDisplay(d1, intToDisplayDigit((time->m_ms % 10000U) / 1000U));
            setDisplay(d2, intToDisplayDigit((time->m_ms / 100U) % 10U));
            setDisplay(d3, intToDisplayDigit((time->m_ms % 100U) / 10U));
            setDisplay(dx, PUNCT_DOT);
            break;

        default:
            if (time->m_m > 0){

            }
            else{
                setDisplay(d0, intToDisplayDigit(time->m_ms / 10000U));
                setDisplay(d1, intToDisplayDigit((time->m_ms % 10000U) / 1000U));
                setDisplay(d2, intToDisplayDigit((time->m_ms / 100U) % 10U));
                setDisplay(d3, intToDisplayDigit((time->m_ms % 100U) / 10U));
                setDisplay(dx, PUNCT_DOT);
            }
            break;
    }

    uint8_t digitValue[4];


    return DISPLAY_OK;
}