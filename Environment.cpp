#include "Environment.h"
#include <filesystem>

const float Environment::kFps = 60;
const char* Environment::kConfigFile = "config.ini";
const char* Environment::kConfigScoreSection = "Scores";

Drawable::Drawable(float x, float y) : m_left(x), m_top(y) {}

void Drawable::Draw() {
    for (auto it = m_interpolators.begin(); it != m_interpolators.end(); ++it) {
        (*it)->Interpolate(this);
    }
    DoDraw();
}

Drawable::~Drawable() {
    m_interpolators.clear();
}

Spirit::Spirit(std::string wildcard_file_path, float x, float y)
    : Drawable(x, y), m_index(0), m_spead(1), m_count(0)
{
    // Wildcard question mark is not supported.
    assert(wildcard_file_path.find('?') == std::string::npos);
	namespace fs = std::filesystem;
    const auto wildcard_path = fs::path{wildcard_file_path};

    const auto directory_path = wildcard_path.parent_path();
    // No wildcard asterisk in directory path.
    assert(directory_path.u8string().find('*') == std::string::npos);

    const auto filename = wildcard_path.filename().u8string();
	const auto wildcard_index = filename.find('*');
    // Only 0 or 1 asterisk in filename.
    assert(wildcard_index == filename.rfind('*'));

	const auto file_prefix = filename.substr(0, wildcard_index);
	const auto file_suffix = wildcard_index + 1 > wildcard_file_path.size()
		? ""
		: filename.substr(wildcard_index + 1);

    for (const auto& entry : fs::directory_iterator(directory_path)) {
        if (!entry.is_regular_file()) { continue; }
        const auto entry_path = entry.path();
        const auto entry_filename = entry_path.filename().u8string();
        if (entry_filename.find(file_prefix) != 0) { continue; }
        if (entry_filename.rfind(file_suffix)
            != entry_filename.size() - file_suffix.size())
        { continue; }

        if (auto bitmap = Environment::Instance().LoadBitmap(
            entry_path.u8string().c_str()))
        {
            m_bitmaps.push_back(bitmap);
        }
    }
}

Spirit::~Spirit() {
    m_bitmaps.clear();
}

void Spirit::DoDraw() {
    if (m_count % (kFramePerPicture / m_spead)) {
        ALLEGRO_BITMAP* bitmap = m_bitmaps[(m_index++) % m_bitmaps.size()];
        if (bitmap != nullptr) {
            al_draw_bitmap(bitmap, GetLeft(), GetTop(), 0);
        }
    }
    ++m_count;
}

int Spirit::GetWidth() {
    if (m_bitmaps.size() > 0 && m_bitmaps[0] != nullptr) {
        return al_get_bitmap_width(m_bitmaps[0]);
    }
    return 0;
}

int Spirit::GetHeight() {
    if (m_bitmaps.size() > 0 && m_bitmaps[0] != nullptr) {
        return al_get_bitmap_height(m_bitmaps[0]);
    }
    return 0;
}

Object::Object(std::string image_path, float x, float y) : Drawable(x, y) {
    m_bitmap = Environment::Instance().LoadBitmap(image_path.c_str());
}

Object::Object() : Drawable(), m_bitmap(nullptr) {}

void Object::DoDraw() {
    if (m_bitmap != nullptr) {
        al_draw_bitmap(m_bitmap, GetLeft(), GetTop(), 0);
    }
}

int Object::GetWidth() {
    if (m_bitmap != nullptr) {
        return al_get_bitmap_width(m_bitmap);
    }
    return 0;
}

int Object::GetHeight() {
    if (m_bitmap != nullptr) {
        return al_get_bitmap_height(m_bitmap);
    }
    return 0;
}

Environment& Environment::Instance() {
    // All member variables are zero for static variables in bss zone
    static Environment _instance;
    return _instance;
}

ALLEGRO_BITMAP* Environment::LoadBitmap(const char* filename) {
    ALLEGRO_BITMAP* bitmap = m_bitmaps[filename];
    if (bitmap == NULL) {
        bitmap = al_load_bitmap(filename);
        if (bitmap == NULL) {
            m_bitmaps.erase(filename);
            LOG("failed to load bitmap!");
            return NULL;
        }
        m_bitmaps[filename] = bitmap;
    }
    return bitmap;
}

