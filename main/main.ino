#include "main.h"

/*

PERFORMANCE TESTING 

unsigned long start_time = millis();
unsigned long end_time = millis();
Calculate the difference in time unsigned long time_diff = end_time - start_time;
Serial.print("Time elapsed: ");
Serial.print(time_diff);
Serial.println(" milliseconds");
*/



void setup()
{

  /* Intialize hardware */
  serial_init();
  sd_init();
  dpad_init(dpad_cfg);
  test_init();
  onboard_dac_init();

  // INITIALIZE AND POPULATE NAV ARRAYS DYNAMICALLY
  nav_cfg = (struct nav_config *)malloc(sizeof(struct nav_config));
  nav_cfg->effects = (array_with_size *)malloc(sizeof(array_with_size));
  nav_cfg->tracks_load = (array_with_size *)malloc(sizeof(array_with_size));
  nav_cfg->sounds_custom = (array_with_size *)malloc(sizeof(array_with_size));
  nav_cfg->sounds_midi = (array_with_size *)malloc(sizeof(array_with_size));

  // COMPLETE
  const char **nav_effects = new const char *[2];
  nav_effects[0] = strdup("effect1");
  nav_effects[1] = strdup("effect2");
  ((nav_cfg->effects)->array) = nav_effects;
  ((nav_cfg->effects)->size) = 2; // sizeof(nav_effects) / sizeof(nav_effects[0]);

  // IMPORT
  const char **nav_sounds_midi = new const char *[2];
  nav_sounds_midi[0] = strdup("midi1");
  nav_sounds_midi[1] = strdup("midi2");
  (nav_cfg->sounds_midi)->array = nav_sounds_midi;
  (nav_cfg->sounds_midi)->size = 2; // sizeof(nav_sounds_midi) / sizeof(nav_sounds_midi[0]);

  // PARSE SD CARD
  sd_fetch_sounds();
  nav_cfg->sounds_custom = sound_list;

  // TRACKS LOAD
  sd_fetch_tracks();
  nav_cfg->tracks_load = track_list;

  lcd = lcd_init(&lcd_cfg);
  nav_data_structure = nav_init(nav_cfg);
  nav_state = (lcd_nav *)malloc(sizeof(lcd_nav));
  nav_state = nav_data_structure;

  delay(3000);                            // 3 second splash strart screen
  lcd_display(lcd, nav_state->lcd_state); // move to start nav

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
  for (int i = 0; i < 4; i++)
  {
    cached_samples[i] = cache_sd_sound((nav_cfg->sounds_custom)->array[2]);
  }
  Serial.println("PROGRAM LOOP BEGINS");
}

/* Main subroutine: follow software block diagram */
void loop()
{
  // Main Timing Loop for 4x4 Measure Matrix
  if (ledMetro.check() == 1)
  {

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
    if (strcmp(nav_state->ptr_str_array[nav_state->index], "Save Track") == 0)
    {
      char *temp_str5 = (char *)malloc(20 + 1);
      snprintf(temp_str5, 20 + 1, "TRACK%d.json", (nav_state->child[1])->size);

      active_track.id = (nav_state->child[1])->size;
      save_track(temp_str5, active_track);

      sd_fetch_tracks();
      (nav_state->child[1])->ptr_str_array = track_list->array;
      (nav_state->child[1])->size = track_list->size;
      (nav_state->child[1])->index = 0;
      array_scroll(nav_state->child[1], 0);

      free(temp_str5);
    }
    else if (strcmp(nav_state->name, "tracks_load") == 0)
    {
      read_track(nav_state->ptr_str_array[nav_state->index], active_track);
    }
    else if (strcmp(nav_state->name, "tracks_set_steps") == 0)
    {
      active_track.measure_steps = nav_state->index + 1;
    }
    else
    {
      nav_state = nav_selection(nav_state, NAV_FORWARD);
    }

    lcd_display(lcd, nav_state->lcd_state);
  }
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
