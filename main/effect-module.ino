/*
 *	test function library
 *
 *
 *
 */
#include "effect-module.h"

#if USING_SAFE_STRINGS == 1 // safe - new
std::vector<std::string> fetch_effects(void)
{
    std::vector<std::string> nav_effects = {"effect1", "effect2"};
    return nav_effects;
}

#else // unsafe - old

array_with_size *fetch_effects(void)
{
    const char **nav_effects = new const char *[2];
    nav_effects[0] = strdup("effect1");
    nav_effects[1] = strdup("effect2");

    effect_list->array = nav_effects;
    effect_list->size = 2;
    return effect_list;
}
#endif

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
        break;
    }
}
