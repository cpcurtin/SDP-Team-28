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