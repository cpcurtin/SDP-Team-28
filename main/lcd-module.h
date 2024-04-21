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
#include <string>

#define MAX_MEASURE_STEPS 6
#define TIMED_SPLASH_SCREEN_PERIOD 2000

struct lcd_pin_config
{
    const int i2c;
    const int rows;
    const int columns;
};

bool splash_screen_active = false;
bool splash_screen_timed = false;
unsigned long timed_splash_start = 0;

LiquidCrystal_I2C *lcd;
LiquidCrystal_I2C *lcd_init(const struct lcd_pin_config *cfg);

std::vector<std::string> state_splash_screen(4, "");

std::vector<std::string> selected_sound = {"SELECTED SOUND:", "", "", "ASSIGN TO PALLETE"};
std::vector<std::string> selected_effect = {"SELECTED EFFECT:", "", "", ""};
std::vector<std::string> error_psram_full = {"PALETTE CACHE FULL", "YOU MUST REMOVE A", "CUSTOM SOUND FROM", "PALETTE AND MEASURE"};
std::vector<std::string> empty_step_splash = {"", "   STEP IS EMPTY   ", "", ""};

void lcd_display(LiquidCrystal_I2C *lcd, std::vector<std::string> print_arr);
void lcd_splash(LiquidCrystal_I2C *lcd, struct Nav *current_nav, std::vector<std::string> print_arr);
void lcd_splash_step(LiquidCrystal_I2C *lcd, struct Step *step);
void update_tempo(LiquidCrystal_I2C *lcd);

#endif // LCD_MODULE_H