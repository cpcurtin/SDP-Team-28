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

struct nav_config
{
    struct array_with_size *sounds_custom;
    struct array_with_size *sounds_midi;
    struct array_with_size *effects;
    struct array_with_size *tracks;
};
struct lcd_nav
{
    char *name;
    char **ptr_str_array;
    struct lcd_nav *parent;
    struct lcd_nav **child;
    size_t size;
    char **lcd_state;
    int index;
    int depth;
};
struct lcd_pin_config
{
    const int i2c;
    const int rows;
    const int columns;
};

LiquidCrystal_I2C *lcd_init(const struct lcd_pin_config *cfg);
void lcd_display(LiquidCrystal_I2C *lcd, char **print_arr);
void array_scroll(struct lcd_nav *nav, int direction);
struct lcd_nav *nav_selection(struct lcd_nav *nav, int direction);
char *format_row(char **ptr_str_array, int index, int format);
struct lcd_nav *nav_init(struct nav_config *cfg);

#endif // LCD_MODULE_H