/*
 *	LCD function library
 *
 *
 *
 */
#include "lcd-module.h"
// #include <LiquidCrystal.h>
int lcd_rows;

LiquidCrystal_I2C *lcd_init(const struct lcd_pin_config &cfg)
{
  // welcome message
  char **lcd_init_message = new char *[4];
  lcd_init_message[0] = strdup("     Welcome to     ");
  lcd_init_message[1] = strdup("      Moduloop      ");
  lcd_init_message[2] = strdup("");
  lcd_init_message[3] = strdup("    SDP team 28    ");

  // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
  // LiquidCrystal *lcd = new LiquidCrystal(cfg.rs, cfg.en, cfg.dig4, cfg.dig5, cfg.dig6, cfg.dig7);
  // lcd_rows = cfg.rows;
  // lcd->begin(cfg.columns, cfg.rows);
  // lcd->clear();
  // lcd_display(lcd, lcd_init_message);

  // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  // LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows
  // LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C(0x27, 20, 4);
  LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C(cfg.i2c, cfg.columns, cfg.rows);

  lcd->init(); // initialize the lcd
  lcd->backlight();
  lcd_rows = cfg.rows;
  lcd->begin(cfg.columns, cfg.rows);
  lcd->clear();
  lcd_display(lcd, lcd_init_message);

  // test_lcd->setCursor(0, 0);            // move cursor the first row
  // test_lcd->print("LCD 20x4");          // print message at the first row
  // test_lcd->setCursor(0, 1);            // move cursor to the second row
  // test_lcd->print("I2C Address: 0x27"); // print message at the second row
  // test_lcd->setCursor(0, 2);            // move cursor to the third row
  // test_lcd->print("TEEEEEEST");          // print message at the third row
  // test_lcd->setCursor(0, 3);            // move cursor to the fourth row
  // test_lcd->print("www.diyables.io");   // print message the fourth row
  // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

  return lcd;
}

void lcd_display(LiquidCrystal_I2C *lcd, char **print_arr)
{
  lcd->clear();
  for (int row = 0; row < lcd_rows; row++)
  {
    lcd->setCursor(0, row); // set cursor to row 0

    lcd->print(print_arr[row]); // print to row 0
  }
  lcd->home();
}

void array_scroll(struct lcd_nav *nav, int direction)
{

  int new_index;
  if (nav->index + direction < 0)
  {
    // new index loops in reverse from 0 to end
    new_index = (int)nav->size - 1;
  }
  else
  {
    // new index changes +/- and goes from end to 0
    new_index = (nav->index + direction) % nav->size;
  }

  nav->index = new_index;
  nav->lcd_state[0] = format_row(nav->ptr_str_array, new_index, 1);

  // THIS IS WHERE IT REPEATS
  for (int row = 1; row < lcd_rows; row++)
  {
    if (row < (int)nav->size)
    {
      int temp_index = (new_index + row) % nav->size;
      nav->lcd_state[row] = format_row(nav->ptr_str_array, temp_index, 0);
    }
    else
    {
      // nav->lcd_state[row] = format_row(nav->ptr_str_array, temp_index, 0);
      nav->lcd_state[row] = strdup("");
    }
  }
  // Serial.println("##############END SCROLL##############");
}

char *format_row(char **ptr_str_array, int index, int format)
{
  char *temp_str;
  char *temp_str2;

  temp_str = (char *)malloc(strlen(ptr_str_array[index]) + 7);
  temp_str2 = (char *)malloc(strlen(ptr_str_array[index]));
  // spacing, enumerated
  if (format == 0)
  {
    sprintf(temp_str, " %d ", index + 1);
  }
  if (format == 1)
  {
    sprintf(temp_str, ">%d ", index + 1);
  }
  strcpy(temp_str2, ptr_str_array[index]);
  strcat(temp_str, temp_str2);

  free(temp_str2);

  return temp_str;
}

struct lcd_nav *nav_selection(struct lcd_nav *nav, int direction)
{

