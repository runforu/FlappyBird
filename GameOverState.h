#ifndef _GAMEOVERSTATE_H_
#define _GAMEOVERSTATE_H_

#include <string>
#include "Environment.h"
#include "State.h"

namespace FSM {

class GameOverState : public State {
public:
    GameOverState() : State("Score State"), m_stop(false) {}

    void Enter() override;

    void Run() override;

    void Stop() override {
        m_stop = true;
    }

    void Exit() override {}

private:
    static const int kMaxScoreRecord = 7;
    bool m_stop;

private:
    int GetNearScore(ScoreRecord (&array)[kMaxScoreRecord], int* rank, int* start_num);
};
}  // namespace FSM

#endif  // !_GAMEOVERSTATE_H_
