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
    struct array_with_size *sounds_midi;
    struct array_with_size *effects;
    struct array_with_size *tracks_load;
};
typedef struct lcd_nav
{
    char *name;
    const char **ptr_str_array;
    struct lcd_nav *parent;
    struct lcd_nav **child;
    size_t size;
    const char **lcd_state;
    int index;
    int depth;
} lcd_nav;
struct lcd_pin_config
{
    const int i2c;
    const int rows;
    const int columns;
};

int lcd_rows;
struct nav_config *nav_cfg = new struct nav_config;

LiquidCrystal_I2C *lcd_init(const struct lcd_pin_config *cfg);
void lcd_display(LiquidCrystal_I2C *lcd, const char **print_arr);
void array_scroll(lcd_nav *nav, int direction);
lcd_nav *nav_selection(lcd_nav *nav, int direction);
const char *format_row(const char **ptr_str_array, int index, int format);
lcd_nav *nav_init(struct nav_config *cfg);
void nav_add(lcd_nav *node);
const char *tracks_update(void);
void update_tempo(LiquidCrystal_I2C *lcd);

#endif // LCD_MODULE_H