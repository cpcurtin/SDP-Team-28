/*
 *	example function library
 *
 *
 *
 */
#include "measure-module.h"

int measure_palette_init(void)
{
    memset(empty_sound.filename, 0, sizeof(empty_sound.filename));

    // SET DEFAULT LAST STEP
    active_step = &(current_measure->beat_list[0].step_list[0]);
    last_step = &(current_measure->beat_list[3].step_list[5]);
    temp_last_step = 5;
    temp_last_beat = 3;

    // INIT PALETTE
    for (int i = 0; i < PALETTE_SIZE; i++)
    {
        testing_palette[i] = {-1, -1, -1, nullptr, true};
    }

    return 0;
}

Measure *measure_create(int id)
{

    Measure *temp_measure = new Measure;
    Beat *temp_beat;
    Step *temp_step;
    Sound *temp_sound;

    // INIT STARTING MEASURE
    temp_measure->active_beats = MAX_BEATS;
    temp_measure->id = id;
    temp_measure->step = 0;
    temp_measure->beat = 0;
    temp_measure->effect_mode = false;

    for (int i = 0; i < MAX_BEATS; i++)
    {

        // Initialize measure beats
        temp_beat = &(temp_measure->beat_list[i]);
        temp_beat->active_steps = MAX_STEPS;
        temp_beat->id = i;

        for (int j = 0; j < MAX_STEPS; j++)
        {

            // Initialize beat steps
            temp_step = &(temp_beat->step_list[j]);
            temp_step->active_sounds = 0;
            temp_step->id = (MAX_STEPS * i) + j;
            for (int k = 0; k < MAX_STEP_SOUNDS; k++)
            {

                // Initialize beat steps
                temp_sound = &(temp_step->sound_list[k]);
                temp_sound->bank = -1;
                temp_sound->instrument = -1;
                temp_sound->note = -1;
                temp_sound->sd_cached_sound = nullptr;
                memset(temp_sound->filename, 0, sizeof(temp_sound->filename));
                temp_sound->empty = true;
                // temp_sound = empty_sound;
            }
        }
    }
    // SET DEFAULT LAST STEP
    temp_measure->current_step = temp_measure->beat_list[0].step_list[0];
    temp_measure->prior_step = temp_measure->beat_list[3].step_list[5];
    return temp_measure;
}

Step *button_to_step(Measure *measure, int actuated_button[])
{
    if (actuated_button[ROW] < 4 && actuated_button[COLUMN] < 6)
    {
        return &(current_measure->beat_list[actuated_button[ROW]]).step_list[actuated_button[COLUMN]];
    }
    else
    {
        return nullptr;
    }
}

Step *next_step(Measure *measure)
{
    measure->step++;
    if (measure->step >= measure->beat_list[measure->beat].active_steps)
    {
        measure->step = 0;
        measure->beat++;
        if (measure->beat >= measure->active_beats)
        {
            measure->beat = 0;
        }
    }
    return &(measure->beat_list[measure->beat].step_list[measure->step]);
}
Step *previous_step(Measure *measure)
{
    bool measure_change = false;
    measure->step--;
    if ((measure->step < 0) || (measure->step >= measure->beat_list[measure->beat].active_steps))
    {
        measure->beat--;

        if ((measure->beat < 0) || (measure->step >= measure->active_beats))
        {

            measure->beat = measure->active_beats - 1;
            measure_change = true;
        }
        measure->step = measure->beat_list[measure->beat].active_steps - 1;
    }
    // if (measure_change)
    // {
    //     current_measure =
    // }

    return &(measure->beat_list[measure->beat].step_list[measure->step]);
}

Step *button_step_lookup(Measure *measure)
{

    return &(measure->beat_list[matrix_button.row].step_list[matrix_button.column]);
}

int stop_step(Step *step_end)
{
    for (int sound = 0; sound < MAX_STEP_SOUNDS; sound++)
    {
        if (step_end->sound_list[sound].empty == false)
        {
            if (step_end->sound_list[sound].bank != MIDI_NULL)
            {
                if (step_end->sound_list[sound].note != MIDI_NULL)
                {
                    // MELODIC
                    midiNoteOff(step_end->sound_list[sound].bank, step_end->sound_list[sound].note, 127);
                }
                else
                {
                    // PERCUSSION
                    midiNoteOff(step_end->sound_list[sound].bank, step_end->sound_list[sound].instrument, 127);
                }
            }
            else
            {
                // SD
                stopFile(0);
            }
        }
    }

    return 0;
}

int play_step(Step *step_play)
{

    for (int sound = 0; sound < MAX_STEP_SOUNDS; sound++)
    {
        if (step_play->sound_list[sound].empty == false)
        {
            if (step_play->sound_list[sound].bank != MIDI_NULL)
            {
                if (step_play->sound_list[sound].note != MIDI_NULL)
                {
                    // MELODIC
                    midiSetInstrument(step_play->sound_list[sound].bank, step_play->sound_list[sound].instrument);
                    midiNoteOn(step_play->sound_list[sound].bank, step_play->sound_list[sound].note, 127);
                }
                else
                {
                    // PERCUSSION
                    midiSetInstrument(step_play->sound_list[sound].bank, 128);
                    midiNoteOn(step_play->sound_list[sound].bank, step_play->sound_list[sound].instrument, 127);
                }
            }
            else
            {
                // SD
                if (step_play->sound_list[sound].sd_cached_sound != nullptr)
                {
                    playFile(step_play->sound_list[sound].sd_cached_sound);
                }
            }
        }
    }

    return 0;
}