  if (direction > 0)
  {
    if ((nav->child) != NULL)
    {
      Serial.println(nav->name);
      return nav->child[nav->index];
    }
  }
  if (direction < 0)
  {
    if ((nav->parent) != NULL)
    {
      Serial.println(nav->name);
      return nav->parent;
    }
  }

  return nav;
}

struct lcd_nav *nav_init(struct nav_config *cfg)
{

  char **nav_main = new char *[3];
  nav_main[0] = strdup("Sounds");
  nav_main[1] = strdup("Effects");
  nav_main[2] = strdup("Tracks");

  char **nav_sounds = new char *[2];
  nav_sounds[0] = strdup("Custom Sounds");
  nav_sounds[1] = strdup("MIDI Sounds");

  // struct instatiation
  struct lcd_nav *main = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *sounds = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *effects = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *tracks = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *sounds_custom = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *sounds_midi = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  char **state_main = (char **)malloc(2 * sizeof(char *));
  char **state_sounds = (char **)malloc(2 * sizeof(char *));
  char **state_effects = (char **)malloc(2 * sizeof(char *));
  char **state_tracks = (char **)malloc(2 * sizeof(char *));
  char **state_sounds_custom = (char **)malloc(2 * sizeof(char *));
  char **state_sounds_midi = (char **)malloc(2 * sizeof(char *));
  // char **state = new char *[2];

  // ptr arrays
  struct lcd_nav **main_child = new struct lcd_nav *[3];
  main_child[0] = sounds;
  main_child[1] = effects;
  main_child[2] = tracks;

  struct lcd_nav **sounds_child = new struct lcd_nav *[2];
  sounds_child[0] = sounds_custom;
  sounds_child[1] = sounds_midi;

  // main
  main->name = strdup("main");
  main->ptr_str_array = nav_main;
  main->parent = NULL;
  main->child = main_child;
  main->size = 3;
  main->lcd_state = state_main;
  main->index = 0;
  main->depth = 0;
  array_scroll(main, 0);

  // sounds
  sounds->name = strdup("sounds");
  sounds->ptr_str_array = nav_sounds;
  sounds->parent = main;
  sounds->child = sounds_child;
  sounds->size = 2;
  sounds->lcd_state = state_sounds;
  sounds->depth = 1;
  sounds->index = 0;
  array_scroll(sounds, 0);

  // effects
  effects->name = strdup("effects");
  effects->ptr_str_array = (cfg->effects)->array;
  effects->parent = main;
  effects->child = NULL;
  effects->size = (cfg->effects)->size;
  effects->lcd_state = state_effects;
  effects->depth = 1;
  effects->index = 0;
  array_scroll(effects, 0);

  // tracks
  tracks->name = strdup("tracks");
  tracks->ptr_str_array = (cfg->tracks)->array;
  tracks->parent = main;
  tracks->child = NULL;
  tracks->size = (cfg->tracks)->size;
  tracks->lcd_state = state_tracks;
  tracks->depth = 1;
  tracks->index = 0;
  array_scroll(tracks, 0);

  // custom_sounds
  sounds_custom->name = strdup("custom_sounds");
  sounds_custom->ptr_str_array = (cfg->sounds_custom)->array;
  sounds_custom->parent = sounds;
  sounds_custom->child = NULL;
  sounds_custom->size = (cfg->sounds_custom)->size;
  sounds_custom->lcd_state = state_sounds_custom;
  sounds_custom->depth = 2;
  sounds_custom->index = 0;
  array_scroll(sounds_custom, 0);

  // sounds_midi
  sounds_midi->name = strdup("sounds_midi");
  sounds_midi->ptr_str_array = (cfg->sounds_midi)->array;
  sounds_midi->parent = sounds;
  sounds_midi->child = NULL;
  sounds_midi->size = (cfg->sounds_midi)->size;
  sounds_midi->lcd_state = state_sounds_midi;
  sounds_midi->depth = 2;
  sounds_midi->index = 0;
  array_scroll(sounds_midi, 0);

  return main;
}
