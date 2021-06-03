#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <map>
#include <string>
#include <vector>
#include "Loger.h"

class Drawable;
class Interpolator {
public:
    virtual void Interpolate(Drawable*) = 0;
};

class Drawable {
public:
    virtual int GetWidth() = 0;

    virtual int GetHeight() = 0;

    virtual ~Drawable();

    void Draw();

    Drawable(float x = 0, float y = 0);

    void AddInterpolator(Interpolator* interpolator) {
        if (interpolator != nullptr) {
            m_interpolators.push_back(interpolator);
        }
    }

    inline float& GetLeft() {
        return m_left;
    }

    inline float& GetTop() {
        return m_top;
    }

private:
    virtual void DoDraw() = 0;
    std::vector<Interpolator*> m_interpolators;
    float m_left;
    float m_top;
};

struct Spirit : public Drawable {
public:
    Spirit(std::string wildcard_file_path, float x = 0, float y = 0);

    ~Spirit();

    void DoDraw() override;

    int GetWidth() override;

    int GetHeight() override;

    // no need to free bitmaps
    std::vector<ALLEGRO_BITMAP*> m_bitmaps;
    static const int kFramePerPicture = 32;
    int m_index;
    int m_spead;
    int m_count;
};

// in future should use border box to check collision not bitmap size
struct Object : public Drawable {
public:
    Object(std::string image_path, float x = 0, float y = 0);

    Object();

    void DoDraw() override;

    int GetWidth() override;

    int GetHeight() override;

    ALLEGRO_BITMAP* m_bitmap = NULL;
};

struct ScoreRecord {
    std::string m_name;
    std::string m_score;
};

// Scores class can avoid forgeting to free memory
struct ScoreList {
    ~ScoreList() {
        delete[] m_scores;
    }

    void Sort() {
        for (int i = 1; i < m_count; ++i) {
            for (int j = i; j > 0; --j) {
                if (std::stoi(m_scores[j].m_score) > std::stoi(m_scores[j - 1].m_score)) {
                    std::string tmp = m_scores[j - 1].m_score;
                    m_scores[j - 1].m_score = m_scores[j].m_score;
                    m_scores[j].m_score = tmp;
                    tmp = m_scores[j - 1].m_name;
                    m_scores[j - 1].m_name = m_scores[j].m_name;
                    m_scores[j].m_name = tmp;
                }
            }
        }
    }

    ScoreRecord* m_scores = NULL;
    int m_count = 0;
};

class Environment {
public:
    static Environment& Instance();

    inline ALLEGRO_EVENT_QUEUE* GetEventQueue() {
        return m_event_queue;
    }

    inline ALLEGRO_TIMER* GetTimer() {
        return m_timer;
    }

    ALLEGRO_BITMAP* LoadBitmap(const char* filename);

    ALLEGRO_FONT* GetTitleFont();

    ALLEGRO_FONT* GetNormalFont();

    inline void NewScore(const ScoreRecord& score) {
        m_new_score = score;
    }

    inline const ScoreRecord& GetNewScore() {
        return m_new_score;
    }

    inline bool AddScore(const char* key, const char* value) {
        return AddConfig(kConfigScoreSection, key, value);
    }

    bool AddConfig(const char* section, const char* key, const char* value);

    bool GetScores(ScoreList* scores);

    inline explicit operator bool const() {
        return m_initialized;
    }

    inline static int GetScreenW() {
        return kScreenWidth;
    }

    inline static int GetScreenH() {
        return kScreenHeight;
    }

private:
    bool Init();

    bool InitDisplay();

    bool InitEventQueue();

    bool InitTimer();

    inline void DestroyDisplay() {
        al_destroy_display(m_display);
    }

    inline void DestroyEventQueue() {
        al_destroy_event_queue(m_event_queue);
    }

    inline void DestroyTimer() {
        al_destroy_timer(m_timer);
    }

    ALLEGRO_CONFIG* InitConfig();

private:
    Environment();

    Environment(const Environment& env) = delete;

    Environment& operator=(const Environment& env) = delete;

    ~Environment();

private:
    static const float kFps;
    static const int kScreenWidth = 1280;
    static const int kScreenHeight = 640;
    static const char* kConfigFile;
    static const char* kConfigScoreSection;

private:
    // It is a static singleton class, so all uninitialized members are zero.
    ALLEGRO_DISPLAY* m_display;
    ALLEGRO_EVENT_QUEUE* m_event_queue;
    ALLEGRO_TIMER* m_timer;
    ALLEGRO_FONT* m_title_font;
    ALLEGRO_FONT* m_normal_font;
    ALLEGRO_CONFIG* m_config;

    ScoreRecord m_new_score;
    std::map<std::string, ALLEGRO_BITMAP*> m_bitmaps;
    bool m_initialized;
};

#endif  // !_ENVIRONMENT_H_
