#ifndef _SCORESTATE_H_
#define _SCORESTATE_H_

#include <string>
#include "Environment.h"
#include "State.h"

namespace FSM {

class ScoreState : public State {
public:
    ScoreState() : State("Score State"), m_stop(false) {}

    void Enter() override;

    void Run() override;

    void Stop() override {
        m_stop = true;
    }

    void Exit() override {}

private:
    static const int kMaxScoreRecord = 10;
    bool m_stop;

};
}  // namespace FSM

#endif  // !_SCORESTATE_H_
