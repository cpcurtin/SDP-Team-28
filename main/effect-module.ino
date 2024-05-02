/*
 *	test function library
 *
 *
 *
 */
#include "effect-module.h"

std::vector<std::string> fetch_effects(void)
{
    std::vector<std::string> nav_effects = {"REPEAT RETURN", "REPEAT CONTINUE", "REVERSE", "EFFECT_ECHO", "MUTE", "EFFECT_SCRATCH", "DOUBLE STEP"};
    return nav_effects;
}

void effect_begin(void)
{
    Serial.println("BEGIN EFFECT");
    effect_mode = true;
    effect = testing_palette_combined[palette_index].effect;
    if (effect != EFFECT_NULL)
    {
        lcd_splash_palette(lcd, testing_palette_combined[palette_index]); // avoid clearing select splash
    }
    else if (!testing_palette_combined[palette_index].sound.empty)
    {
        LED_mode = LED_SOUND_SWEEP;
    }
    else if (effect == EFFECT_REVERSE_CURRENT) // set chained measure as repeat
    {
        for (int i = 0; i < current_track->active_measures; i++)
        {
            if (current_track->measure_list[i]->id != current_measure->id)
            {
                current_track->measure_list[i]->beat = current_track->measure_list[i]->active_beats - 1;
                current_track->measure_list[i]->step = current_track->measure_list[i]->beat_list[current_track->measure_list[i]->beat].active_steps - 1;
            }
        }
    }
    LED_routine(matrix_button.row, matrix_button.column); // light up pressed palette button
}
void effect_end(void)
{

    Serial.println("END EFFECT");
    effect_mode = false;
    switch (effect_return_state)
    {
    case EFFECT_RETURN_SAVE:
    {
        // SET STEP STATE TO STEP WHEN EFFECT FIRST PRESSED
        current_measure = current_track->measure_list[saved_measure];
        current_measure->beat = saved_beat;
        current_measure->step = saved_step;
        active_step = &(current_measure->beat_list[saved_beat].step_list[saved_step]);
        break;
    }
    case EFFECT_RETURN_RESET:
    {
        // SET STEP STATE TO BEAT=0 STEP=0
        current_measure->beat = 0;
        current_measure->step = 0;
        break;
    }
    case EFFECT_ECHO:
    {
        volume = 127;
        dac_vol = 0.6;
        amp1.gain(dac_vol);
        // amp2.gain(dac_vol);
        break;
    }
    case EFFECT_PAUSE_SOUNDS:
    {
        silent = 0;
        break;
    }
    case EFFECT_SCRATCH:
    {
        evenodd = 0;
        break;
    }
    case EFFECT_DOUBLE_REPEAT:
    {
        evenodd = 0;
        effect_return_state = -1;
        break;
    }
    default:
    {
        // ELSE, LEAVE STEP STATE AT LAST EFFECT
        for (int i = 0; i < current_track->active_measures; i++)
        {
            if (current_track->measure_list[i]->id != current_measure->id)
            {
                current_track->measure_list[i]->beat = 0;
                current_track->measure_list[i]->step = 0;
            }
        }
        break;
    }
    }
}
void run_effect(int effect)
{
    switch (effect)
    {
    case EFFECT_REPLAY_CURRENT:
    {
        Serial.println("EFFECT_REPLAY_CURRENT");
        effect_return_state = EFFECT_RETURN_CURRENT;
        break;
    }

    case EFFECT_REPLAY_RESET:
    {
        Serial.println("EFFECT_REPLAY_RESET");
        effect_return_state = EFFECT_RETURN_RESET;
        break;
    }

    case EFFECT_REVERSE_CURRENT:
    {
        Serial.println("EFFECT_REVERSE_CURRENT");
        effect_return_state = EFFECT_RETURN_CURRENT;
        active_step = previous_step(current_measure);
        break;
    }

    case EFFECT_ECHO:
    {
        Serial.println("EFFECT_ECHO");
        effect_return_state = EFFECT_ECHO;
        if (volume != 7 && volume != 0)
        {
            volume = volume - 10;
        }
        if (volume == 7)
        {
            volume = 0;
        }
        if (dac_vol > 0.1)
        {
            dac_vol = dac_vol - 0.1;
        }

        amp1.gain(dac_vol);
        // amp2.gain(dac_vol);
        Serial.println(dac_vol);
        break;
    }

    case EFFECT_PAUSE_SOUNDS:
    {
        Serial.println("EFFECT_PAUSE_SOUNDS");
        effect_return_state = EFFECT_PAUSE_SOUNDS;
        silent = 1;
        active_step = next_step(current_measure);
        break;
    }
    case EFFECT_SCRATCH:
    {
        Serial.println("EFFECT_SCRATCH");
        effect_return_state = EFFECT_SCRATCH;
        if ((evenodd % 2) == 0)
        {
            active_step = previous_step(current_measure);
        }
        else
        {
            active_step = next_step(current_measure);
        }
        evenodd++;
        break;
    }
    case EFFECT_DOUBLE_REPEAT:
    {
        Serial.println("EFFECT_DOUBLE_REPEAT");
        effect_return_state = EFFECT_DOUBLE_REPEAT;
        if ((evenodd % 2) == 1)
        {
            active_step = next_step(current_measure);
        }
        evenodd++;
        break;
    }
    default:
    {
        Serial.println("DEFAULT EFFECT");
        active_step = next_step(current_measure);
        effect_return_state = EFFECT_RETURN_CURRENT;
        break;
    }
    }
}