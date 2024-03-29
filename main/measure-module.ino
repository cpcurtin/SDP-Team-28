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
            }
        }
    }

    // INIT PALETTE
    for (int i = 0; i < PALETTE_SIZE; i++)
    {
        testing_palette[i] = {-1, -1, -1, nullptr};
    }
}

Step button_to_step(int actuated_button[])
{
    if (actuated_button[0] != 9 && actuated_button[1] < 6)
    {
        return (testing_measure.beat_list[actuated_button[0]]).step_list[actuated_button[1]];
    }
    else
    {
        return defaultStep;
    }
}
