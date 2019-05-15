#ifndef _PLAYSTATE_H_
#define _PLAYSTATE_H_

#include <string>
#include "Environment.h"
#include "State.h"

namespace FSM {

class UpDownInterpolator : public Interpolator {
public:
    UpDownInterpolator() : m_step(1) {}

    void Interpolate(Drawable* drawable) override {
        if (drawable->GetTop() < 100) {
            m_step = 1;
        }
        if (drawable->GetTop() > 500) {
            m_step = -1;
        }
        drawable->GetTop() += m_step;
    }

private:
    float m_step;
};

class MoveLeftInterpolator : public Interpolator {
public:
    void Interpolate(Drawable* drawable) override {
        drawable->GetLeft() -= 2;
    }
};

class PlayState : public State {
public:
    PlayState() : State("Play State"), m_stop(false) {
        m_up_down_interpolator = new UpDownInterpolator();
        m_move_left_interpolator = new MoveLeftInterpolator();
    }

    ~PlayState() {
        delete m_up_down_interpolator;
        delete m_move_left_interpolator;
    }

    void Enter() override;

    void Run() override;

    void Stop() override;

    void Exit() override;

    bool CheckCollision();

    bool MissionCompleted();

private:
    bool m_redraw;
    bool m_stop;
    int m_score;
    Drawable* m_bird;
    Drawable* m_background;
    std::vector<Drawable*> m_objects;
    Interpolator* m_move_left_interpolator;
    Interpolator* m_up_down_interpolator;
};
}  // namespace FSM

#endif  // !_PLAYSTATE_H_
