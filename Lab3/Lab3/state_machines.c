

// State-Centric State Machine

typedef enum{
    STATE_0,
    STATE_1,
    STATE_N
} state_t

state_t state = STATE_0;

while (1){
    // look for event

    switch (state){
        case STATE_0:
            // state 0 action
            if (event){
                // change state and do whatever it does while changing
            }
            break;
        case STATE_1:
            // state 1 action
        default: // unhandled state
            // spit out error in case of bugs?
            break;
    }
}

// State-Centric State Machine with Hidden Transitions

void next_state(void){
    switch (state) {
        case STATE_0:
            // change to next state
            break;
    }
}

while (1){
    // look for event

    switch (state){
        case STATE_0:
            // state 0 action
            if (event){
                // transition action
                next_state();
            }
            break;
        case STATE_1:
            // state 1 action
        default: // unhandled state
            // spit out error in case of bugs?
            break;
    }
}

// Event-Centric State Machine

typedef enum{
    EVENT_0,
    EVENT_1,
    EVENT_N
} event_t

event_t event = EVENT_0;

while (1){
    switch (event)
    case (EVENT_0):
        if (state == STATE_0){
            // do something
            next_state();
        }
        if (state == STATE_1){
            // do something
            next_state();
        }
        break;
}