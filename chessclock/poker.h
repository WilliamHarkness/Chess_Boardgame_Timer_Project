#ifndef __POKER_H__
#define __POKER_H__

#include "timer.h"
#include "processes.h"

/////////////////////// Macros /////////////////////

#define POKER_STATE_CLOCK_HANDS 0
#define POKER_STATE_TIMER 1
#define POKER_STATE_NUMBER 2

#define POKER_TIMER_DEFAULT_INCREMENT 15000000
#define POKER_TIMER_DEFAULT_INIT {CLOCK_NOT_OVERFLOWED, 0U, POKER_TIMER_DEFAULT_INCREMENT * 2}

/////////////////////// Struct and Enum Definitions /////////////////////


/////////////////////// Function Definitions /////////////////////



#endif /* __POKER_H__ */
