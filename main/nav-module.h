/*
 *
 *       example function library
 *
 *
 */
#ifndef NAV_MODULE_H
#define NAV_MODULE_H

#ifndef USING_SAFE_STRINGS
#define USING_SAFE_STRINGS 1
#endif

#include <vector>

#if USING_SAFE_STRINGS == 1 // safe - new

struct nav_config
{
    std::vector<std::string> sounds_custom;
    std::vector<std::string> effects;
    std::vector<std::string> tracks_load;
    std::vector<std::string> sounds_midi_melodic;
    std::vector<std::string> sounds_midi_percussion;
};

typedef struct Nav
{
    std::string name;
    std::vector<std::string> data_array;
    struct Nav *parent;
    struct Nav **child;
    std::vector<std::string> lcd_state;
    int index;

} Nav;

Nav *nav_init(struct nav_config *cfg);

std::vector<std::string> octaves = {"-2 LOWEST", "-1", "0", "1", "2", "3", "4", "5", "6", "7", "8 HIGHEST"};
std::vector<std::string> note_names = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

#else // unsafe - old

const char *octaves[] = {"-2 LOWEST", "-1", "0", "1", "2", "3", "4", "5", "6", "7", "8 HIGHEST"};
const char *note_names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

struct nav_config
{
    struct array_with_size *sounds_custom;
    struct array_with_size *effects;
    struct array_with_size *tracks_load;
    struct array_with_size *sounds_midi_melodic;
    struct array_with_size *sounds_midi_percussion;
};

typedef struct Nav
{

    char *name;
    const char **data_array;
    struct Nav *parent;
    struct Nav **child;
    size_t size;
    const char **lcd_state;
    int index;

} Nav;

Nav *nav_init(struct nav_config *cfg);
#endif

// NAV STRUCT INITIALIZATION
Nav *nav_state = new Nav;
Nav *main_nav = new Nav;
Nav *sounds_nav = new Nav;
Nav *effects_nav = new Nav;
Nav *tracks_nav = new Nav;
Nav *sounds_custom_nav = new Nav;
Nav *sounds_midi_nav = new Nav;
Nav *tracks_load_nav = new Nav;
Nav *tracks_set_steps_nav = new Nav;

Nav *sounds_midi_melodic_nav = new Nav;
Nav *sounds_midi_percussion_nav = new Nav;
Nav *sounds_midi_octaves_nav = new Nav;
Nav *sounds_midi_notes_nav = new Nav;

struct nav_config *nav_cfg = new struct nav_config;
// functions, extern variables, structs go here
Nav *nav_selection(Nav *nav, int direction);

void nav_add(Nav *node);
#if USING_SAFE_STRINGS == 1

void array_scroll(Nav *nav, int direction);
std::string format_row(std::vector<std::string> data_array, int index, int format);
std::string tracks_update(void);

#else

void array_scroll(Nav *nav, int direction);
const char *format_row(const char **data_array, int index, int format);

const char *tracks_update(void);
#endif

void dpad_nav_routine(int dpad_pressed);

#endif // EXAMPLE_MODULE_H