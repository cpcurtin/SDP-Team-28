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

void lcd_display(LiquidCrystal_I2C *lcd, const char **print_arr)
{
  lcd->clear();

  for (int row = 0; row < LCD_ROWS - 1; row++)
  {
    lcd->setCursor(0, row); // set cursor to row 0

    lcd->print(print_arr[row]); // print to row 0
  }

  lcd->home();
}

void array_scroll(lcd_nav *nav, int direction)
{

  int new_index;
  // new index changes +/- and goes from end to 0
  new_index = (nav->index + direction + nav->size) % nav->size;

  nav->index = new_index;
  nav->lcd_state[0] = format_row(nav->data_array, new_index, 1);

  // THIS IS WHERE IT REPEATS
  for (int row = 1; row < LCD_ROWS - 1; row++)
  {
    if (row < (int)nav->size)
    {
      int temp_index = (new_index + row) % nav->size;
      nav->lcd_state[row] = format_row(nav->data_array, temp_index, 0);
    }
    else
    {

      nav->lcd_state[row] = strdup("");
    }
  }
}

const char *format_row(const char **data_array, int index, int format)
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
    snprintf(temp_str, 20 + 1, " %d %s", index + 1, data_array[index]);
  }
  else if (format == 1)
  {
    snprintf(temp_str, 20 + 1, ">%d %s", index + 1, data_array[index]);
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
    else if (strcmp(nav->name, "sounds_midi_melodic") == 0)
    {
      return sounds_midi_octaves_nav;
    }
    else if (strcmp(nav->name, "sounds_midi_octaves") == 0)
    {
      return sounds_midi_notes_nav;
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

  const char **main_preset_options = new const char *[3];
  main_preset_options[0] = strdup("Sounds");
  main_preset_options[1] = strdup("Effects");
  main_preset_options[2] = strdup("Tracks");

  const char **sounds_preset_options = new const char *[2];
  sounds_preset_options[0] = strdup("Custom Sounds");
  sounds_preset_options[1] = strdup("MIDI Sounds");

  const char **tracks_preset_options = new const char *[4];
  tracks_preset_options[0] = strdup("Set # steps");
  tracks_preset_options[1] = strdup("Save Track");
  tracks_preset_options[2] = strdup("Load Track");
  tracks_preset_options[3] = strdup("Delete Track");

  const char **tracks_preset_options_steps = new const char *[6];
  for (int i = 0; i < MAX_MEASURE_STEPS; i++)
  {
    tracks_preset_options_steps[i] = strdup("Step");
  }

  const char **midi_preset_options = new const char *[2];
  midi_preset_options[0] = strdup("Melodic Instruments");
  midi_preset_options[1] = strdup("Percussion Instruments");

  // LCD STATES INITIALIZATION
  const char **state_main = new const char *[LCD_ROWS];
  const char **state_sounds = new const char *[LCD_ROWS];
  const char **state_effects = new const char *[LCD_ROWS];
  const char **state_tracks = new const char *[LCD_ROWS];
  const char **state_sounds_custom = new const char *[LCD_ROWS];
  const char **state_sounds_midi = new const char *[LCD_ROWS];
  const char **state_tracks_load = new const char *[LCD_ROWS];
  const char **state_tracks_set_steps = new const char *[LCD_ROWS];
  const char **state_midi_melodic = new const char *[LCD_ROWS];
  const char **state_midi_percussion = new const char *[LCD_ROWS];
  const char **state_midi_octaves = new const char *[LCD_ROWS];
  const char **state_midi_notes = new const char *[LCD_ROWS];

  // ptr arrays
  lcd_nav **main_child = new lcd_nav *[3];
  main_child[0] = sounds_nav;
  main_child[1] = effects_nav;
  main_child[2] = tracks_nav;

  lcd_nav **sounds_child = new lcd_nav *[2];
  sounds_child[0] = sounds_custom_nav;
  sounds_child[1] = sounds_midi_nav;

  lcd_nav **midi_child = new lcd_nav *[2];
  midi_child[0] = sounds_midi_melodic_nav;
  midi_child[1] = sounds_midi_percussion_nav;

  lcd_nav **tracks_child = new lcd_nav *[4];
  tracks_child[0] = tracks_set_steps_nav; // set steps
  // tracks_child[1] = SAVE TRACK
  tracks_child[2] = tracks_load_nav; // load track
  // tracks_child[3] = DELETE TRACK

  // main
  main_nav->name = strdup("main");
  main_nav->data_array = main_preset_options;
  main_nav->parent = NULL;
  main_nav->child = main_child;
  main_nav->size = 3; // sizeof(main_preset_options) / sizeof(main_preset_options[0]);
  main_nav->lcd_state = state_main;
  main_nav->index = 0;
  array_scroll(main_nav, 0);

  // sounds
  sounds_nav->name = strdup("sounds");
  sounds_nav->data_array = sounds_preset_options;
  sounds_nav->parent = main_nav;
  sounds_nav->child = sounds_child;
  sounds_nav->size = 2; // sizeof(sounds_preset_options) / sizeof(sounds_preset_options[0]);
  sounds_nav->lcd_state = state_sounds;
  sounds_nav->index = 0;
  array_scroll(sounds_nav, 0);

  // effects
  effects_nav->name = strdup("effects");
  effects_nav->data_array = (cfg->effects)->array;
  effects_nav->parent = main_nav;
  effects_nav->child = NULL;
  effects_nav->size = (cfg->effects)->size;
  effects_nav->lcd_state = state_effects;
  effects_nav->index = 0;
  array_scroll(effects_nav, 0);

  // tracks
  tracks_nav->name = strdup("tracks");
  tracks_nav->data_array = tracks_preset_options;
  tracks_nav->parent = main_nav;
  tracks_nav->child = tracks_child;
  tracks_nav->size = 4; // sizeof(tracks_preset_options) / sizeof(tracks_preset_options[0]);
  tracks_nav->lcd_state = state_tracks;
  tracks_nav->index = 0;
  array_scroll(tracks_nav, 0);

  // tracks LOAD
  tracks_load_nav->name = strdup("tracks_load");
  tracks_load_nav->data_array = (cfg->tracks_load)->array;
  tracks_load_nav->parent = tracks_nav;
  tracks_load_nav->child = NULL;
  tracks_load_nav->size = (cfg->tracks_load)->size;
  tracks_load_nav->lcd_state = state_tracks_load;
  tracks_load_nav->index = 0;
  array_scroll(tracks_load_nav, 0);

  // tracks SET STEPS
  tracks_set_steps_nav->name = strdup("tracks_set_steps");
  tracks_set_steps_nav->data_array = tracks_preset_options_steps;
  tracks_set_steps_nav->parent = tracks_nav;
  tracks_set_steps_nav->child = NULL;
  tracks_set_steps_nav->size = 6; // sizeof(tracks_preset_options_steps) / sizeof(tracks_preset_options_steps[0]);
  tracks_set_steps_nav->lcd_state = state_tracks_set_steps;
  tracks_set_steps_nav->index = 0;
  array_scroll(tracks_set_steps_nav, 0);

  // custom_sounds
  sounds_custom_nav->name = strdup("custom_sounds");
  sounds_custom_nav->data_array = (cfg->sounds_custom)->array;
  sounds_custom_nav->parent = sounds_nav;
  sounds_custom_nav->child = NULL;
  sounds_custom_nav->size = (cfg->sounds_custom)->size;
  sounds_custom_nav->lcd_state = state_sounds_custom;
  sounds_custom_nav->index = 0;
  array_scroll(sounds_custom_nav, 0);

  // sounds_midi
  sounds_midi_nav->name = strdup("sounds_midi");
  sounds_midi_nav->data_array = midi_preset_options;
  sounds_midi_nav->parent = sounds_nav;
  sounds_midi_nav->child = midi_child;
  sounds_midi_nav->size = 2;
  sounds_midi_nav->lcd_state = state_sounds_midi;
  sounds_midi_nav->index = 0;
  array_scroll(sounds_midi_nav, 0);

  // sounds_midi_melodic
  sounds_midi_melodic_nav->name = strdup("sounds_midi_melodic");
  sounds_midi_melodic_nav->data_array = (cfg->sounds_midi_melodic)->array;
  sounds_midi_melodic_nav->parent = sounds_midi_nav;
  sounds_midi_melodic_nav->child = NULL;
  sounds_midi_melodic_nav->size = (cfg->sounds_midi_melodic)->size;
  sounds_midi_melodic_nav->lcd_state = state_midi_melodic;
  sounds_midi_melodic_nav->index = 0;
  array_scroll(sounds_midi_melodic_nav, 0);

  // sounds_midi_percussion
  sounds_midi_percussion_nav->name = strdup("sounds_midi_percussion");
  sounds_midi_percussion_nav->data_array = (cfg->sounds_midi_percussion)->array;
  sounds_midi_percussion_nav->parent = sounds_midi_nav;
  sounds_midi_percussion_nav->child = NULL;
  sounds_midi_percussion_nav->size = (cfg->sounds_midi_percussion)->size;
  sounds_midi_percussion_nav->lcd_state = state_midi_percussion;
  sounds_midi_percussion_nav->index = 0;
  array_scroll(sounds_midi_percussion_nav, 0);

  // sounds_midi_octaves
  sounds_midi_octaves_nav->name = strdup("sounds_midi_octaves");
  sounds_midi_octaves_nav->data_array = octaves;
  sounds_midi_octaves_nav->parent = sounds_midi_percussion_nav;
  sounds_midi_octaves_nav->child = NULL;
  sounds_midi_octaves_nav->size = 11;
  sounds_midi_octaves_nav->lcd_state = state_midi_octaves;
  sounds_midi_octaves_nav->index = 2;
  array_scroll(sounds_midi_octaves_nav, 0);

  // sounds_midi_notes
  sounds_midi_notes_nav->name = strdup("sounds_midi_notes");
  sounds_midi_notes_nav->data_array = note_names;
  sounds_midi_notes_nav->parent = sounds_midi_octaves_nav;
  sounds_midi_notes_nav->child = NULL;
  sounds_midi_notes_nav->size = 12;
  sounds_midi_notes_nav->lcd_state = state_midi_notes;
  sounds_midi_notes_nav->index = 0;
  array_scroll(sounds_midi_notes_nav, 0);

  return main_nav;
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
  lcd->setCursor(0, LCD_ROWS - 1); // set cursor to row 0
  lcd->print(tracks_update());     // print to row 0
  lcd->home();
}