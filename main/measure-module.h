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

} Measure;

Measure testing_measure;
Sound testing_palette[PALETTE_SIZE];
Sound new_sound;
Step defaultStep; // Define a default Step object
bool new_sound_assignment = false;

// functions, extern variables, structs go here
void measure_palette_init(void);
Step button_to_step(int actuated_button[]);

#endif // EXAMPLE_MODULE_H