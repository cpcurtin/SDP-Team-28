/*
 *
 *       example function library
 *
 *
 */
#ifndef EFFECT_MODULE_H
#define EFFECT_MODULE_H

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
bool effect_mode = false;
// TEMMP
int active_palette_effects[3] = {DOUBLE_REPEAT, EFFECT_REPLAY_RESET, EFFECT_REVERSE_CURRENT};
// TEMMP

std::vector<std::string> fetch_effects();
// functions, extern variables, structs go here

void run_effect(int effect);
void effect_begin(void);
void effect_end(void);

#endif // EFFECT_MODULE_H