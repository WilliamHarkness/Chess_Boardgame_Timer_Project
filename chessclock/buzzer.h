#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "timer.h"

#define BUZZER_ON 4U
#define BUZZER_OFF 0U

#define BUZZER_ON_TIME 1000000 // us
#define BUZZER_PERIOD 200000 // us
#define BUZZER_PERIOD_OFF_TIME 165000

void initBuzzer(void);

void setBuzzer(void);

void updateBuzzer(void);

#endif /* __BUZZER_H__ */
