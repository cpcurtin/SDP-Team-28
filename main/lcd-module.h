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

struct lcd_pin_config
{
    const int i2c;
    const int rows;
    const int columns;
};

const char **state_splash_screen = new const char *[4];

const char *octaves[] = {"-2 LOWEST", "-1", "0", "1", "2", "3", "4", "5", "6", "7", "8 HIGHEST"};
const char *note_names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
const char *selected_sound[] = {"SELECTED SOUND:", "", "", "ASSIGN TO PALLETE"};
const char *selected_effect[] = {"SELECTED EFFECT:", "", "", ""};
const char *error_psram_full[] = {"PALETTE CACHE FULL", "YOU MUST REMOVE A", "CUSTOM SOUND FROM", "PALETTE AND MEASURE"};

bool splash_screen_active = false;

LiquidCrystal_I2C *lcd_init(const struct lcd_pin_config *cfg);
void lcd_display(LiquidCrystal_I2C *lcd, const char **print_arr);

void lcd_splash(LiquidCrystal_I2C *lcd, struct Nav *current_nav, const char **print_arr);
void update_tempo(LiquidCrystal_I2C *lcd);
#endif // LCD_MODULE_H