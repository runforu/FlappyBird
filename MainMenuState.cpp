#include "Event.h"
#include "MainMenuState.h"

void FSM::MainMenuState::Enter() {
    al_flip_display();
    m_stop = false;
}

void FSM::MainMenuState::Run() {
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

    Drawable* background = new Object("res/background.png");
    background->Draw();

    const char* menu[3] = {"Play ", "Scorces ", "Exit"};
    int highlight = 0;

    int font_height = al_get_font_line_height(font);

    float center_x = Environment::GetScreenW() / 2.0;
    float center_y = Environment::GetScreenH() / 2.0;

    for (int i = 0; i < sizeof(menu) / sizeof(const char*); ++i) {
        al_draw_text(font, highlight == i ? al_map_rgb(128, 55, 196) : al_map_rgb(55, 128, 55), center_x,
                     center_y + (i - 1) * font_height * 2, ALLEGRO_ALIGN_CENTRE, menu[i]);
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

                case ALLEGRO_KEY_ENTER:
                    if (highlight == 0) {
                        PlayEvent::Instance().Trigger();
                    } else if (highlight == 2) {
                        ExitEvent::Instance().Trigger();
                    } else {
                        GoToScoreEvent::Instance().Trigger();
                    }
                    break;
            }
        }

        if (redraw && al_is_event_queue_empty(env.GetEventQueue())) {
            redraw = false;
            background->Draw();

            for (int i = 0; i < sizeof(menu) / sizeof(const char*); ++i) {
                al_draw_text(font, highlight == i ? al_map_rgb(128, 55, 196) : al_map_rgb(55, 128, 55), center_x,
                             center_y + (i - 1) * font_height * 2, ALLEGRO_ALIGN_CENTRE, menu[i]);
            }

            al_flip_display();
        }
    }

    delete background;
}

void FSM::MainMenuState::Exit() {}
