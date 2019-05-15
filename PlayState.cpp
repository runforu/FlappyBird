#include <iostream>
#include <stdlib.h>
#include "FiniteStateMachine.h"
#include "PlayState.h"

namespace FSM {

void PlayState::Enter() {
    m_background = new Object("res/background.png");
    m_background->Draw();

    m_bird = new Spirit("res/bird*.png");
    m_bird->GetLeft() = Environment::GetScreenW() / 2.0 - m_bird->GetWidth() / 2.0;
    m_bird->GetTop() = Environment::GetScreenH() / 2.0 - m_bird->GetHeight() / 2.0;

    Drawable* obj = nullptr;

    for (int i = 0; i < 20; i++) {
        obj = new Object("res/pillar1.png");
        obj->GetLeft() = Environment::GetScreenW() * (1 + i / 2.5f) + rand() % 50;
        obj->GetTop() = Environment::GetScreenH() - obj->GetHeight() + rand() % 200 + 200;
        m_objects.push_back(obj);
        obj->AddInterpolator(m_move_left_interpolator);

        obj = new Object("res/pillar.png");
        obj->GetLeft() = Environment::GetScreenW() * (1 + i / 2.5f) + rand() % 50;
        obj->GetTop() = -rand() % 200 - 200;
        m_objects.push_back(obj);
        obj->AddInterpolator(m_move_left_interpolator);
    }

    for (int i = 0; i < 20; i++) {
        obj = new Spirit("res/helicopter*.png");
        obj->GetLeft() = Environment::GetScreenW() * (1 + i / 2.5f) + 250;
        obj->GetTop() = 150 + rand() % 300;
        m_objects.push_back(obj);
        obj->AddInterpolator(m_move_left_interpolator);
        obj->AddInterpolator(m_up_down_interpolator);
    }

    m_stop = false;
    m_redraw = false;
    m_score = 0;
}

void PlayState::Run() {
    enum MYKEYS { KEY_UP, KEY_DOWN };
    bool key[4] = {false, false, false, false};

    Environment& env = Environment::Instance();

    if (!env) {
        ExitEvent::Instance().Trigger();
        return;
    }

    al_start_timer(env.GetTimer());

    al_flip_display();
    while (!m_stop) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(env.GetEventQueue(), &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            m_score++;

            m_bird->GetTop() += 2.0;
            if (key[KEY_UP]) {
                m_bird->GetTop() -= 4.0;
            }

            if (key[KEY_DOWN]) {
                m_bird->GetTop() += 4.0;
            }

            // check collision
            if (CheckCollision() || MissionCompleted()) {
                char buf[16];
                sprintf_s(buf, "%d", m_score);
                ScoreRecord record = {"", buf};
                env.NewScore(record);
                DeadEvent::Instance().Trigger();
            }
            m_redraw = true;
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            ExitEvent::Instance().Trigger();
            break;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = true;
                    break;

                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = true;
                    break;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = false;
                    break;

                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = false;
                    break;

                case ALLEGRO_KEY_ESCAPE:
                    // triger exit event
                    ExitEvent::Instance().Trigger();
                    break;

                case ALLEGRO_KEY_BACKSPACE:
                    // triger back to menu event
                    BackToMenuEvent::Instance().Trigger();
                    break;
            }
        }

        if (m_redraw && al_is_event_queue_empty(env.GetEventQueue())) {
            m_redraw = false;

            m_background->Draw();

            for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
                (*it)->Draw();
            }

            m_bird->Draw();

            char buf[16];
            sprintf_s(buf, "%d", m_score);
            al_draw_text(env.GetNormalFont(), al_map_rgb(55, 55, 55), env.GetScreenW() - 100, 10, ALLEGRO_ALIGN_CENTRE, buf);

            al_flip_display();
        }
    }
    al_stop_timer(env.GetTimer());
}

void PlayState::Stop() {
    m_stop = true;
}

void PlayState::Exit() {
    delete m_bird;
    m_bird = nullptr;

    delete m_background;
    m_background = nullptr;

    for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
        delete (*it);
    }
    m_objects.clear();
}

bool PlayState::CheckCollision() {
    if (m_bird == nullptr) {
        return false;
    }

    if (m_bird->GetTop() >= Environment::GetScreenH() - m_bird->GetHeight() - 64.0f || m_bird->GetTop() <= 0.0f) {
        return true;
    }

    for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
        int x1 = m_bird->GetLeft() + m_bird->GetWidth() / 2;
        int y1 = m_bird->GetTop() + m_bird->GetHeight() / 2;
        int x2 = (*it)->GetLeft() + (*it)->GetWidth() / 2;
        int y2 = (*it)->GetTop() + (*it)->GetHeight() / 2;
        int w = (m_bird->GetWidth() + (*it)->GetWidth()) / 2;
        int h = (m_bird->GetHeight() + (*it)->GetHeight()) / 2;
        if (abs(x1 - x2) < w - 10 && abs(y1 - y2) < h - 10) {
            return true;
        }
    }
    return false;
}

bool PlayState::MissionCompleted() {
    for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
        if ((*it)->GetLeft() > 0) {
            return false;
        }
    }
    return true;
}

}  // namespace FSM
