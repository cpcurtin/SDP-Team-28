/*
 *
 *       LCD function library
 *
 *
 */
#ifndef LCD_MODULE_H
#define LCD_MODULE_H
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#define MAX_MEASURE_STEPS 6

struct nav_config
{
    struct array_with_size *sounds_custom;
    struct array_with_size *effects;
    struct array_with_size *tracks_load;
    struct array_with_size *sounds_midi_melodic;
    struct array_with_size *sounds_midi_percussion;
};
typedef struct lcd_nav
{
    char *name;
    const char **data_array;
    struct lcd_nav *parent;
    struct lcd_nav **child;
    size_t size;
    const char **lcd_state;
    int index;

} lcd_nav;
struct lcd_pin_config
{
    const int i2c;
    const int rows;
    const int columns;
};

// NAV STRUCT INITIALIZATION
lcd_nav *nav_state = new lcd_nav;
lcd_nav *main_nav = new lcd_nav;
lcd_nav *sounds_nav = new lcd_nav;
lcd_nav *effects_nav = new lcd_nav;
lcd_nav *tracks_nav = new lcd_nav;
lcd_nav *sounds_custom_nav = new lcd_nav;
lcd_nav *sounds_midi_nav = new lcd_nav;
lcd_nav *tracks_load_nav = new lcd_nav;
lcd_nav *tracks_set_steps_nav = new lcd_nav;

lcd_nav *sounds_midi_melodic_nav = new lcd_nav;
lcd_nav *sounds_midi_percussion_nav = new lcd_nav;
lcd_nav *sounds_midi_octaves_nav = new lcd_nav;
lcd_nav *sounds_midi_notes_nav = new lcd_nav;
const char *octaves[] = {"-2 LOWEST", "-1", "0", "1", "2", "3", "4", "5", "6", "7", "8 HIGHEST"};
const char *note_names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
const char *selected_sound[] = {"SELECTED SOUND:", "", "", "STEPS: "};
const char *selected_sound[] = {"SELECTED EFFECT:", "", "", ""};

struct nav_config *nav_cfg = new struct nav_config;

LiquidCrystal_I2C *lcd_init(const struct lcd_pin_config *cfg);
void lcd_display(LiquidCrystal_I2C *lcd, const char **print_arr);
void array_scroll(lcd_nav *nav, int direction);
lcd_nav *nav_selection(lcd_nav *nav, int direction);
const char *format_row(const char **data_array, int index, int format);
lcd_nav *nav_init(struct nav_config *cfg);
void nav_add(lcd_nav *node);
const char *tracks_update(void);
void update_tempo(LiquidCrystal_I2C *lcd);

void lcd_splash(const char **print_arr);

#endif // LCD_MODULE_H