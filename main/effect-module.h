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
#define EFFECT_NULL -1
#define EFFECT_REPLAY_SAVE 0
#define EFFECT_REPLAY_CURRENT 1
#define EFFECT_REPLAY_RESET 2
#define EFFECT_REVERSE_SAVE 3
#define EFFECT_REVERSE_CURRENT 4
#define EFFECT_REVERSE_RESET 5
#define ECHO 6
#define PAUSE_SOUNDS 7
#define SCRATCH 8
#define DOUBLE_REPEAT 9

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
std::vector<std::string> fetch_effects();
#else // unsafe - old
array_with_size *effect_list = new array_with_size;
array_with_size *fetch_effects(void);
#endif
// functions, extern variables, structs go here

void run_effect(int effect);
void effect_begin(void);
void effect_end(void);

#endif // EFFECT_MODULE_H