#include "main.h"

LiquidCrystal_I2C *lcd;

char **lcd_state = new char *[LCD_ROWS];
int lcd_index = 0;
struct lcd_nav *sounds;
struct lcd_nav *nav_data_structure;
struct lcd_nav *nav_state;
struct palette_matrix *palette;
struct button_maxtrix_pin_config measure_matrix_button;
struct button_maxtrix_pin_config measure_matrix_led;
struct nav_config *nav_cfg;

char *selection;

// const struct lcd_pin_config lcd_cfg = {LCD_RS, LCD_EN, LCD_DIGITAL_4, LCD_DIGITAL_5, LCD_DIGITAL_6, LCD_DIGITAL_7, LCD_ROWS, LCD_COLUMNS};
const struct lcd_pin_config lcd_cfg = {LCD_I2C, LCD_ROWS, LCD_COLUMNS};
const struct dac_pin_config dac_cfg = {DAC_DIN, DAC_WS, DAC_BCK};
const struct dpad_pin_config dpad_cfg = {BUTTON_DPAD_LEFT, BUTTON_DPAD_DOWN, BUTTON_DPAD_UP, BUTTON_DPAD_RIGHT};
// create 2D array of palette_cell structs

// Metronome Definition
Metro ledMetro = Metro(250);
int count_temp = 0;

