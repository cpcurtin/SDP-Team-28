/*
 *
 *       example function library
 *
 *
 */
#ifndef EFFECT_MODULE_H
#define EFFECT_MODULE_H
#ifndef USING_SAFE_STRINGS
#define USING_SAFE_STRINGS 1
#endif

/* EFFECT CODES */
#define EFFECT_NONE 0
#define EFFECT_REPLAY_SAVE 1
#define EFFECT_REPLAY_CURRENT 2
#define EFFECT_REPLAY_RESET 3
#define EFFECT_REVERSE_SAVE 4
#define EFFECT_REVERSE_CURRENT 5
#define EFFECT_REVERSE_RESET 6
#define ECHO 7
#define PAUSE_SOUNDS 8
#define SCRATCH 9
#define DOUBLE_REPEAT 10

#define EFFECT_RETURN_SAVE 0
#define EFFECT_RETURN_CURRENT 1
#define EFFECT_RETURN_RESET 2

int effect;
int effect_return_state;
int saved_step;
int saved_beat;

// TEMMP
int active_palette_effects[3] = {DOUBLE_REPEAT, EFFECT_REPLAY_RESET, EFFECT_REVERSE_CURRENT};
// TEMMP

#if USING_SAFE_STRINGS == 1 // safe - new
std::vector<const char *> fetch_effects();
#else // unsafe - old
array_with_size *effect_list = new array_with_size;
array_with_size *fetch_effects(void);
#endif
// functions, extern variables, structs go here

void run_effect(int effect);

#endif // EFFECT_MODULE_H