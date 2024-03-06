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
      "",
      "    SDP team 28    "};

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

  for (int row = 0; row < lcd_rows - 1; row++)
  {
    lcd->setCursor(0, row); // set cursor to row 0

    lcd->print(print_arr[row]); // print to row 0
  }

  lcd->home();
}

void array_scroll(lcd_nav *nav, int direction)
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

      nav->lcd_state[row] = strdup("");
    }
  }
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

lcd_nav *nav_selection(lcd_nav *nav, int direction)
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

lcd_nav *nav_init(struct nav_config *cfg)
{

  const char **nav_main = new const char *[3];
  nav_main[0] = strdup("Sounds");
  nav_main[1] = strdup("Effects");
  nav_main[2] = strdup("Tracks");

  const char **nav_sounds = new const char *[2];
  nav_sounds[0] = strdup("Custom Sounds");
  nav_sounds[1] = strdup("MIDI Sounds");

  const char **nav_tracks = new const char *[3];

  nav_tracks[0] = strdup("Save Track");
  nav_tracks[1] = strdup("Load Track");
  nav_tracks[2] = strdup("Set # steps");
  const char **nav_tracks_steps = new const char *[6];
  for (int i = 0; i < MAX_MEASURE_STEPS; i++)
  {
    nav_tracks_steps[i] = strdup("Step");
  }

  /* NAV STRUCT INITIALIZATION */
  lcd_nav *main = (lcd_nav *)malloc(sizeof(lcd_nav));
  lcd_nav *sounds = (lcd_nav *)malloc(sizeof(lcd_nav));
  lcd_nav *effects = (lcd_nav *)malloc(sizeof(lcd_nav));
  lcd_nav *tracks = (lcd_nav *)malloc(sizeof(lcd_nav));
  lcd_nav *sounds_custom = (lcd_nav *)malloc(sizeof(lcd_nav));
  lcd_nav *sounds_midi = (lcd_nav *)malloc(sizeof(lcd_nav));
  lcd_nav *tracks_load = (lcd_nav *)malloc(sizeof(lcd_nav));
  lcd_nav *tracks_set_steps = (lcd_nav *)malloc(sizeof(lcd_nav));

  /* LCD STATES INITIALIZATION */
  const char **state_main = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_sounds = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_effects = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_tracks = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_sounds_custom = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_sounds_midi = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_tracks_load = (const char **)malloc(lcd_rows * sizeof(char *));
  const char **state_tracks_set_steps = (const char **)malloc(lcd_rows * sizeof(char *));
  // char **state = new char *[2];

  // ptr arrays
  lcd_nav **main_child = new lcd_nav *[3];
  main_child[0] = sounds;
  main_child[1] = effects;
  main_child[2] = tracks;

  lcd_nav **sounds_child = new lcd_nav *[2];
  sounds_child[0] = sounds_custom;
  sounds_child[1] = sounds_midi;

  lcd_nav **tracks_child = new lcd_nav *[3];
  // tracks_child[0]  save track
  tracks_child[1] = tracks_load;      // load track
  tracks_child[2] = tracks_set_steps; // set steps
  // main
  main->name = strdup("main");
  main->ptr_str_array = nav_main;
  main->parent = NULL;
  main->child = main_child;
  main->size = 3; // sizeof(nav_main) / sizeof(nav_main[0]);
  main->lcd_state = state_main;
  main->index = 0;
  main->depth = 0;
  array_scroll(main, 0);

  // sounds
  sounds->name = strdup("sounds");
  sounds->ptr_str_array = nav_sounds;
  sounds->parent = main;
  sounds->child = sounds_child;
  sounds->size = 2; // sizeof(nav_sounds) / sizeof(nav_sounds[0]);
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
  tracks->ptr_str_array = nav_tracks;
  tracks->parent = main;
  tracks->child = tracks_child;
  tracks->size = 3; // sizeof(nav_tracks) / sizeof(nav_tracks[0]);
  tracks->lcd_state = state_tracks;
  tracks->depth = 1;
  tracks->index = 0;
  array_scroll(tracks, 0);

  // tracks LOAD
  tracks_load->name = strdup("tracks_load");
  tracks_load->ptr_str_array = (cfg->tracks_load)->array;
  tracks_load->parent = tracks;
  tracks_load->child = NULL;
  tracks_load->size = (cfg->tracks_load)->size;
  tracks_load->lcd_state = state_tracks_load;
  tracks_load->depth = 2;
  tracks_load->index = 0;
  array_scroll(tracks_load, 0);

  // tracks SET STEPS
  tracks_set_steps->name = strdup("tracks_set_steps");
  tracks_set_steps->ptr_str_array = nav_tracks_steps;
  tracks_set_steps->parent = tracks;
  tracks_set_steps->child = NULL;
  tracks_set_steps->size = 6; // sizeof(nav_tracks_steps) / sizeof(nav_tracks_steps[0]);
  tracks_set_steps->lcd_state = state_tracks_set_steps;
  tracks_set_steps->depth = 2;
  tracks_set_steps->index = 0;
  array_scroll(tracks_set_steps, 0);

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
void nav_add(lcd_nav *node)
{
}
const char *tracks_update(void)
{
  int max_length = 20;
  char *temp_str = (char *)malloc(max_length + 1);

  if (temp_str == NULL)
  {
    // Allocation failed
    return NULL;
  }

  // spacing, enumerated
  if (active_track.bpm != 0)
  {
    snprintf(temp_str, 20 + 1, "BPM:%d STEPS:%d ID:%d ", active_track.bpm, active_track.measure_steps, active_track.id);
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