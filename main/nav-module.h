/*
 *
 *       example function library
 *
 *
 */
#ifndef NAV_MODULE_H
#define NAV_MODULE_H

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
#define NAVIGATION_DELETE 14
#define NAVIGATION_MEASURE 15
#define NAVIGATION_MEASURE_SELECT 16
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

#define LEAF_MEASURES_EDIT 0
#define LEAF_MEASURES_ADD 1
#define LEAF_MEASURES_REMOVE 2

#define LEAF_DELETE_LAST 0
#define LEAF_DELETE_STEP 1
#define LEAF_DELETE_MEASURE 2
/******************************************************************************/
#define TRACK_SAVE_NAME_LENGTH 3

#define PALETTE_ASSIGNMENT_DEFAULT 0
#define PALETTE_ASSIGNMENT_SOUND 1
#define PALETTE_ASSIGNMENT_EFFECT 2

#include <vector>
#include <string>

int track_save_panel = 0;
std::string track_save_string(TRACK_SAVE_NAME_LENGTH, ' ');

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

// NAV STRUCT INITIALIZATION
Nav *nav_state = new Nav;
Nav *main_nav = new Nav;
Nav *sounds_nav = new Nav;
Nav *effects_nav = new Nav;
Nav *tracks_nav = new Nav;
Nav *sounds_custom_nav = new Nav;
Nav *sounds_midi_nav = new Nav;
Nav *measure_nav = new Nav;
Nav *delete_nav = new Nav;

Nav *tracks_save_nav = new Nav;
Nav *tracks_load_nav = new Nav;
Nav *tracks_set_measures_nav = new Nav;
Nav *tracks_set_beats_nav = new Nav;
Nav *tracks_set_steps_nav = new Nav;

Nav *sounds_midi_melodic_nav = new Nav;
Nav *sounds_midi_percussion_nav = new Nav;
Nav *sounds_midi_octaves_nav = new Nav;
Nav *sounds_midi_notes_nav = new Nav;

Nav *measure_select_nav = new Nav;

struct nav_config *nav_cfg = new struct nav_config;
// functions, extern variables, structs go here
Nav *nav_selection(Nav *nav, int direction);
void nav_add(Nav *node);
Nav *nav_init(struct nav_config *cfg);
void dpad_nav_routine(int dpad_pressed);
int execute_leaf(void);
void array_scroll(Nav *nav, int direction);
std::string format_row(std::vector<std::string> data_array, int index, int format);
std::string tracks_update(void);
int measure_options(void);
int track_options(void);
int delete_options(void);

#endif // EXAMPLE_MODULE_H