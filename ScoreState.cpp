#include <iostream>
#include "Event.h"
#include "ScoreState.h"

namespace FSM {

void ScoreState::Enter() {
    al_flip_display();
    m_stop = false;
}

void ScoreState::Run() {
    enum MYKEYS { KEY_UP, KEY_DOWN };
    bool key[4] = {false, false, false, false};
    bool redraw = true;
    Environment& env = Environment::Instance();

    if (!env) {
        ExitEvent::Instance().Trigger();
        return;
    }

    ALLEGRO_FONT* font = env.GetTitleFont();

    if (!font) {
        LOG("failed to create bouncer bitmap!\n");
        ExitEvent::Instance().Trigger();
        return;
    }

    ALLEGRO_FONT* normal_font = env.GetNormalFont();

    if (!normal_font) {
        LOG("failed to create bouncer bitmap!\n");
        ExitEvent::Instance().Trigger();
        return;
    }

    const char* title = "Scores";

    ScoreList scores;
    env.GetScores(&scores);
    scores.Sort();

    int highlight = 0;
    int start_num = 0;
    int font_height = al_get_font_line_height(font);
    int normal_font_height = al_get_font_line_height(normal_font);

    float center_x = Environment::GetScreenW() / 2.0f;
    float center_y = Environment::GetScreenH() / 2.0f;

    Drawable* background = new Object("res/background.png");
    background->Draw();

    // title
    al_draw_text(font, al_map_rgb(128, 55, 196), center_x, font_height / 4, ALLEGRO_ALIGN_CENTRE, title);

    for (int i = start_num; i < scores.m_count && i < start_num + kMaxScoreRecord; ++i) {
        char buf[128] = {0};
        sprintf_s(buf, "No.%-4d %-8s %-8s%C", i + 1, scores.m_scores[i].m_name.c_str(), scores.m_scores[i].m_score.c_str(),
                  highlight == i ? '<' : ' ');
        al_draw_text(normal_font, highlight == i ? al_map_rgb(128, 55, 196) : al_map_rgb(55, 128, 55), center_x,
                     center_y + (i - start_num - kMaxScoreRecord / 2) * normal_font_height * 1.2, ALLEGRO_ALIGN_CENTER, buf);
    }

    al_flip_display();

    while (!m_stop) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(env.GetEventQueue(), &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            ExitEvent::Instance().Trigger();
            break;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    std::cout << "key up" << std::endl;
                    --highlight;
                    if (highlight < 0) {
                        highlight = 0;
                    }
                    if (start_num > highlight) {
                        start_num = highlight;
                    }
                    redraw = true;
                    break;

                case ALLEGRO_KEY_DOWN:
                    ++highlight;
                    if (highlight >= scores.m_count) {
                        highlight = scores.m_count - 1;
                    }
                    if (highlight >= start_num + kMaxScoreRecord) {
                        start_num++;
                    }
                    redraw = true;
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
                    BackToMenuEvent::Instance().Trigger();
                    break;
            }
        }

        if (redraw && al_is_event_queue_empty(env.GetEventQueue())) {
            redraw = false;

            background->Draw();

            // title
            al_draw_text(font, al_map_rgb(128, 55, 196), center_x, font_height / 4, ALLEGRO_ALIGN_CENTRE, title);

            for (int i = start_num; i < scores.m_count && i < start_num + kMaxScoreRecord; ++i) {
                char buf[128] = {0};
                sprintf_s(buf, "No.%-4d %-8s %-8s%C", i + 1, scores.m_scores[i].m_name.c_str(),
                          scores.m_scores[i].m_score.c_str(), highlight == i ? '<' : ' ');
                al_draw_text(normal_font, highlight == i ? al_map_rgb(128, 55, 196) : al_map_rgb(55, 128, 55), center_x,
                             center_y + (i - start_num - kMaxScoreRecord / 2) * normal_font_height * 1.2, ALLEGRO_ALIGN_CENTER,
                             buf);
            }

            al_flip_display();
        }
    }
    delete background;
}

}  // namespace FSM
