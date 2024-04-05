/*
 *	test function library
 *
 *
 *
 */
#include "effect-module.h"

array_with_size *fetch_effects(void)
{
    const char **nav_effects = new const char *[2];
    nav_effects[0] = strdup("effect1");
    nav_effects[1] = strdup("effect2");

    effect_list->array = nav_effects;
    effect_list->size = 2;
    return effect_list;
}

void run_effect(int effect)
{
    switch (effect)
    {
    case EFFECT_REPLAY_SAVE:
        Serial.println("EFFECT_REPLAY_SAVE");
        effect_return_state = EFFECT_RETURN_SAVE;

        break;
    case EFFECT_REPLAY_CURRENT:
        Serial.println("EFFECT_REPLAY_CURRENT");
        effect_return_state = EFFECT_RETURN_CURRENT;

        break;
    case EFFECT_REPLAY_RESET:
        Serial.println("EFFECT_REPLAY_RESET");
        effect_return_state = EFFECT_RETURN_RESET;

        break;
    case EFFECT_REVERSE_SAVE:
        Serial.println("EFFECT_REVERSE_SAVE");
        effect_return_state = EFFECT_RETURN_SAVE;

        saved_step = testing_measure.step;
        saved_beat = testing_measure.beat;
        active_step = previous_step(&testing_measure);
        break;
    case EFFECT_REVERSE_CURRENT:
        Serial.println("EFFECT_REVERSE_CURRENT");
        effect_return_state = EFFECT_RETURN_CURRENT;

        active_step = previous_step(&testing_measure);
        break;
    case EFFECT_REVERSE_RESET:
        Serial.println("EFFECT_REVERSE_RESET");
        effect_return_state = EFFECT_RETURN_RESET;
        active_step = previous_step(&testing_measure);

        break;
    default:
        break;
    }
}
