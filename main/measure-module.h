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


typedef struct Sound
{
    int bank;
    int instrument;
    int note;
    newdigate::audiosample *sd_cached_sound;
    bool empty;

    // Overload the equality operator
    bool operator==(const Sound &other) const
    {
        return bank == other.bank &&
               instrument == other.instrument &&
               note == other.note &&
               sd_cached_sound == other.sd_cached_sound &&
               empty == other.empty;
    }
} Sound;

typedef struct Step
{
    int id;
    int active_sounds;
    struct Sound sound_list[4];

} Step;

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
    struct Beat beat_list[4];
    int step;
    int beat;
    bool effect_mode;
    struct Step current_step;
    struct Step prior_step;

} Measure;

Measure testing_measure;

Step *active_step;
Step *last_step;
Sound temp_adding_sound;

Sound testing_palette[PALETTE_SIZE];
Sound new_sound;
Sound empty_sound = {-1, -1, -1, nullptr, true};

bool new_sound_assignment = false;
bool measure_edit = false;

int beat = 0;
int step = 0;
int temp_last_step = 0;
int temp_last_beat = 0;
int volume = 127;
int silent = 0;
int evenodd = 0;

// functions, extern variables, structs go here
void measure_palette_init(void);

Step *button_to_step(int actuated_button[]);
Step *next_step(Measure *measure);
Step *previous_step(Measure *measure);
Step *button_step_lookup(Measure *measure);

int stop_step(Step *step_end);
int play_step(Step *step_play);
float step_interval_calc(Measure *measure);
int add_remove_measure_sound(Measure *measure);
void print_step(Measure *measure);
void print_palette(int palette_index);

void populate_default_measure(void);

#endif // EXAMPLE_MODULE_H