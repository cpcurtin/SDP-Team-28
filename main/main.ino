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
int count = 0;

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
  // button_matrix_init(matrix_cfg);
  // button_matrix_init(matrix_cfg);
  measure_matrix_init(measure_matrix_button, measure_matrix_led);

  nav_cfg = (struct nav_config *)malloc(sizeof(struct nav_config));
  nav_cfg->effects = (struct array_with_size *)malloc(sizeof(struct array_with_size));
  nav_cfg->tracks = (struct array_with_size *)malloc(sizeof(struct array_with_size));
  nav_cfg->sounds_custom = (struct array_with_size *)malloc(sizeof(struct array_with_size));
  nav_cfg->sounds_midi = (struct array_with_size *)malloc(sizeof(struct array_with_size));

  Serial.println("made it here1");

  const char **nav_effects = new const char *[2];
  nav_effects[0] = strdup("effect1");
  nav_effects[1] = strdup("effect2");
  ((nav_cfg->effects)->array) = nav_effects;
  ((nav_cfg->effects)->size) = 2;

  Serial.println("made it here2");

  const char **nav_sounds_midi = new const char *[2];
  nav_sounds_midi[0] = strdup("midi1");
  nav_sounds_midi[1] = strdup("midi2");
  (nav_cfg->sounds_midi)->array = nav_sounds_midi;
  (nav_cfg->sounds_midi)->size = 2;

  Serial.println("made it here3");

  (nav_cfg->sounds_custom = parsefiles());
  Serial.println("parsed files size");
  Serial.println((nav_cfg->sounds_custom)->size);
  // playFile((nav_cfg->sounds_custom)->array[1]);

  Serial.println("made it here4");

  const char **nav_tracks = new const char *[2];
  nav_tracks[0] = strdup("track1");
  nav_tracks[1] = strdup("track2");
  (nav_cfg->tracks)->array = nav_tracks;
  (nav_cfg->tracks)->size = 2;

  lcd = lcd_init(&lcd_cfg);
  nav_data_structure = nav_init(nav_cfg);
  nav_state = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  // Serial.println(((nav_state->child[0])->child[0])->size);
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
}

/* Main subroutine: follow software block diagram */
void loop()
{
  // Main Timing Loop for 4x4 Measure Matrix
  if (ledMetro.check() == 1)
  {
    if (count == 0)
    {
      Serial.println("Hit");
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
      digitalWrite(LED_MEASURE_MATRIX_ROW_4, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
      midiNoteOn(0, Crash1, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
      // playFile((nav_cfg->sounds_custom)->array[6]);
    }

    if (count == 1)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
    }

    if (count == 2)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticSnare, 127);
    }

    if (count == 3)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
    }

    if (count == 4)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
      digitalWrite(LED_MEASURE_MATRIX_ROW_1, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
    }

    if (count == 5)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
    }

    if (count == 6)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticSnare, 127);
    }

    if (count == 7)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
    }

    if (count == 8)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
      digitalWrite(LED_MEASURE_MATRIX_ROW_2, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
    }

    if (count == 9)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
    }

    if (count == 10)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticSnare, 127);
    }

    if (count == 11)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
    }

    if (count == 12)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
      digitalWrite(LED_MEASURE_MATRIX_ROW_3, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticBassDrum, 127);
    }

    if (count == 13)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);
      midiNoteOn(0, ClosedHiHat, 127);
    }

    if (count == 14)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
      midiNoteOn(0, AcousticSnare, 127);
    }

    if (count == 15)
    {
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
      digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
      digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);

      midiNoteOn(0, ClosedHiHat, 127);
    }

    count++;
    if (count == 16)
    {
      count = 0;
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
    if (strcmp(nav_state->name, "custom_sounds") == 0)
    {
      Serial.println("made it here");
      // palette_assign(palette, nav_state->ptr_str_array[nav_state->index]);
      // for (int m = 0; m < PALETTE_MATRIX_ROWS; m++)
      // {

      //   for (int n = 0; n < PALETTE_MATRIX_COLUMNS; n++)
      //   {
      //     Serial.println((palette->cells[m][n])->sound);
      //   }
      // }
    }
    else
    {
      nav_state = nav_selection(nav_state, NAV_FORWARD);
      // Serial.printf("forward, %s\n",selection);
      lcd_display(lcd, nav_state->lcd_state);
    }
  }
  // for (int i = 0; i < 3; i++)
  // {
  //   if (!(palette->cells[0][i])->available)
  //   {
  //     if (button_pressed(28 + i))
  //     {

  //       playFile((palette->cells[0][i])->sound);
  //     }
  //   }
  // }

  // readMatrix(measure_matrix_button, measure_matrix_led);

  /* RIGHT SECTION OF TEST BUTTONS (3 LEFTMOST BUTTONS) */
  // if (button_pressed(28)) // left button
  // {
  //   Serial.println("Button 28");
  //   // example playing a sd file at current leaf string array index
  //   if (strcmp(nav_state->name,"custom_sounds")){
  //     // makes sure custom sound leaf node
  //     Serial.println("Custom Sounds");
  //     playFile(nav_state->ptr_str_array[nav_state->index]);
  //   }

  // //   // Serial.printf("forward, %s\n",selection);
  // //   lcd_display(lcd, nav_state->lcd_state);
  // }
  // if (button_pressed(29)) // middle button
  // {

  //   // Serial.printf("forward, %s\n",selection);
  //   lcd_display(lcd, nav_state->lcd_state);
  // }
  // if (button_pressed(30)) // right button
  // {

  //   // Serial.printf("forward, %s\n",selection);
  //   lcd_display(lcd, nav_state->lcd_state);
  // }

  // delay(1000);
  // Serial.printf("CURRENT ARRAY SIZE: %s\n\n",nav_state->size);
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
