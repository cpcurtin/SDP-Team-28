/*
 *
 *       LCD function library
 *
 *
 */
#ifndef LCD_MODULE_H
#define LCD_MODULE_H

#ifndef USING_SAFE_STRINGS
#define USING_SAFE_STRINGS 1
#endif

#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#define MAX_MEASURE_STEPS 6

struct lcd_pin_config
{
    const int i2c;
    const int rows;
    const int columns;
};

bool splash_screen_active = false;

LiquidCrystal_I2C *lcd_init(const struct lcd_pin_config *cfg);

#if USING_SAFE_STRINGS == 1 // safe - new

std::vector<const char *> state_splash_screen(4, nullptr);

std::vector<const char *> selected_sound = {"SELECTED SOUND:", "", "", "ASSIGN TO PALLETE"};
std::vector<const char *> selected_effect = {"SELECTED EFFECT:", "", "", ""};
std::vector<const char *> error_psram_full = {"PALETTE CACHE FULL", "YOU MUST REMOVE A", "CUSTOM SOUND FROM", "PALETTE AND MEASURE"};

void lcd_display(LiquidCrystal_I2C *lcd, std::vector<const char *> print_arr);
void lcd_splash(LiquidCrystal_I2C *lcd, struct Nav *current_nav, std::vector<const char *> print_arr);

#else // unsafe - old

const char **state_splash_screen = new const char *[4];
const char *selected_sound[] = {"SELECTED SOUND:", "", "", "ASSIGN TO PALLETE"};
const char *selected_effect[] = {"SELECTED EFFECT:", "", "", ""};
const char *error_psram_full[] = {"PALETTE CACHE FULL", "YOU MUST REMOVE A", "CUSTOM SOUND FROM", "PALETTE AND MEASURE"};
void lcd_display(LiquidCrystal_I2C *lcd, const char **print_arr);
void lcd_splash(LiquidCrystal_I2C *lcd, struct Nav *current_nav, const char **print_arr);
#endif

void update_tempo(LiquidCrystal_I2C *lcd);
#endif // LCD_MODULE_H