ALLEGRO_FONT* Environment::GetTitleFont() {
    if (m_title_font == nullptr) {
        m_title_font = al_load_ttf_font("res/consola.ttf", 64, 0);
    }
    return m_title_font;
}

ALLEGRO_FONT* Environment::GetNormalFont() {
    if (m_normal_font == nullptr) {
        m_normal_font = al_load_ttf_font("res/consola.ttf", 32, 0);
    }
    return m_normal_font;
}

bool Environment::AddConfig(const char* section, const char* key, const char* value) {
    // lazy init
    InitConfig();
    if (m_config != nullptr) {
        al_set_config_value(m_config, section, key, value);
        return true;
    }
    return false;
}

bool Environment::GetScores(ScoreList* scores) {
    // lazy init
    InitConfig();
    if (m_config != nullptr) {
        ALLEGRO_CONFIG_ENTRY* entry = nullptr;
        scores->m_count = 0;
        const char* key = al_get_first_config_entry(m_config, kConfigScoreSection, &entry);
        while (key != nullptr) {
            scores->m_count++;
            key = al_get_next_config_entry(&entry);
        }
        scores->m_scores = new ScoreRecord[scores->m_count];

        key = al_get_first_config_entry(m_config, kConfigScoreSection, &entry);
        for (int i = 0; i < scores->m_count; ++i) {
            scores->m_scores[i].m_name = key;
            scores->m_scores[i].m_score = al_get_config_value(m_config, kConfigScoreSection, key);
            key = al_get_next_config_entry(&entry);
        }
        return true;
    }
    return false;
}

bool Environment::Init() {
    if (!al_init()) {
        LOG("failed to initialize allegro!");
        return false;
    }

    if (!al_init_image_addon()) {
        LOG("failed to initialize allegro image addon!");
        return false;
    }

    if (!al_init_primitives_addon()) {
        LOG("failed to initialize allegro primitives addon!");
        return false;
    }

    if (!al_install_keyboard()) {
        LOG("failed to initialize keyboard!");
        return false;
    }

    if (!al_init_font_addon()) {
        LOG("failed to initialize font addon!");
        return false;
    }

    if (!al_init_ttf_addon()) {
        LOG("failed to initialize ttf addon!");
        return false;
    }

    return true;
}

bool Environment::InitDisplay() {
    m_display = al_create_display(kScreenWidth, kScreenHeight);
    if (!m_display) {
        LOG("failed to create display!");
        return false;
    }
    return true;
}

bool Environment::InitEventQueue() {
    m_event_queue = al_create_event_queue();
    if (!m_event_queue) {
        LOG("failed to create event_queue!");
        return false;
    }
    al_register_event_source(m_event_queue, al_get_display_event_source(m_display));

    al_register_event_source(m_event_queue, al_get_timer_event_source(m_timer));

    al_register_event_source(m_event_queue, al_get_keyboard_event_source());
    return true;
}

bool Environment::InitTimer() {
    m_timer = al_create_timer(1.0 / kFps);
    if (!m_timer) {
        LOG("failed to create timer!");
        return false;
    }
    return true;
}

ALLEGRO_CONFIG* Environment::InitConfig() {
    if (m_config == nullptr) {
        m_config = al_load_config_file(kConfigFile);
        if (m_config == nullptr) {
            m_config = al_create_config();
            if (m_config != nullptr) {
                al_add_config_section(m_config, kConfigScoreSection);
                al_save_config_file(kConfigFile, m_config);
            }
        }
    }
    return m_config;
}

Environment::Environment() {
    m_initialized = Init() && InitDisplay() && InitTimer() && InitEventQueue();
}

Environment::~Environment() {
    for (auto iter = m_bitmaps.begin(); iter != m_bitmaps.end(); iter++) {
        al_destroy_bitmap(iter->second);
    }
    if (m_title_font != nullptr) {
        al_destroy_font(m_title_font);
    }
    if (m_normal_font != nullptr) {
        al_destroy_font(m_normal_font);
    }
    if (m_config != nullptr) {
        al_save_config_file(kConfigFile, m_config);
        al_destroy_config(m_config);
    }
    DestroyEventQueue();
    DestroyTimer();
    DestroyDisplay();
}
