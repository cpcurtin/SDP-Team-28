/*
 *	LCD function library
 *
 *
 *
 */
#include "lcd-module.h"
// #include <LiquidCrystal.h>
int lcd_rows;

LiquidCrystal *lcd_init(const struct lcd_pin_config &cfg)
{
  // welcome message
  char **lcd_init_message = new char *[2];
  lcd_init_message[0] = strdup("   Welcome to   ");
  lcd_init_message[1] = strdup("    Moduloop    ");

  // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
  LiquidCrystal *lcd = new LiquidCrystal(cfg.rs, cfg.en, cfg.dig4, cfg.dig5, cfg.dig6, cfg.dig7);
  lcd_rows = cfg.rows;
  lcd->begin(cfg.columns, cfg.rows);
  lcd->clear();
  lcd_display(lcd, lcd_init_message);

  return lcd;
}

void lcd_display(LiquidCrystal *lcd, char **print_arr)
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
    new_index = (int)nav->size - 1;
  }
  else
  {
    new_index = (nav->index + direction) % nav->size;
  }

  nav->index = new_index;
  char *temp_row1_str = (char *)malloc(strlen(nav->ptr_str_array[new_index]) + 3);
  char *temp_row1_str2 = (char *)malloc(strlen(nav->ptr_str_array[new_index]));

  strcpy(temp_row1_str, ">");

  strcpy(temp_row1_str2, nav->ptr_str_array[new_index]);

  strcat(temp_row1_str, temp_row1_str2);

  nav->lcd_state[0] = strdup(temp_row1_str);

  free(temp_row1_str);
  free(temp_row1_str2);

  for (int row = 1; row < lcd_rows; row++)
  {
    int temp_index = (new_index + row) % nav->size;
    // nav->lcd_state[row] = (nav->ptr_str_array[temp_index]);

    char *temp_str = (char *)malloc(strlen(nav->ptr_str_array[temp_index]) + 3);
    char *temp_str2 = (char *)malloc(strlen(nav->ptr_str_array[temp_index]));

    strcpy(temp_str, " ");

    strcpy(temp_str2, nav->ptr_str_array[temp_index]);

    strcat(temp_str, temp_str2);

    nav->lcd_state[row] = strdup(temp_str);

    free(temp_str);
    free(temp_str2);
  }
  
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

  char **nav_main = new char *[2];
  nav_main[0] = strdup("Sounds");
  nav_main[1] = strdup("Effects");

  char **nav_sounds = new char *[2];
  nav_sounds[0] = strdup("Custom Sounds");
  nav_sounds[1] = strdup("MIDI Sounds");

  // struct instatiation
  struct lcd_nav *main = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *sounds = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *effects = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *sounds_custom = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *sounds_midi = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  char **state_main = (char **)malloc(2 * sizeof(char *));
  char **state_sounds = (char **)malloc(2 * sizeof(char *));
  char **state_effects = (char **)malloc(2 * sizeof(char *));
  char **state_sounds_custom = (char **)malloc(2 * sizeof(char *));
  char **state_sounds_midi = (char **)malloc(2 * sizeof(char *));
  // char **state = new char *[2];

  // ptr arrays
  struct lcd_nav **main_child = new struct lcd_nav *[2];
  main_child[0] = sounds;
  main_child[1] = effects;

  struct lcd_nav **sounds_child = new struct lcd_nav *[2];
  sounds_child[0] = sounds_custom;
  sounds_child[1] = sounds_midi;

  //   char *name;
  //   char **ptr_str_array;
  //   struct lcd_nav **parent;
  //   struct lcd_nav **child;
  //   size_t size;
  //   char **lcd_state;
  //   int index;
  //   int depth;

  // main
  main->name = strdup("main");
  main->ptr_str_array = nav_main;
  main->parent = NULL;
  main->child = main_child;
  main->size = 2;
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