float step_interval_calc(Measure *measure)
{
    return (60000 / (current_track.bpm * measure->beat_list[beat].active_steps));
}

int add_remove_measure_sound(Measure *measure)
{

    bool sound_exists = false;
    for (int sound = 0; sound < MAX_STEP_SOUNDS; sound++)
    {

        if (testing_palette[palette_index] == button_step_lookup(measure)->sound_list[sound])
        {
            Serial.println("MEASURE REMOVE SOUND");
            // SELECTED PALETTE SOUND EXISTS ON CURRENT STEP
            // REMOVE FROM MEASURE STEP
            button_step_lookup(current_measure)->sound_list[sound] = empty_sound;
            sound_exists = true;
            break;
        }
    }
    if (sound_exists == false && button_step_lookup(current_measure)->active_sounds < MAX_STEP_SOUNDS)
    {
        Serial.println("MEASURE ADD SOUND");
        // SELECTED STEP HAS AVAILABLE SOUND SLOTS
        for (int sound = 0; sound < MAX_STEP_SOUNDS; sound++)
        {
            if (button_step_lookup(current_measure)->sound_list[sound].empty)
            {
                // ASSIGN PALETTE SOUND TO FIRST AVAILABLE STEP SOUND SLOT
                button_step_lookup(current_measure)->sound_list[sound] = testing_palette[palette_index];
                break;
            }
        }
    }
    else
    {
        // ALLOCATED STEP SOUNDS FULL, CANNOT ADD PALETTE SOUND
        return 1;
    }
    return 0;
}

void print_step(Step *step)
{

    Serial.println("\n\n");
    Serial.print("Beat: ");
    Serial.print(current_measure->beat);
    Serial.print("\tStep: ");
    Serial.println(current_measure->step);

    for (int i = 0; i < MAX_STEP_SOUNDS; i++)
    {

        Serial.print("B: ");
        Serial.print(step->sound_list[i].bank);
        Serial.print("\tI: ");
        Serial.print(step->sound_list[i].instrument);
        Serial.print("\tN: ");
        Serial.print(step->sound_list[i].note);
        Serial.print("\tf: ");
        Serial.print(step->sound_list[i].filename);
        Serial.print("\tE: ");
        Serial.print(step->sound_list[i].empty);

        char str[20];
        sprintf(str, "%p", (void *)step->sound_list[i].sd_cached_sound); // Using sprintf to format the pointer address
        Serial.println("\tSD: " + String(str));
    }
}

void print_palette(int palette_index)
{

    Serial.println("\n\n");
    Serial.println("PALETTE");
    if (palette_index >= PALETTE_SIZE)
    {

        for (int i = 0; i < PALETTE_SIZE; i++)
        {

            Serial.print("B: ");
            Serial.print(testing_palette[i].bank);
            Serial.print("\tI: ");
            Serial.print(testing_palette[i].instrument);
            Serial.print("\tN: ");
            Serial.print(testing_palette[i].note);
            Serial.print("\tf: ");
            Serial.print(testing_palette[i].filename);
            Serial.print("\tE: ");
            Serial.println(testing_palette[i].empty);
        }
    }
    else
    {
        Serial.print("B: ");
        Serial.print(testing_palette[palette_index].bank);
        Serial.print("\tI: ");
        Serial.print(testing_palette[palette_index].instrument);
        Serial.print("\tN: ");
        Serial.print(testing_palette[palette_index].note);
        Serial.print("\tf: ");
        Serial.print(testing_palette[palette_index].filename);
        Serial.print("\tE: ");
        Serial.println(testing_palette[palette_index].empty);
    }
}

void populate_default_measure(void)
{

    int temp_populate_step = 0;
    for (int b = 0; b < MAX_BEATS; b++)
    {

        for (int s = 0; s < MAX_STEPS; s++)
        {

            for (int i = 0; i < MAX_STEP_SOUNDS; i++)
            {
                temp_adding_sound.bank = meMat[temp_populate_step][(i * 3) + 0];
                temp_adding_sound.instrument = meMat[temp_populate_step][(i * 3) + 1];
                temp_adding_sound.note = meMat[temp_populate_step][(i * 3) + 2];
                temp_adding_sound.sd_cached_sound = nullptr;
                memset(temp_adding_sound.filename, 0, sizeof(temp_adding_sound.filename));
                if (meMat[temp_populate_step][(i * 3) + 0] == MIDI_NULL)
                {
                    temp_adding_sound.empty = true;
                }
                else
                {
                    temp_adding_sound.empty = false;
                }
                // temp_sound = empty_sound;

                (&(current_measure->beat_list[b].step_list[s]))->sound_list[i] = temp_adding_sound;
            }

            temp_populate_step++;
        }
    }

    active_step = &(current_measure->beat_list[0].step_list[0]);
    last_step = &(current_measure->beat_list[3].step_list[5]);
    temp_last_step = 5;
    temp_last_beat = 3;
}