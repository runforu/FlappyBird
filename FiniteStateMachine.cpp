#include <functional>
#include <iostream>
#include <process.h>
#include <time.h>
#include <windows.h>
#include "Event.h"
#include "FiniteStateMachine.h"
#include "GameOverState.h"
#include "Loger.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "ScoreState.h"
#include "State.h"

namespace FSM {

FiniteStateMachine& FiniteStateMachine::Instance() {
    static FiniteStateMachine __instance;
    return __instance;
}

void FiniteStateMachine::Start() {
    while (m_currentState != nullptr) {
        LOG("Entering state: (%s)", m_currentState->GetDescription().c_str());
        m_currentState->Enter();
        m_currentState->Run();
        m_currentState->Exit();
        m_currentState = m_next_state;
    }
}

FiniteStateMachine::~FiniteStateMachine() {
    for (auto it = m_states.begin(); it != m_states.end(); ++it) {
        delete *it;
    }
    m_states.clear();
}

void FiniteStateMachine::AddTransition(Event* e, State* from, State* to) {
    for (auto it = m_transitions.begin(); it != m_transitions.end(); it++) {
        if (e->GetCode() == (*it).m_event->GetCode() && from->GetCode() == (*it).m_fromState->GetCode() &&
            to->GetCode() == (*it).m_toState->GetCode()) {
            // already exits
            return;
        }
    }
    Transition t;
    t.m_event = e;
    t.m_fromState = from;
    t.m_toState = to;
    m_transitions.push_back(t);
}

void FiniteStateMachine::Handle(Event* e) {
    LOG("event: (%s) triggered on state: (%s)", e->GetDescription().c_str(), m_currentState->GetDescription().c_str());
    for (auto it = m_transitions.begin(); it != m_transitions.end(); it++) {
        if (e->GetCode() == (*it).m_event->GetCode() && m_currentState->GetCode() == (*it).m_fromState->GetCode()) {
            // check if a valid transition
            m_currentState->Stop();
            m_next_state = (*it).m_toState;
            return;
        }
    }
    LOG("Unexpected event");
}

inline FiniteStateMachine::FiniteStateMachine() {
    State* main_menu = new MainMenuState();
    if (main_menu != nullptr) {
        m_states.push_back(main_menu);
    }
    m_currentState = main_menu;

    State* play_state = new PlayState();
    if (play_state != nullptr) {
        m_states.push_back(play_state);
    }

    State* score_state = new ScoreState();
    if (score_state != nullptr) {
        m_states.push_back(score_state);
    }

    State* game_over_state = new GameOverState();
    if (game_over_state != nullptr) {
        m_states.push_back(game_over_state);
    }

    Event& exit_event = ExitEvent::Instance();
    RegisterEvent(&exit_event);
    Event& play_event = PlayEvent::Instance();
    RegisterEvent(&play_event);
    Event& back_to_menu_event = BackToMenuEvent::Instance();
    RegisterEvent(&back_to_menu_event);
    Event& go_to_score_event = GoToScoreEvent::Instance();
    RegisterEvent(&go_to_score_event);
    Event& dead_event = DeadEvent::Instance();
    RegisterEvent(&dead_event);

    AddTransition(&exit_event, main_menu, nullptr);
    AddTransition(&exit_event, score_state, nullptr);
    AddTransition(&exit_event, play_state, nullptr);
    AddTransition(&exit_event, game_over_state, nullptr);

    AddTransition(&play_event, main_menu, play_state);
    AddTransition(&go_to_score_event, main_menu, score_state);

    AddTransition(&back_to_menu_event, score_state, main_menu);

    AddTransition(&back_to_menu_event, play_state, main_menu);
    AddTransition(&dead_event, play_state, game_over_state);

    AddTransition(&back_to_menu_event, game_over_state, main_menu);
    AddTransition(&play_event, game_over_state, play_state);
}

}  // namespace FSM
