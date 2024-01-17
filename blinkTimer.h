#ifndef __BLINK_H__
#define __BLINK_H__

#include "timer.h"

#define QUICK_BLINK_PERIOD 750
#define QUICK_BLINK_DUTY_CYCLE 0.8F

#define BLINK_ARRAY_SIZE 10U

#define SLOW_BLINK_PERIOD 3000
#define SLOW_BLINK_DUTY_CYCLE 0.8F

#define BLINK_OFF 0U
#define BLINK_ON 1U

typedef struct blinkStruct{
    timestamp_ms m_timeStamp;
    timestamp_ms m_period;
    timestamp_ms m_onTime;
    uint8_t m_startingState;
} blinkStruct_t;


#endif /* __BLINK_H__ */