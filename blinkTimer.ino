
#include "blinkTimer.h"

blinkStruct_t g_blinkObject[BLINK_ARRAY_SIZE];

void resetBlinkTimer(uint8_t blinkIndex, timestamp_ms period, float dutyCycle, uint8_t startingState){
    if(blinkIndex < BLINK_ARRAY_SIZE){
        g_blinkObject[blinkIndex].m_timeStamp = getTimeStamp();
        g_blinkObject[blinkIndex].m_period = period;
        g_blinkObject[blinkIndex].m_onTime = period * dutyCycle;
        g_blinkObject[blinkIndex].m_startingState = startingState;
    }
}

int isBlinkOn(uint8_t blinkIndex){

    if(blinkIndex < BLINK_ARRAY_SIZE){
        timestamp_ms delta = timeSince(g_blinkObject[blinkIndex].m_timeStamp);

        if(delta < g_blinkObject[blinkIndex].m_onTime){
            return g_blinkObject[blinkIndex].m_startingState;
        }
        else if (delta >= g_blinkObject[blinkIndex].m_period){
            g_blinkObject[blinkIndex].m_timeStamp += g_blinkObject[blinkIndex].m_period;
        }

        return (g_blinkObject[blinkIndex].m_startingState == BLINK_ON) ? BLINK_OFF : BLINK_ON; 
    }
    return BLINK_OFF;
}
