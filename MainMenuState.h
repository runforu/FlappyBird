#ifndef _MAINMENUSTATE_H_
#define _MAINMENUSTATE_H_

#include <string>
#include "Environment.h"
#include "State.h"

namespace FSM {

class MainMenuState : public State {
public:
    MainMenuState() : State("MainMenu State"), m_stop(false) {}

    void Enter() override;

    void Run() override;

    void Stop() override {
        m_stop = true;
    }

    void Exit() override;

private:
    bool m_stop;
};
}  // namespace FSM

#endif  // !_MAINMENUSTATE_H_
