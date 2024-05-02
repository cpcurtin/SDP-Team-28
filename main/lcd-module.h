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

/*******************************************************************************
******************************  LCD BANNER MODES  ******************************
*******************************************************************************/
#define BANNER_DEFAULT 0
#define BANNER_NAV_NAME 1
#define BANNER_TRACK_SAVE 2
#define BANNER_MEASURE_SWAP 3

#define LCD_PERSIST 0
#define LCD_VANISH 1

#define LCD_DEFAULT 0
#define LCD_SPLASH_TIMED 1
#define LCD_BANNER_TIMED 2
#define LCD_SPLASH 3

/******************************************************************************/
#define MAX_MEASURE_STEPS 6
#define VANISH_PERIOD 1500
/******************************************************************************/
struct lcd_pin_config
{
    const int i2c;
    const int rows;
    const int columns;
};

unsigned long timed_splash_start = 0;
unsigned long timed_banner_start = 0;

int lcd_mode = 0;

LiquidCrystal_I2C *lcd;
LiquidCrystal_I2C *lcd_init(const struct lcd_pin_config *cfg);

std::string lcd_banner;
std::vector<std::string> state_splash_screen(4, "");
std::vector<std::string> empty_splash(4, "");
std::vector<std::string> init_splash = {"Welcome to",
                                        "Moduloop",
                                        "",
                                        "SDP team 28"};
std::vector<std::string> delete_step_splash = {"SELECT MEASURE STEP", "TO DELETE ALL SOUNDS", "", ""};
std::vector<std::string> selected_sound = {"SELECTED SOUND:", "", "", "ASSIGN TO PALLETE"};
std::vector<std::string> selected_effect = {"SELECTED EFFECT:", "", "", ""};
std::vector<std::string> error_psram_full = {"PALETTE CACHE FULL", "YOU MUST REMOVE A", "CUSTOM SOUND FROM", "PALETTE AND MEASURE"};
std::vector<std::string> empty_step_splash = {"", "   STEP IS EMPTY   ", "", ""};
std::vector<std::string> empty_palette_splash = {"", " PALETTE SLOT EMPTY ", "", ""};
std::vector<std::string> track_saved_splash = {"    TRACK SAVED:    ", "", "", ""};


















void lcd_display(LiquidCrystal_I2C *lcd, std::vector<std::string> print_arr);
void lcd_splash(LiquidCrystal_I2C *lcd, struct Nav *current_nav, std::vector<std::string> print_arr);
void lcd_splash_step(LiquidCrystal_I2C *lcd, struct Step *step);
void lcd_splash_palette(LiquidCrystal_I2C *lcd, struct Palette_Slot &slot);
void lcd_display_banner(LiquidCrystal_I2C *lcd, int type, int mode);

#endif // LCD_MODULE_H