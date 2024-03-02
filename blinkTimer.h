#ifndef __BLINK_H__
#define __BLINK_H__

#include "timer.h"

#define QUICK_BLINK_PERIOD 800000
#define QUICK_BLINK_DUTY_CYCLE 0.15F

#define BLINK_ARRAY_SIZE 10U

#define ALT_BLINK_PERIOD 2000000
#define ALT_BLINK_DUTY_CYCLE 0.5F

#define SLOW_BLINK_PERIOD 2000000
#define SLOW_BLINK_DUTY_CYCLE 0.75F

#define SPLASH_BLINK_PERIOD 250000
#define SPLASH_BLINK_DUTY_CYCLE 1.0F

#define BLINK_OFF 0U
#define BLINK_ON 1U

typedef struct blinkStruct{
    timestamp_us m_timeStamp;
    timestamp_us m_period;
    timestamp_us m_onTime;
    uint8_t m_startingState;
} blinkStruct_t;


#endif /* __BLINK_H__ */