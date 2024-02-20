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

// struct lcd_pin_config {
//  const int rs;
//  const int en;
//  const int dig4;
//  const int dig5;
//  const int dig6;
//  const int dig7;
// };

LiquidCrystal_I2C *lcd_init(const struct lcd_pin_config *cfg);
void lcd_display(LiquidCrystal_I2C *lcd, char **print_arr);
void array_scroll(struct lcd_nav *nav, int direction);
struct lcd_nav *nav_selection(struct lcd_nav *nav, int direction);
char *format_row(char **ptr_str_array, int index, int format);
struct lcd_nav *nav_init(struct nav_config *cfg);

#endif // LCD_MODULE_H