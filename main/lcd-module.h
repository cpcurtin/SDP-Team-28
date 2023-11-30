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
void array_scroll(struct array_with_size *full_array, int direction);


#endif // LCD_MODULE_H