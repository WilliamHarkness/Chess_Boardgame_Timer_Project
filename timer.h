#ifndef __TIMER_H__
#define __TIMER_H__

#define CLOCK_FORMAT_INIT {0, 0}
#define MS_TO_MIN 60000U

typedef unsigned long timestamp_ms;

typedef struct clockFormat {
    timestamp_ms m_m;
    timestamp_ms m_ms;
} clockFormat_t;

void balanceClock(clockFormat_t * clock){
    if(clock->m_ms >= MS_TO_MIN){
        clock->m_m += (clock->m_ms / MS_TO_MIN);
        clock->m_ms = clock->m_ms % MS_TO_MIN;
    }
}

void timeStampToClockFormat(clockFormat_t * clock, timestamp_ms time_ms){
    clock->m_m = (timestamp_ms)(time_ms / MS_TO_MIN);
    clock->m_ms = (timestamp_ms)(time_ms % MS_TO_MIN);
}

void addTimeStamp(clockFormat_t * clock, timestamp_ms timeStamp){
    clock->m_m += timeStamp / MS_TO_MIN;
    clock->m_ms += timeStamp % MS_TO_MIN;
    balanceClock(clock);
}

void subTimeStamp(clockFormat_t * clock, timestamp_ms timeStamp){
    timestamp_ms min = timeStamp / MS_TO_MIN;
    timestamp_ms ms = timeStamp % MS_TO_MIN;

    if(clock->m_ms < ms){
        if(clock->m_m == 0U){
            clock->m_m = 0U;
            clock->m_ms = 0U;
            return;
        }
        clock->m_m--;
        clock->m_ms += MS_TO_MIN - ms; 
    }
    else{
        clock->m_ms -= ms;
    }

    if(clock->m_m < min){
        clock->m_m = 0U;
        clock->m_ms = 0U;
        return;
    }
    else{
        clock->m_m -= min;
    }

    balanceClock(clock);
}

void addClocks(clockFormat_t * BaseClock, clockFormat_t * addValue){
    BaseClock->m_m += addValue->m_m;
    BaseClock->m_ms += addValue->m_ms;
    balanceClock(BaseClock);
}

void subClocks(clockFormat_t * BaseClock, clockFormat_t * subValue){
    if(BaseClock->m_ms < subValue->m_ms){
        if(BaseClock->m_m == 0U){
            BaseClock->m_m = 0U;
            BaseClock->m_ms = 0U;
            return;
        }
        BaseClock->m_m--;
        BaseClock->m_ms += MS_TO_MIN - subValue->m_ms; 
    }

    if(BaseClock->m_m < subValue->m_m){
        BaseClock->m_m = 0U;
        BaseClock->m_ms = 0U;
        return;
    }
    else{
        BaseClock->m_m -= subValue->m_m;
    }

    balanceClock(BaseClock);
}

timestamp_ms getTimeStamp(void){
    return millis();
}

timestamp_ms timeSince(timestamp_ms time){
    return millis() - time;
}

timestamp_ms timeSinceSeemless(timestamp_ms *time){
    timestamp_ms diff = millis() - *time;
    *time += diff;
    return diff;
}

#endif /* __TIMER_H__ */