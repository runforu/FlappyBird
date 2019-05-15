#include "Event.h"
#include "FiniteStateMachine.h"
#include "UniqueIntGenerator.h"

namespace FSM {

void Event::Trigger() {
    if (m_handler != NULL) {
        m_handler->Handle(this);
    }
}

int Event::GetCode() {
    return m_code;
}

std::string& Event::GetDescription() {
    return m_description;
}

void Event::SetHandler(EventHandler* eh) {
    m_handler = eh;
}

Event::Event(const std::string des) : m_description(des), m_code(UniqueIntGenerator::NextInt()) {}

ExitEvent& ExitEvent::Instance() {
    static ExitEvent __instance;
    return __instance;
}

PlayEvent& FSM::PlayEvent::Instance() {
    static PlayEvent __instance;
    return __instance;
}

BackToMenuEvent& BackToMenuEvent::Instance() {
    static BackToMenuEvent __instance;
    return __instance;
}

GoToScoreEvent& GoToScoreEvent::Instance() {
    static GoToScoreEvent __instance;
    return __instance;
}

DeadEvent& DeadEvent::Instance() {
    static DeadEvent __instance;
    return __instance;
}

RestartEvent& RestartEvent::Instance() {
    static RestartEvent __instance;
    return __instance;
}

}  // namespace FSM