/*
 *	example function library
 *
 *
 *
 */
#include "measure-module.h"

void measure_palette_init(void)
{
    Beat *temp_beat;
    Step *temp_step;
    Sound *temp_sound;

    // INIT STARTING MEASURE
    testing_measure.active_beats = MAX_BEATS;
    for (int i = 0; i < MAX_BEATS; i++)
    {
        // Initialize measure beats
        temp_beat = &testing_measure.beat_list[i];
        temp_beat->active_steps = MAX_STEPS;
        temp_beat->id = i;

        for (int j = 0; j < MAX_STEPS; j++)
        {
            // Initialize beat steps
            temp_step = &temp_beat->step_list[j];
            temp_step->active_sounds = 0;
            temp_step->id = (MAX_STEPS * i) + j;
            for (int k = 0; k < MAX_STEP_SOUNDS; k++)
            {
                // Initialize beat steps
                temp_sound = &temp_step->sound_list[k];
                temp_sound->bank = -1;
                temp_sound->instrument = -1;
                temp_sound->note = -1;
                temp_sound->sd_cached_sound = nullptr;
                temp_sound->empty = true;
            }
        }
    }

    // INIT PALETTE
    for (int i = 0; i < PALETTE_SIZE; i++)
    {
        testing_palette[i] = {-1, -1, -1, nullptr, true};
    }
}

Step button_to_step(int actuated_button[])
{
    if (actuated_button[ROW] < 4 && actuated_button[COLUMN] < 6)
    {
        return (testing_measure.beat_list[actuated_button[ROW]]).step_list[actuated_button[COLUMN]];
    }
    else
    {
        return defaultStep;
    }
}

Step next_step(Measure measure)
{
    step++;
    if (step == measure.beat_list[beat].active_steps)
    {
        step = 0;
        beat++;
        if (beat == measure.active_beats)
        {
            beat = 0;
        }
    }
    return measure.beat_list[beat].step_list[step];
}

int stop_step(Step step_end)
{
    for (int sound = 0; sound < MAX_STEP_SOUNDS; sound++)
    {
        if (step_end.sound_list[sound].empty == false)
        {
            if (step_end.sound_list[sound].bank != MIDI_NULL)
            {
                if (step_end.sound_list[sound].note != MIDI_NULL)
                {
                    // MELODIC
                    midiNoteOff(step_end.sound_list[sound].bank, step_end.sound_list[sound].note, 127);
                }
                else
                {
                    // PERCUSSION
                    midiNoteOff(step_end.sound_list[sound].bank, step_end.sound_list[sound].instrument, 127);
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

int play_step(Step step_play)
{
    for (int sound = 0; sound < MAX_STEP_SOUNDS; sound++)
    {
        if (step_play.sound_list[sound].empty == false)
        {
            if (step_play.sound_list[sound].bank != MIDI_NULL)
            {
                if (step_play.sound_list[sound].note != MIDI_NULL)
                {
                    // MELODIC
                    midiSetInstrument(step_play.sound_list[sound].bank, 128);
                    midiNoteOn(step_play.sound_list[sound].bank, step_play.sound_list[sound].note, 127);
                }
                else
                {
                    // PERCUSSION
                    midiSetInstrument(step_play.sound_list[sound].bank, 128);
                    midiNoteOn(step_play.sound_list[sound].bank, step_play.sound_list[sound].instrument, 127);
                }
            }
            else
            {
                // SD
                playFile(step_play.sound_list[sound].sd_cached_sound);
            }
        }
    }

    return 0;
}

float step_interval_calc(Measure measure)
{
    return (60000 / (active_track.bpm * measure.beat_list[beat].active_steps));
}

void print_measure(Measure measure)
{
    Beat *temp_beat;
    Step *temp_step;
    Sound *temp_sound;
    Serial.println("\n\n");
    Serial.print("Beat: ");
    Serial.print(beat);
    Serial.print("\tStep: ");
    Serial.println(step);

    for (int i = 0; i < MAX_STEP_SOUNDS; i++)
    {

        Serial.print("B: ");
        Serial.print(measure.beat_list[beat].step_list[step].sound_list[i].bank);
        Serial.print("\tI: ");
        Serial.print(measure.beat_list[beat].step_list[step].sound_list[i].instrument);
        Serial.print("\tN: ");
        Serial.println(measure.beat_list[beat].step_list[step].sound_list[i].note);
    }
}