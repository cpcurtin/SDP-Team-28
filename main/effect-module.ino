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
    case EFFECT_REPLAY:
        Serial.println("REPLAY EFFECT");
        last_step = active_step;
        temp_last_step = testing_measure.step;
        temp_last_beat = testing_measure.beat;
        // active_step = next_step(&testing_measure);

        break;
    case EFFECT_REVERSE_RESET:
        last_step = active_step;
        temp_last_step = testing_measure.step;
        temp_last_beat = testing_measure.beat;
        active_step = previous_step(&testing_measure);
        break;
    case EFFECT_REVERSE_CURRENT:

        break;
    default:
        break;
    }
}
