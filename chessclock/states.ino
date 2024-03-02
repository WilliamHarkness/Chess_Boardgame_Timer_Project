
#include "processes.h"

///////////////// LOCAL FUNCTIONS ///////////////////////

stateStatus_t defaultProcess(gameState_t* state, stateStatus_t stateStatus){
    if(state == NULL){
        return STATE_ERROR;
    }

    return STATE_RUNNING;
}

///////////////// GLOBAL DEFINITIONS ////////////////////

static gameState_t* g_currentState = NULL;
static stateStatus_t g_status = STATE_START; 

extern gameState_t g_splashScreenState;

//////////////// App Interface ///////////////////////

stateStatus_t initState(void){
    g_currentState = &g_splashScreenState;
    g_status = STATE_START;
}

stateStatus_t processState(void){
    if(g_currentState == NULL){
        g_status = STATE_ERROR;
    }
    if(g_status != STATE_ERROR){
        g_status = g_currentState->m_process(g_currentState, g_status);
        if(STATE_EXIT == g_status){
            if(g_currentState->m_next != NULL){
                if(g_currentState->m_next != g_currentState->m_prev){
                    g_currentState->m_next->m_prev = g_currentState;
                }                
            }
            g_currentState = g_currentState->m_next;
            g_status = STATE_START;
        }
    }
    return g_status;
}