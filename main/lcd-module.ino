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
  // lcd->setCursor(0, LCD_ROWS - 1); // set cursor to row 0
  // Serial.println(lcd_banner.c_str());
  // lcd->print(lcd_banner.c_str()); // print to row 0
  Serial.println("XXXXXXXXXXXXXXXXXXXX");

  lcd->home();
}

void lcd_splash(LiquidCrystal_I2C *lcd, struct Nav *current_nav, std::vector<std::string> print_arr)
{

  lcd_mode = LCD_SPLASH;
  state_splash_screen.clear();
  std::copy(print_arr.begin(), print_arr.end(), std::back_inserter(state_splash_screen));

  // if (current_nav == nullptr)
  // {
  //   Serial.println("size too large");
  //   // Maybe find out what sounds are the largest and display them
  // }

  switch (current_nav->id)
  {
  case NAVIGATION_MIDI_NOTES:
  {
    if (state_splash_screen.size() >= 2)
    {
      state_splash_screen[1] = sounds_midi_melodic_nav->data_array[sounds_midi_melodic_nav->index];
    }
    if (state_splash_screen.size() >= 3)
    {
      state_splash_screen[2] = "Octave:" + std::to_string(sounds_midi_octaves_nav->index - 2) + " Note:" + std::string(sounds_midi_notes_nav->data_array[sounds_midi_notes_nav->index]);
    }
    break;
  }

  case NAVIGATION_SOUNDS_MIDI_PERCUSSION:
  {
    if (state_splash_screen.size() >= 2)
    {
      state_splash_screen[1] = sounds_midi_percussion_nav->data_array[sounds_midi_percussion_nav->index];
    }
    break;
  }
  case NAVIGATION_SOUNDS_CUSTOM:
  {
    if (state_splash_screen.size() >= 2)
    {
      state_splash_screen[1] = sounds_custom_nav->data_array[sounds_custom_nav->index];
    }
    break;
  }
  case NAVIGATION_EFFECTS:
  {
    if (state_splash_screen.size() >= 2)
    {
      state_splash_screen[1] = effects_nav->data_array[effects_nav->index];
    }
    break;
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
void lcd_splash_step(LiquidCrystal_I2C *lcd, struct Step *step)
{

  lcd_mode = LCD_SPLASH_TIMED;
  timed_splash_start = millis();
  state_splash_screen.clear();

  if (step->active_sounds > 0)
  {
    for (int sound = 0; sound < MAX_STEP_SOUNDS; sound++)
    {
      if (step->sound_list[sound].empty == false)
      {
        if (step->sound_list[sound].bank != MIDI_NULL)
        {
          if (step->sound_list[sound].note != MIDI_NULL)
          {
            // MELODIC
            state_splash_screen.push_back(midi_melodic_sounds[step->sound_list[sound].instrument]);
          }
          else
          {
            // PERCUSSION
            state_splash_screen.push_back(midi_percussion_sounds[step->sound_list[sound].instrument]);
          }
        }
        else
        {
          // SD
          if (step->sound_list[sound].sd_cached_sound != nullptr)
          {
            state_splash_screen.push_back(step->sound_list[sound].filename);
          }
        }
      }
      else
      {
        state_splash_screen.push_back(std::string(LCD_COLUMNS, ' '));
      }
    }
  }
  else
  {
    std::copy(empty_step_splash.begin(), empty_step_splash.end(), std::back_inserter(state_splash_screen));
  }
  for (int sound = 0; sound < step->active_sounds; sound++)
  {
    state_splash_screen[sound] = format_row(state_splash_screen, sound, 2);
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

void lcd_splash_palette(LiquidCrystal_I2C *lcd, struct Palette_Slot &slot)
{

  state_splash_screen.clear();

  if (!slot.is_empty)
  {
    std::copy(empty_splash.begin(), empty_splash.end(), std::back_inserter(state_splash_screen));
    if (!slot.sound.empty)
    {
      lcd_mode = LCD_SPLASH;
      state_splash_screen[0] = "   PALETTE SOUND   ";
      if (slot.sound.bank != MIDI_NULL)
      {
        if (slot.sound.note != MIDI_NULL)
        {
          // MELODIC
          state_splash_screen[1] = "MIDI MELODIC SOUND:";
          state_splash_screen[2] = midi_melodic_sounds[slot.sound.instrument - midi_melodic_values[0]];
          state_splash_screen[3] = "Octave:" + std::to_string((slot.sound.note / NUM_OCTAVES) - 2) + " Note:" + sounds_midi_notes_nav->data_array[slot.sound.note % NUM_NOTES];
        }
        else
        {
          // PERCUSSION
          state_splash_screen[1] = "MIDI PERCUSSION SOUND:";
          state_splash_screen[2] = midi_percussion_sounds[slot.sound.instrument - midi_percussion_values[0]];
          state_splash_screen[3] = std::string(LCD_COLUMNS, ' ');
        }
      }
      else
      {
        // SD
        if (slot.sound.sd_cached_sound != nullptr)
        {
          state_splash_screen[1] = "CUSTOM SOUND:";
          state_splash_screen[2] = slot.sound.filename;
          state_splash_screen[3] = std::string(LCD_COLUMNS, ' ');
        }
      }
    }
    else
    {
      lcd_mode = LCD_SPLASH_TIMED;
      timed_splash_start = millis();
      state_splash_screen[0] = "  PALETTE EFFECT   ";
      state_splash_screen[1] = effects_nav->data_array[slot.effect];
      state_splash_screen[2] = std::string(LCD_COLUMNS, ' ');
      state_splash_screen[3] = std::string(LCD_COLUMNS, ' ');
    }
  }
  else
  {
    lcd_mode = LCD_SPLASH_TIMED;
    timed_splash_start = millis();
    std::copy(empty_palette_splash.begin(), empty_palette_splash.end(), std::back_inserter(state_splash_screen));
  }

  lcd->clear();
  Serial.println("XXXXXXXSPLASHXXXXXXX");
  for (size_t row = 0; row < state_splash_screen.size(); row++)
  {
    lcd->setCursor(0, row); // Set cursor to current row
    state_splash_screen[row].resize(LCD_COLUMNS);
    state_splash_screen[row].shrink_to_fit();
    Serial.println(state_splash_screen[row].c_str());
    lcd->print(state_splash_screen[row].c_str()); // Print current row
  }
  Serial.println("XXXXXXXXXXXXXXXXXXXX");

  lcd->home();
}

void lcd_display_banner(LiquidCrystal_I2C *lcd, int type, int mode)
{
  lcd->noBlink();
  if (mode == LCD_VANISH)
  {

    lcd_mode = LCD_BANNER_TIMED;
    timed_banner_start = millis();
  }
  lcd_banner.clear();
  switch (type)
  {
  case BANNER_DEFAULT:
  {
    lcd_banner = tracks_update();
    break;
  }
  case BANNER_NAV_NAME:
  {
    lcd_banner = nav_state->name;
    break;
  }
  case BANNER_TRACK_SAVE:
  {
    lcd_banner = "SAVE:TRACK-" + track_save_string + ".json";
    break;
  }
  case BANNER_MEASURE_SWAP:
  {
    lcd_banner = "SELECT MEASURE SWAP:" + std::to_string(measure_swap_panel + 1);
    break;
  }
  }

  lcd_banner.resize(LCD_COLUMNS);
  lcd_banner.shrink_to_fit();

  lcd->setCursor(0, LCD_ROWS - 1); // set cursor to last row, first column
  lcd->print(lcd_banner.c_str());
  lcd->home();
  if (type == BANNER_TRACK_SAVE)
  {
    lcd->setCursor(11 + track_save_panel, LCD_ROWS - 1);
    lcd->blink();
  }
}