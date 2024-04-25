/*
 *
 *       example function library
 *
 *
 */
#ifndef MEASURE_MODULE_H
#define MEASURE_MODULE_H

#define PALETTE_SIZE 12

#define MAX_MEASURES 10
#define MAX_BEATS 4
#define MAX_STEPS 6
#define MAX_STEP_SOUNDS 4

#define ROW 0
#define COLUMN 1

#define LAST_MEASURE_COLUMN 5
#define BUTTON_FLOATING 9
#define EFFECTS_ROW 3

#define MIDI_NULL -1

#define DEFAULT_LAST_STEP_INDEX 0
#define DEFAULT_LAST_BEAT_INDEX 3
#define DEFAULT_LAST_STEP 5

#include <Metro.h>

typedef struct Sound
{
    int bank;
    int instrument;
    int note;
    newdigate::audiosample *sd_cached_sound;
    std::string filename;
    bool empty;

    // Overload the equality operator
    bool operator==(const Sound &other) const
    {
        return bank == other.bank &&
               instrument == other.instrument &&
               note == other.note &&
               sd_cached_sound == other.sd_cached_sound &&
               filename == other.filename &&
               empty == other.empty;
    }
} Sound;

Sound empty_sound = {-1, -1, -1, nullptr, "", true};

typedef struct Step
{
    int id;
    int active_sounds;
    struct Sound sound_list[4];

} Step;
Step *placeholder;

typedef struct Beat
{
    int id;
    int active_steps;
    struct Step step_list[6];

} Beat;

typedef struct Measure
{
    int id;
    int active_beats;
    int step;
    int beat;
    struct Step current_step;
    struct Step prior_step;
    struct Beat beat_list[4];

} Measure;

typedef struct Palette_Slot
{
    struct Sound sound;
    int effect;
    bool is_empty;
} Palette_Slot;

std::vector<Palette_Slot> testing_palette_combined(PALETTE_SIZE);

Measure *current_measure;
Measure *edit_measure;

Step *active_step;
Step *last_step;
Sound temp_adding_sound;

Sound testing_palette[PALETTE_SIZE];
Palette_Slot new_palette_slot;

bool new_sound_assignment = false;
int palette_assignment = 0;
bool measure_edit = false;

int beat = 0;
int step = 0;
int temp_last_step = 0;
int temp_last_beat = 0;
int volume = 127;
int silent = 0;
int evenodd = 0;
int play_pause_toggle = 1;

Metro step_timer = Metro(100);
int placeholder_sound = 0;

// functions, extern variables, structs go here
int measure_palette_init(void);
Measure *measure_create(int id);

Step *button_to_step(int actuated_button[]);
Step *next_step(Measure *measure);
Step *previous_step(Measure *measure);
Step *button_step_lookup(Measure *measure);
int wipe_step(Step *step);

int stop_step(Step *step_end);
int play_step(Step *step_play);
float step_interval_calc(Measure *measure);
int add_remove_measure_sound(Measure *measure);
int check_palette_sound(Step *step_LED);

void print_step(Step *step);
void print_palette(int palette_index);

void populate_default_measure(void);

#endif // EXAMPLE_MODULE_H