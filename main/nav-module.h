/*
 *
 *       example function library
 *
 *
 */
#ifndef NAV_MODULE_H
#define NAV_MODULE_H

struct nav_config
{
    struct array_with_size *sounds_custom;
    struct array_with_size *effects;
    struct array_with_size *tracks_load;
    struct array_with_size *sounds_midi_melodic;
    struct array_with_size *sounds_midi_percussion;
};

typedef struct Nav
{
    char *name;
    const char **data_array;
    struct Nav *parent;
    struct Nav **child;
    size_t size;
    const char **lcd_state;
    int index;

} Nav;

// NAV STRUCT INITIALIZATION
Nav *nav_state = new Nav;
Nav *main_nav = new Nav;
Nav *sounds_nav = new Nav;
Nav *effects_nav = new Nav;
Nav *tracks_nav = new Nav;
Nav *sounds_custom_nav = new Nav;
Nav *sounds_midi_nav = new Nav;
Nav *tracks_load_nav = new Nav;
Nav *tracks_set_steps_nav = new Nav;

Nav *sounds_midi_melodic_nav = new Nav;
Nav *sounds_midi_percussion_nav = new Nav;
Nav *sounds_midi_octaves_nav = new Nav;
Nav *sounds_midi_notes_nav = new Nav;

struct nav_config *nav_cfg = new struct nav_config;
// functions, extern variables, structs go here
Nav *nav_selection(Nav *nav, int direction);
Nav *nav_init(struct nav_config *cfg);

void array_scroll(Nav *nav, int direction);
const char *format_row(const char **data_array, int index, int format);

void nav_add(Nav *node);
const char *tracks_update(void);

#endif // EXAMPLE_MODULE_H