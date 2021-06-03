#include "Event.h"
#include "GameOverState.h"

namespace FSM {

void GameOverState::Enter() {
    al_flip_display();
    m_stop = false;
}

int GameOverState::GetNearScore(ScoreRecord (&array)[kMaxScoreRecord], int* rank, int* start_num) {
    Environment& env = Environment::Instance();
    ScoreList scores;
    env.GetScores(&scores);
    scores.Sort();

    ScoreRecord new_score = env.GetNewScore();
    int pos = 0;
    for (; pos < scores.m_count; ++pos) {
        if (std::stoi(scores.m_scores[pos].m_score) < std::stoi(new_score.m_score)) {
            break;
        }
    }
    int count = 0;
    int m = (scores.m_count - pos) > 3 ? 3 : 6 - (scores.m_count - pos);

    for (int i = 0; i < m; ++i) {
        if (pos - m + i >= 0) {
            array[count++] = scores.m_scores[pos - m + i];
        }
    }
    *rank = count;
    *start_num = pos - count;
    array[count++] = new_score;
    while (pos < scores.m_count && count < kMaxScoreRecord) {
        array[count++] = scores.m_scores[pos++];
    }
    return count;
}

void GameOverState::Run() {
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

    const char* title = "Game Over";

    const char* menu[3] = {"Restart", "Main Menu", "Exit"};
    int highlight = 0;

    ScoreRecord array[7];
    int rank = 0;
    int start_num = 0;
    int count = GetNearScore(array, &rank, &start_num);

    int font_height = al_get_font_line_height(font);
    int normal_font_height = al_get_font_line_height(normal_font);

    float center_x = Environment::GetScreenW() / 2.0;
    float center_y = Environment::GetScreenH() / 2.0;

    Drawable* background = new Object("res/background.png");
    background->Draw();

    // title
    al_draw_text(font, al_map_rgb(128, 55, 196), center_x, font_height / 4, ALLEGRO_ALIGN_CENTRE, title);

    for (int i = 0; i < count; ++i) {
        char buf[128] = {0};
        snprintf(buf, std::size(buf), "No.%-4d %-8s   %-8s%c", i + start_num + 1, array[i].m_name.c_str(), array[i].m_score.c_str(),
                  rank == i ? '<' : ' ');
        al_draw_text(normal_font, rank == i ? al_map_rgb(128, 55, 196) : al_map_rgb(55, 128, 55), center_x,
                     center_y + (i - 5) * normal_font_height, ALLEGRO_ALIGN_CENTER, buf);
    }

    for (int i = 0; i < sizeof(menu) / sizeof(const char*); ++i) {
        al_draw_text(font, highlight == i ? al_map_rgb(128, 55, 196) : al_map_rgb(55, 128, 55), center_x,
                     Environment::GetScreenH() + (i - 3) * font_height, ALLEGRO_ALIGN_CENTER, menu[i]);
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
                    highlight = (highlight - 1 + sizeof(menu) / sizeof(const char*)) % (sizeof(menu) / sizeof(const char*));
                    redraw = true;
                    break;

                case ALLEGRO_KEY_DOWN:
                    highlight = (highlight + 1 + sizeof(menu) / sizeof(const char*)) % (sizeof(menu) / sizeof(const char*));
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
                    PlayEvent::Instance().Trigger();
                    break;

                case ALLEGRO_KEY_ENTER:
                    if (highlight == 0) {
                        PlayEvent::Instance().Trigger();
                    } else if (highlight == 2) {
                        ExitEvent::Instance().Trigger();
                    } else {
                        BackToMenuEvent::Instance().Trigger();
                    }
                    break;
            }
            if (ev.keyboard.keycode >= ALLEGRO_KEY_A && ev.keyboard.keycode <= ALLEGRO_KEY_Z) {
                if (array[rank].m_name.length() < 8) {
                    array[rank].m_name += ((char)(ev.keyboard.keycode + 'A' - 1));
                    redraw = true;
                }
            }
        }

        if (redraw && al_is_event_queue_empty(env.GetEventQueue())) {
            redraw = false;

            background->Draw();

            // title
            al_draw_text(font, al_map_rgb(128, 55, 196), center_x, font_height / 4, ALLEGRO_ALIGN_CENTRE, title);

            for (int i = 0; i < count; ++i) {
                char buf[128] = {0};
                snprintf(buf, std::size(buf), "No.%-4d %-8s   %-8s%c", i + start_num + 1, array[i].m_name.c_str(), array[i].m_score.c_str(),
                          rank == i ? '<' : ' ');
                al_draw_text(normal_font, rank == i ? al_map_rgb(128, 55, 196) : al_map_rgb(55, 128, 55), center_x,
                             center_y + (i - 5) * normal_font_height, ALLEGRO_ALIGN_CENTER, buf);
            }

            for (int i = 0; i < sizeof(menu) / sizeof(const char*); ++i) {
                al_draw_text(font, highlight == i ? al_map_rgb(128, 55, 196) : al_map_rgb(55, 128, 55), center_x,
                             Environment::GetScreenH() + (i - 3) * font_height, ALLEGRO_ALIGN_CENTER, menu[i]);
            }

            al_flip_display();
        }
    }
    if (array[rank].m_name.length() > 0) {
        env.AddScore(array[rank].m_name.c_str(), array[rank].m_score.c_str());
    } else {
        env.AddScore("DH", array[rank].m_score.c_str());
    }
    delete background;
}

}  // namespace FSM
