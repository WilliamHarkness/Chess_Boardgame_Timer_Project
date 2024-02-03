#ifndef __TIMER_H__
#define __TIMER_H__

#define CLOCK_OVERFLOWED 1
#define CLOCK_NOT_OVERFLOWED 0
#define CLOCK_FORMAT_INIT {CLOCK_NOT_OVERFLOWED, 0U, 0U}
#define US_TO_MIN 60000000U

typedef unsigned long timestamp_us;

typedef struct clockFormat {
    uint8_t m_overflow;
    timestamp_us m_m;
    timestamp_us m_us;
} clockFormat_t;

void balanceClock(clockFormat_t * clock){
    if(clock->m_us >= US_TO_MIN){
        clock->m_m += (clock->m_us / US_TO_MIN);
        clock->m_us = clock->m_us % US_TO_MIN;
    }
}

void timeStampToClockFormat(clockFormat_t * clock, timestamp_us time_us){
    clock->m_m = (timestamp_us)(time_us / US_TO_MIN);
    clock->m_us = (timestamp_us)(time_us % US_TO_MIN);
}

void addClocks(clockFormat_t * BaseClock, clockFormat_t * addValue){
    BaseClock->m_m += addValue->m_m;
    BaseClock->m_us += addValue->m_us;
    balanceClock(BaseClock);
    if(BaseClock->m_m >= 6000){
        BaseClock->m_m = 5999U;
        BaseClock->m_us = 59990U;
    }
}

void subClocks(clockFormat_t * BaseClock, clockFormat_t * subValue){
    balanceClock(subValue);
    balanceClock(BaseClock);
    if(BaseClock->m_us < subValue->m_us){
        if(BaseClock->m_m == 0U){
            subClocks(subValue, BaseClock);
            *BaseClock = *subValue;
            BaseClock->m_overflow = CLOCK_OVERFLOWED;
            return;
        }
        BaseClock->m_m--;
        BaseClock->m_us += US_TO_MIN - subValue->m_us; 
    }
    else{
        BaseClock->m_us -= subValue->m_us;
    }

    if(BaseClock->m_m < subValue->m_m){
        subClocks(subValue, BaseClock);
        *BaseClock = *subValue;
        BaseClock->m_overflow = CLOCK_OVERFLOWED;
        return;
    }
    else{
        BaseClock->m_m -= subValue->m_m;
    }
    
    balanceClock(BaseClock);
}

void addTimeStamp(clockFormat_t * clock, timestamp_us timeStamp){
    clockFormat_t refClock;
    timeStampToClockFormat(&refClock, timeStamp);
    addClocks(clock, &refClock);
}

void subTimeStamp(clockFormat_t * clock, timestamp_us timeStamp){
    clockFormat_t refClock;
    timeStampToClockFormat(&refClock, timeStamp);
    subClocks(clock, &refClock);
}

timestamp_us getTimeStamp(void){
    return micros();
}

timestamp_us timeSince(timestamp_us time){
  
    return micros() - time;
}

timestamp_us timeSinceSeemless(timestamp_us *time){
    timestamp_us diff = micros() - *time;
    *time += diff;
    return diff;
}

uint8_t isTimeOverFlowed(clockFormat_t *time){
    return time->m_overflow;
}

uint8_t isTimeZero(clockFormat_t *time){
    if(time->m_m == 0 && time->m_us == 0){
        return 1;
    }
    return 0;
}

#endif /* __TIMER_H__ */