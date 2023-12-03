/*
 *
 *       LCD function library
 *
 *
 */
#ifndef LCD_MODULE_H
#define LCD_MODULE_H
#include <LiquidCrystal.h>

// struct lcd_pin_config {
//  const int rs;
//  const int en;
//  const int dig4;
//  const int dig5;
//  const int dig6;
//  const int dig7;
// };



LiquidCrystal* lcd_init(const struct lcd_pin_config &cfg);
void lcd_display(LiquidCrystal *lcd, char **print_arr);
void array_scroll(struct lcd_nav *nav, int direction);
struct lcd_nav* nav_selection(struct lcd_nav *nav, int direction);
struct lcd_nav *nav_init(struct nav_config *cfg);


#endif // LCD_MODULE_H