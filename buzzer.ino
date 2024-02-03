
#include "buzzer.h"

static uint8_t g_buzzerInit = 0U;
static uint8_t g_isBuzzerOn = 0U;
timestamp_us g_buzzerTimer;
timestamp_us g_buzzerTs;

void initBuzzer(void){
    if(!g_buzzerInit){
        pinMode(BUZZER_PIN, OUTPUT);
        digitalWrite(BUZZER_PIN, LOW);
        //analogWrite(BUZZER_PIN, BUZZER_OFF);

        g_buzzerTs = getTimeStamp();

        g_buzzerInit = 1U;
        g_isBuzzerOn = 0U;
        g_buzzerTs = 0U;
    }
}

void setBuzzer(void){
    if(g_buzzerInit){
        g_isBuzzerOn = 1U;
        g_buzzerTimer = BUZZER_ON_TIME;
    }
}

void updateBuzzer(void){
    if(g_buzzerInit){
        if(g_isBuzzerOn){
            timestamp_us timeDelta = timeSinceSeemless(&g_buzzerTs);
            if(timeDelta < g_buzzerTimer){
                g_buzzerTimer -= timeDelta;
            }  
            else{
                g_buzzerTimer = 0U;
            }
            if(g_buzzerTimer == 0U){
                g_isBuzzerOn = 0U;
                digitalWrite(BUZZER_PIN, LOW);
                return;
            }
            else{
                timeDelta = g_buzzerTimer % BUZZER_PERIOD;
                if(BUZZER_PERIOD_OFF_TIME < timeDelta){
                    digitalWrite(BUZZER_PIN, HIGH);
                }
                else{
                    digitalWrite(BUZZER_PIN, LOW);
                }
            }
        }
        else{
            g_buzzerTs = getTimeStamp();
        }
    }
}
