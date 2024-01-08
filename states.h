#ifndef __STATES_H__
#define __STATES_H__

typedef enum stateStatus{
    STATE_START,
    STATE_RUNNING,
    STATE_EXIT,
    STATE_ERROR,
} stateStatus_t;

typedef struct gameState {
    struct gameState * m_next;
    struct gameState * m_prev;
    stateStatus_t (*m_process)(struct gameState*, stateStatus_t);
}gameState_t;


stateStatus_t initState(void);

stateStatus_t processState(void);


#endif /* __STATES_H__ */