void setup()
{
  //
  //
  // button & led measure matrix
  //
  //
  // measure_matrix_button = (struct button_maxtrix_pin_config *)malloc(sizeof(struct button_maxtrix_pin_config));
  int button_matrix_rows[MEASURE_MATRIX_ROWS + 1] = {BUTTON_MEASURE_MATRIX_ROW_1, BUTTON_MEASURE_MATRIX_ROW_2, BUTTON_MEASURE_MATRIX_ROW_3, BUTTON_MEASURE_MATRIX_ROW_4};
  int button_matrix_columms[MEASURE_MATRIX_COLUMNS + 1] = {BUTTON_MEASURE_MATRIX_COLUMN_1, BUTTON_MEASURE_MATRIX_COLUMN_2, BUTTON_MEASURE_MATRIX_COLUMN_3, BUTTON_MEASURE_MATRIX_COLUMN_4, BUTTON_MEASURE_MATRIX_COLUMN_5, BUTTON_MEASURE_MATRIX_COLUMN_6};
  measure_matrix_button.width = MEASURE_MATRIX_COLUMNS;
  measure_matrix_button.length = MEASURE_MATRIX_ROWS;

  for (size_t i = 0; i < MEASURE_MATRIX_ROWS; ++i)
  {
    measure_matrix_button.rows[i] = button_matrix_rows[i];
  }
  for (size_t i = 0; i < MEASURE_MATRIX_COLUMNS; ++i)
  {
    measure_matrix_button.columns[i] = button_matrix_columms[i];
  }

  // measure_matrix_led = (struct button_maxtrix_pin_config *)malloc(sizeof(struct button_maxtrix_pin_config));
  int led_matrix_rows[MEASURE_MATRIX_ROWS + 1] = {LED_MEASURE_MATRIX_ROW_1, LED_MEASURE_MATRIX_ROW_2, LED_MEASURE_MATRIX_ROW_3, LED_MEASURE_MATRIX_ROW_4};
  int led_matrix_columms[MEASURE_MATRIX_COLUMNS + 1] = {LED_MEASURE_MATRIX_COLUMN_1, LED_MEASURE_MATRIX_COLUMN_2, LED_MEASURE_MATRIX_COLUMN_3, LED_MEASURE_MATRIX_COLUMN_4, LED_MEASURE_MATRIX_COLUMN_5, LED_MEASURE_MATRIX_COLUMN_6};
  measure_matrix_led.width = MEASURE_MATRIX_COLUMNS;
  measure_matrix_led.length = MEASURE_MATRIX_ROWS;
  for (size_t i = 0; i < MEASURE_MATRIX_ROWS; ++i)
  {
    measure_matrix_led.rows[i] = led_matrix_rows[i];
  }
  for (size_t i = 0; i < MEASURE_MATRIX_COLUMNS; ++i)
  {
    measure_matrix_led.columns[i] = led_matrix_columms[i];
  }

  /* Intialize hardware */
  serial_init();
  sd_init();
  dpad_init(dpad_cfg);
  test_init();
  onboard_dac_init();
  measure_matrix_init(measure_matrix_button, measure_matrix_led);

  nav_cfg = (struct nav_config *)malloc(sizeof(struct nav_config));
  nav_cfg->effects = (struct array_with_size *)malloc(sizeof(struct array_with_size));
  nav_cfg->tracks = (struct array_with_size *)malloc(sizeof(struct array_with_size));
  nav_cfg->sounds_custom = (struct array_with_size *)malloc(sizeof(struct array_with_size));
  nav_cfg->sounds_midi = (struct array_with_size *)malloc(sizeof(struct array_with_size));

  const char **nav_effects = new const char *[2];
  nav_effects[0] = strdup("effect1");
  nav_effects[1] = strdup("effect2");
  ((nav_cfg->effects)->array) = nav_effects;
  ((nav_cfg->effects)->size) = 2;

  const char **nav_sounds_midi = new const char *[2];
  nav_sounds_midi[0] = strdup("midi1");
  nav_sounds_midi[1] = strdup("midi2");
  (nav_cfg->sounds_midi)->array = nav_sounds_midi;
  (nav_cfg->sounds_midi)->size = 2;

  (nav_cfg->sounds_custom = parsefiles());
  Serial.println("parsed files size");
  Serial.println((nav_cfg->sounds_custom)->size);
  const char **nav_tracks = new const char *[2];
  nav_tracks[0] = strdup("track1");
  nav_tracks[1] = strdup("track2");
  (nav_cfg->tracks)->array = nav_tracks;
  (nav_cfg->tracks)->size = 2;

  lcd = lcd_init(&lcd_cfg);
  nav_data_structure = nav_init(nav_cfg);
  nav_state = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  nav_state = nav_data_structure;

  Serial.println("PROGRAM LOOP BEGINS");
  for (size_t i = 0; i < (nav_cfg->sounds_custom)->size; i++)
  {
    Serial.println((nav_cfg->sounds_custom)->array[i]);
  }
  delay(3000);
  lcd_display(lcd, nav_state->lcd_state);

  // Midi Init
  MIDI.begin(31250);
  pinMode(VS1053_RST, OUTPUT);
  digitalWrite(VS1053_RST, LOW);
  delay(10);
  digitalWrite(VS1053_RST, HIGH);
  delay(10);
  midiSetChannelVolume(0, 127);
  midiSetChannelBank(0, Drums1);

  midiSetInstrument(0, 128);
  /*****************************************
  PUT THIS LINE AT THE TOP OF ALL PERCUSSIVE SOUND BLOCKS SO IT WILL
  START AS A PERCUSION SOUND:
  midiSetInstrument(0,128);
  *****************************************/
  // struct track tracktst;
  read_track(fileNamejson, active_track);
  // active_track=&tracktst;
  for (int i = 0;i < 4; i++)
  {
    cached_samples[i] = cache_sd_sound((nav_cfg->sounds_custom)->array[2]);
  }
}
int mixer_1;
int mixer_2;
int mixer_3;
int mixer_4;
float metro_active_tempo;
/* Main subroutine: follow software block diagram */
void loop()
{
  // Main Timing Loop for 4x4 Measure Matrix
  if (ledMetro.check() == 1)
  {
    unsigned long start_time = millis();

    if (count_temp == 0)
    {
      Serial.println("Hit");
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
      digitalWrite(LED_MEASURE_MATRIX_ROW_4, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
      midiNoteOn(0, Crash1, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
      mixer_1 = playFile(cached_samples[0]);
    }

    if (count_temp == 1)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
      mixer_2 = playFile(cached_samples[0]);
      stopFile(mixer_1);
    }

    if (count_temp == 2)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticSnare, 127);
      mixer_3 = playFile(cached_samples[0]);
      stopFile(mixer_2);
    }

    if (count_temp == 3)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
      mixer_4 = playFile(cached_samples[0]);
      stopFile(mixer_3);
    }

    if (count_temp == 4)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
      digitalWrite(LED_MEASURE_MATRIX_ROW_1, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
      stopFile(mixer_4);
    }

    if (count_temp == 5)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
    }

    if (count_temp == 6)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticSnare, 127);
    }

    if (count_temp == 7)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
    }

    if (count_temp == 8)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
      digitalWrite(LED_MEASURE_MATRIX_ROW_2, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
    }

    if (count_temp == 9)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
    }

    if (count_temp == 10)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticSnare, 127);
    }

    if (count_temp == 11)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
    }

    if (count_temp == 12)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
      digitalWrite(LED_MEASURE_MATRIX_ROW_3, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
    }

    if (count_temp == 13)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
    }

    if (count_temp == 14)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticSnare, 127);
    }

    if (count_temp == 15)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      metro_active_tempo = (15000 / active_track.bpm);
      ledMetro.interval(metro_active_tempo);
    }
    unsigned long end_time = millis();
    // Calculate the difference in time
    unsigned long time_diff = end_time - start_time;
    // Output the time difference
    Serial.print("Time elapsed: ");
    Serial.print(time_diff);
    Serial.println(" milliseconds");

    count_temp++;
    active_track.bpm = read_tempo();
    update_tempo(lcd);
    if (count_temp == 16)
    {
      count_temp = 0;
    }
  }

  if (button_pressed(BUTTON_DPAD_LEFT)) // return / exit
  {
    nav_state = nav_selection(nav_state, NAV_BACKWARD);

    lcd_display(lcd, nav_state->lcd_state);
  }
  if (button_pressed(BUTTON_DPAD_DOWN)) // scroll down
  {
    array_scroll(nav_state, NAV_DOWN);
    lcd_display(lcd, nav_state->lcd_state);
  }
  if (button_pressed(BUTTON_DPAD_UP)) // scroll up
  {
    array_scroll(nav_state, NAV_UP);
    lcd_display(lcd, nav_state->lcd_state);
  }
  if (button_pressed(BUTTON_DPAD_RIGHT)) // select
  {
    if (strcmp(nav_state->name, "tracks") == 0)
    {
      print_JSON(fileNamejson);
      Serial.println(active_track.name);
    }
    else
    {
      nav_state = nav_selection(nav_state, NAV_FORWARD);
      // Serial.printf("forward, %s\n",selection);
      lcd_display(lcd, nav_state->lcd_state);
    }
  }

  // readMatrix(measure_matrix_button, measure_matrix_led);
}

void serial_init(void)
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect.
  }
  Serial.println("\n\n\n\n\nSerial Initialized<<<<<<<<<<<<<<");
}
