/*
 *	test function library
 *
 *
 *
 */
#include "effect-module.h"

std::vector<std::string> fetch_effects(void)
{
    std::vector<std::string> nav_effects = {"REPLAY SAVE", "REPLAY CURRENT", "REPLAY RESET", "REVERSE SAVE", "REVERSE CURRENT", "REVERSE RESET", "ECHO", "PAUSE SOUNDS", "SCRATCH", "DOUBLE REPEAT"};
    return nav_effects;
}

void effect_begin(void)
{
    Serial.println("BEGIN EFFECT");
    effect_mode = true;
    effect = testing_palette_combined[palette_index].effect;
    if (!testing_palette_combined[palette_index].sound.empty)
    {
        LED_mode = LED_SOUND_SWEEP;
    }
}
void effect_end(void)
{

    Serial.println("END EFFECT");
    effect_mode = false;

    // SET STEP STATE TO STEP WHEN EFFECT FIRST PRESSED
    if (effect_return_state == EFFECT_RETURN_SAVE)
    {
        current_measure->beat = saved_beat;
        current_measure->step = saved_step;
        active_step = &(current_measure->beat_list[saved_beat].step_list[saved_step]);
    }

    // SET STEP STATE TO BEAT=0 STEP=0
    else if (effect_return_state == EFFECT_RETURN_RESET)
    {
        current_measure->beat = 0;
        current_measure->step = 0;
    }

    else if (effect_return_state == ECHO)
    {
        volume = 127;
        dac_vol = 1.0;
        amp1.gain(dac_vol);
        amp2.gain(dac_vol);
    }

    else if (effect_return_state == PAUSE_SOUNDS)
    {
        silent = 0;
    }

    else if (effect_return_state == SCRATCH)
    {
        evenodd = 0;
    }
    else if (effect_return_state == DOUBLE_REPEAT)
    {
        evenodd = 0;
        effect_return_state = -1;
    }

    // ELSE, LEAVE STEP STATE AT LAST EFFECT
}
void run_effect(int effect)
{
    switch (effect)
    {
    case EFFECT_REPLAY_SAVE:
    {
        Serial.println("EFFECT_REPLAY_SAVE");
        effect_return_state = EFFECT_RETURN_SAVE;
        break;
    }

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

    case EFFECT_REVERSE_SAVE:
    {
        Serial.println("EFFECT_REVERSE_SAVE");
        effect_return_state = EFFECT_RETURN_SAVE;
        saved_step = current_measure->step;
        saved_beat = current_measure->beat;
        active_step = previous_step(current_measure);
        break;
    }

    case EFFECT_REVERSE_CURRENT:
    {
        Serial.println("EFFECT_REVERSE_CURRENT");
        effect_return_state = EFFECT_RETURN_CURRENT;
        active_step = previous_step(current_measure);
        break;
    }

    case EFFECT_REVERSE_RESET:
    {
        Serial.println("EFFECT_REVERSE_RESET");
        effect_return_state = EFFECT_RETURN_RESET;
        active_step = previous_step(current_measure);
        break;
    }

    case ECHO:
    {
        Serial.println("ECHO");
        effect_return_state = ECHO;
        if (volume != 7 && volume != 0)
        {
            volume = volume - 10;
        }
        if (volume == 7)
        {
            volume = 0;
        }
        if (dac_vol != 0)
        {
            dac_vol = dac_vol - 0.1;
        }

        amp1.gain(dac_vol);
        amp2.gain(dac_vol);
        break;
    }

    case PAUSE_SOUNDS:
    {
        Serial.println("PAUSE_SOUNDS");
        effect_return_state = PAUSE_SOUNDS;
        silent = 1;
        active_step = next_step(current_measure);
        break;
    }
    case SCRATCH:
    {
        Serial.println("SCRATCH");
        effect_return_state = SCRATCH;
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
    case DOUBLE_REPEAT:
    {
        Serial.println("DOUBLE_REPEAT");
        effect_return_state = DOUBLE_REPEAT;
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