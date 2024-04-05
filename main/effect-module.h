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
#define EFFECT_REPLAY_SAVE 1
#define EFFECT_REPLAY_CURRENT 2
#define EFFECT_REPLAY_RESET 3
#define EFFECT_REVERSE_SAVE 4
#define EFFECT_REVERSE_CURRENT 5
#define EFFECT_REVERSE_RESET 6

#define EFFECT_RETURN_SAVE 0
#define EFFECT_RETURN_CURRENT 1
#define EFFECT_RETURN_RESET 2

int effect;
int effect_return_state;
int saved_step;
int saved_beat;

// TEMMP
int active_palette_effects[3] = {EFFECT_REPLAY_CURRENT, EFFECT_REPLAY_RESET, EFFECT_REVERSE_CURRENT};
// TEMMP

// functions, extern variables, structs go here
array_with_size *effect_list = new array_with_size;

array_with_size *fetch_effects(void);
void run_effect(int effect);

#endif // EFFECT_MODULE_H