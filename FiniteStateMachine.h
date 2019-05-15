#ifndef _FINITESTATEMACHINE_H_
#define _FINITESTATEMACHINE_H_

#include <string>
#include <vector>
#include "Event.h"
#include "State.h"

namespace FSM {

class FiniteStateMachine : public EventHandler {
    struct Transition {
        Event* m_event;
        State* m_fromState;
        State* m_toState;
    };

public:
    static FiniteStateMachine& Instance();

    void Start();

    inline void RegisterEvent(Event* e) {
        e->SetHandler(this);
    }

    void Handle(Event* e) override;

private:
    FiniteStateMachine();

    FiniteStateMachine(const FiniteStateMachine& fsm) = delete;

    FiniteStateMachine& operator=(const FiniteStateMachine& fsm) = delete;

    ~FiniteStateMachine();

private:
    void AddTransition(Event* e, State* from, State* to);

private:
    State* m_currentState;
    State* m_next_state;
    std::vector<Transition> m_transitions;

    std::vector<State*> m_states;
};

}  // namespace FSM

#endif  // !_FINITESTATEMACHINE_H_