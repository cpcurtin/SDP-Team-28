/*
 *
 *       example function library
 *
 *
 */
#ifndef EFFECT_MODULE_H
#define EFFECT_MODULE_H

/* EFFECT CODES */
#define EFFECT_NONE 0
#define EFFECT_REPLAY 6
#define EFFECT_REVERSE_RESET 7
#define EFFECT_REVERSE_CURRENT 8

int effect;

int saved_step;
int saved_beat;

// functions, extern variables, structs go here
array_with_size *effect_list = new array_with_size;

array_with_size *fetch_effects(void);
void run_effect(int effect);

#endif // EFFECT_MODULE_H