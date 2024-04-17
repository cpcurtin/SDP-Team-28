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

/*******************************************************************************
****************************  NAVIGATION INSTANCES  ****************************
*******************************************************************************/
#define NAVIGATION_MAIN 0
#define NAVIGATION_SOUNDS 1
#define NAVIGATION_EFFECTS 2
#define NAVIGATION_TRACKS 3
#define NAVIGATION_TRACK_LOAD 4
#define NAVIGATION_SET_BEATS 5
#define NAVIGATION_SET_STEPS 6
#define NAVIGATION_SOUNDS_CUSTOM 7
#define NAVIGATION_SOUNDS_MIDI 8
#define NAVIGATION_SOUNDS_MIDI_PERCUSSION 9
#define NAVIGATION_SOUNDS_MIDI_MELODIC 10
#define NAVIGATION_MIDI_OCTAVES 11
#define NAVIGATION_MIDI_NOTES 12
#define NAVIGATION_TRACK_SAVE 13
/******************************************************************************/

/*******************************************************************************
***************************  LEAF SELECTION INDICES  ***************************
*******************************************************************************/
#define LEAF_TRACKS_SAVE 0
#define LEAF_TRACKS_LOAD 1
#define LEAF_TRACKS_DELETE 2
#define LEAF_TRACKS_GLOBAL_BEATS 3
#define LEAF_TRACKS_GLOBAL_STEPS 4
#define LEAF_TRACKS_LOCAL_STEPS 5

/******************************************************************************/
#define TRACK_SAVE_NAME_LENGTH 3

#include <vector>
#include <string>

int track_save_panel = 0;
std::string track_save_string(TRACK_SAVE_NAME_LENGTH, ' ');
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
    // std::string name;
    int id;
    std::vector<std::string> data_array;
    struct Nav *parent;
    struct Nav **child;
    std::vector<std::string> lcd_state;
    int index;

} Nav;

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

#endif

// NAV STRUCT INITIALIZATION
Nav *nav_state = new Nav;
Nav *main_nav = new Nav;
Nav *sounds_nav = new Nav;
Nav *effects_nav = new Nav;
Nav *tracks_nav = new Nav;
Nav *sounds_custom_nav = new Nav;
Nav *sounds_midi_nav = new Nav;

Nav *tracks_save_nav = new Nav;
Nav *tracks_load_nav = new Nav;
Nav *tracks_set_measures_nav = new Nav;
Nav *tracks_set_beats_nav = new Nav;
Nav *tracks_set_steps_nav = new Nav;

Nav *sounds_midi_melodic_nav = new Nav;
Nav *sounds_midi_percussion_nav = new Nav;
Nav *sounds_midi_octaves_nav = new Nav;
Nav *sounds_midi_notes_nav = new Nav;

struct nav_config *nav_cfg = new struct nav_config;
// functions, extern variables, structs go here
Nav *nav_selection(Nav *nav, int direction);
void nav_add(Nav *node);

#if USING_SAFE_STRINGS == 1 // safe - new
Nav *nav_init(struct nav_config *cfg);
void dpad_nav_routine(int dpad_pressed);
int execute_leaf(void);
void array_scroll(Nav *nav, int direction);
std::string format_row(std::vector<std::string> data_array, int index, int format);
std::string tracks_update(void);
int track_options(void);
#else // unsafe - old
Nav *nav_init(struct nav_config *cfg);

void array_scroll(Nav *nav, int direction);
const char *format_row(const char **data_array, int index, int format);
void dpad_nav_routine(int dpad_pressed);
const char *tracks_update(void);
#endif

#endif // EXAMPLE_MODULE_H