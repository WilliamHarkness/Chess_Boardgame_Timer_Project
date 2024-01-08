#ifndef __TIMER_H__
#define __TIMER_H__

#define CLOCK_FORMAT_INIT {0, 0, 0, 0}

typedef unsigned long timestamp_ms;

typedef struct clockFormat {
    int16_t m_h;
    int16_t m_m;
    int16_t m_s;
    int16_t m_ms;
} clockFormat_t;

void timeStampToClockFormat(clockFormat_t * clock, timestamp_ms time_ms){
    clock->m_h = time_ms / 3600000;
    clock->m_m = (time_ms / 60000) % 60;
    clock->m_s = (time_ms / 1000) % 60;
    clock->m_ms = time_ms % 1000;
}

void AddClocks(clockFormat_t * BaseClock, clockFormat_t * AddedValue){
    BaseClock->m_h += AddedValue->m_h;
    BaseClock->m_m += AddedValue->m_m;
    BaseClock->m_s += AddedValue->m_s;
    BaseClock->m_ms += AddedValue->m_ms;
    balanceClock(BaseClock);
}

void balanceClock(clockFormat_t * clock){
    clock->m_s += (clock->m_ms / 1000);
    clock->m_m += (clock->m_s / 60);
    clock->m_h += (clock->m_m / 60);
}

timestamp_ms getTimeStamp(void){
    return millis();
}

timestamp_ms timeSince(timestamp_ms time){
    return millis() - time;
}

#endif /* __TIMER_H__ */