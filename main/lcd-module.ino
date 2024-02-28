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
  // char **lcd_init_message = new char *[4];
  // lcd_init_message[0] = strdup("     Welcome to     ");
  // lcd_init_message[1] = strdup("      Moduloop      ");
  // lcd_init_message[2] = strdup("");
  // lcd_init_message[3] = strdup("    SDP team 28    ");
  const char *lcd_init_message[] = {
      "     Welcome to     ",
      "      Moduloop      ",
      "",
      "    SDP team 28    "};

  // LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C(cfg.i2c, cfg.columns, cfg.rows);
  LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C(cfg->i2c, cfg->columns, cfg->rows);

  lcd->init(); // initialize the lcd
  lcd->backlight();
  lcd_rows = cfg->rows;
  lcd->begin(cfg->columns, cfg->rows);
  lcd->clear();
  for (int row = 0; row < lcd_rows; row++)
  {
    lcd->setCursor(0, row); // set cursor to row 0

    lcd->print(lcd_init_message[row]); // print to row 0
  }
  lcd->home();

  return lcd;
}

void lcd_display(LiquidCrystal_I2C *lcd, const char **print_arr)
{
  lcd->clear();
  // if (active_track.bpm == 0)
  // {
  for (int row = 0; row < lcd_rows - 1; row++)
  {
    lcd->setCursor(0, row); // set cursor to row 0

    lcd->print(print_arr[row]); // print to row 0
  }
  // lcd->setCursor(0, lcd_rows - 1); // set cursor to row 0
  // lcd->print(tracks_update());     // print to row 0
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
  for (int row = 1; row < lcd_rows - 1; row++)
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
  // nav->lcd_state[lcd_rows-1] = format_row(nav->ptr_str_array, 0, 2); //system info
  // Serial.println("##############END SCROLL##############");
}

const char *format_row(const char **ptr_str_array, int index, int format)
{
  char *temp_str = (char *)malloc(20 + 1); // Allocate memory dynamically

  if (temp_str == NULL)
  {
    // Allocation failed
    return NULL;
  }

  // spacing, enumerated
  if (format == 0)
  {
    snprintf(temp_str, 20 + 1, " %d %s", index + 1, ptr_str_array[index]);
  }
  else if (format == 1)
  {
    snprintf(temp_str, 20 + 1, ">%d %s", index + 1, ptr_str_array[index]);
  }

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

  // Initialize the navigation strings
  // const char *nav_main[] = {"Sounds", "Effects", "Tracks"};
  // const char *nav_sounds[] = {"Custom Sounds", "MIDI Sounds"};

  const char **nav_main = new const char *[3];
  nav_main[0] = strdup("Sounds");
  nav_main[1] = strdup("Effects");
  nav_main[2] = strdup("Tracks");

  const char **nav_sounds = new const char *[2];
  nav_sounds[0] = strdup("Custom Sounds");
  nav_sounds[1] = strdup("MIDI Sounds");

  // struct instatiation
  struct lcd_nav *main = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *sounds = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *effects = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *tracks = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *sounds_custom = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  struct lcd_nav *sounds_midi = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  const char **state_main = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_sounds = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_effects = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_tracks = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_sounds_custom = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_sounds_midi = (const char **)malloc(lcd_rows * sizeof(char *));
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
void nav_add(struct lcd_nav *node)
{
}
const char *tracks_update(void)
{
  int max_length = 20;
  char *temp_str = (char *)malloc(max_length + 1); // Allocate memory dynamically

  if (temp_str == NULL)
  {
    // Allocation failed
    return NULL;
  }

  // spacing, enumerated
  if (active_track.bpm != 0)
  {
    snprintf(temp_str, 20 + 1, "BPM:%d %s", active_track.bpm, active_track.name);
  }
  else
  {
    snprintf(temp_str, 20 + 1, "NO TRACK SELECTED");
  }
  return temp_str;
}
void update_tempo(LiquidCrystal_I2C *lcd)
{
  lcd->setCursor(0, lcd_rows - 1); // set cursor to row 0
  lcd->print(tracks_update());     // print to row 0
  lcd->home();
}