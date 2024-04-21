/*
 *	LCD function library
 *
 *
 *
 */
#include "lcd-module.h"

LiquidCrystal_I2C *lcd_init(const struct lcd_pin_config *cfg)
{
  // welcome message
  const char *lcd_init_message[] = {
      "     Welcome to     ",
      "      Moduloop      ",
      "                    ",
      "    SDP team 28    "};

  LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C(cfg->i2c, cfg->columns, cfg->rows);

  lcd->init(); // initialize the lcd
  lcd->backlight();
  lcd->begin(cfg->columns, cfg->rows);
  lcd->clear();
  for (int row = 0; row < LCD_ROWS; row++)
  {
    lcd->setCursor(0, row); // set cursor to row 0

    lcd->print(lcd_init_message[row]); // print to row 0
  }
  lcd->home();

  return lcd;
}

void lcd_display(LiquidCrystal_I2C *lcd, std::vector<std::string> print_arr)
{
  lcd->clear();

  Serial.println("XXXXLCD DISPLAYXXXXX");
  for (int row = 0; row < LCD_ROWS - 1; row++)
  {
    lcd->setCursor(0, row); // set cursor to row 0
    Serial.println(print_arr[row].c_str());
    lcd->print(print_arr[row].c_str()); // print to row 0
  }
  Serial.println("XXXXXXXXXXXXXXXXXXXX");

  lcd->home();
}

void lcd_splash(LiquidCrystal_I2C *lcd, struct Nav *current_nav, std::vector<std::string> print_arr)
{
  splash_screen_active = true;
  state_splash_screen.clear();
  std::copy(print_arr.begin(), print_arr.end(), std::back_inserter(state_splash_screen));

  if (current_nav == nullptr)
  {
    Serial.println("size too large");
    // Maybe find out what sounds are the largest and display them
  }
  // else if (current_nav->name == "sounds_midi_notes")
  else if (current_nav->id == NAVIGATION_MIDI_NOTES)
  {
    if (state_splash_screen.size() >= 2)
    {
      state_splash_screen[1] = sounds_midi_melodic_nav->data_array[sounds_midi_melodic_nav->index];
    }
    if (state_splash_screen.size() >= 3)
    {
      state_splash_screen[2] = ("Octave:" + std::string(sounds_midi_octaves_nav->data_array[sounds_midi_octaves_nav->index]) + " Note:" + std::string(sounds_midi_notes_nav->data_array[sounds_midi_notes_nav->index])).c_str();
    }
  }
  // else if (current_nav->name == "sounds_midi_percussion")
  else if (current_nav->id == NAVIGATION_SOUNDS_MIDI_PERCUSSION)
  {
    if (state_splash_screen.size() >= 2)
    {
      state_splash_screen[1] = sounds_midi_percussion_nav->data_array[sounds_midi_percussion_nav->index];
    }
  }
  // else if (current_nav->name == "custom_sounds")
  else if (current_nav->id == NAVIGATION_SOUNDS_CUSTOM)
  {
    if (state_splash_screen.size() >= 2)
    {
      state_splash_screen[1] = sounds_custom_nav->data_array[sounds_custom_nav->index];
    }
  }
  else if (current_nav->id == NAVIGATION_EFFECTS)
  {
    if (state_splash_screen.size() >= 2)
    {
      state_splash_screen[1] = effects_nav->data_array[effects_nav->index];
    }
  }

  lcd->clear();
  Serial.println("XXXXXXXSPLASHXXXXXXX");
  for (size_t row = 0; row < state_splash_screen.size(); row++)
  {
    lcd->setCursor(0, row); // Set cursor to current row
    Serial.println(state_splash_screen[row].c_str());
    lcd->print(state_splash_screen[row].c_str()); // Print current row
  }
  Serial.println("XXXXXXXXXXXXXXXXXXXX");

  lcd->home();
}

void update_tempo(LiquidCrystal_I2C *lcd)
{
  lcd->setCursor(0, LCD_ROWS - 1); // set cursor to row 0
  lcd->print(tracks_update().c_str());
  lcd->home();
}