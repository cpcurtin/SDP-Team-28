#include "main.h"

/*

PERFORMANCE TESTING

unsigned long start_time = millis();
unsigned long end_time = millis();
Calculate the difference in time unsigned long time_diff = end_time - start_time;
// Serial.print("Time elapsed: ");
// Serial.print(time_diff);
Serial.println(" milliseconds");
*/

void setup()
{

/* Intialize hardware */
// READ THIS!!!!!!! - If you are trying to run on battery power, take out the serial_init() below!!!!!!!!
#if BATTERY_OPERATED == 0
  if (serial_init())
    Serial.println("SERIAL INIT FAILED");
  else
    Serial.println("SERIAL INIT SUCCESS");

#endif

  if (sd_init())
    Serial.println("SD INIT FAILED");
  else
    Serial.println("SD INIT SUCCESS");

  if (midi_init())
    Serial.println("MIDI INIT FAILED");
  else
    Serial.println("MIDI INIT SUCCESS");

  if (dpad_init())
    Serial.println("DPAD INIT FAILED");
  else
    Serial.println("DPAD INIT SUCCESS");

  if (button_matrix_init())
    Serial.println("BUTTON MATRIX INIT FAILED");
  else
    Serial.println("BUTTON MATRIX INIT SUCCESS");

  if (dac_init())
    Serial.println("DAC INIT FAILED");
  else
    Serial.println("DAC INIT SUCCESS");

  if (track_init())
    Serial.println("TRACK INIT FAILED");
  else
    Serial.println("TRACK INIT SUCCESS");

  if (measure_palette_init())
    Serial.println("MEASURE PALETTE INIT FAILED");
  else
    Serial.println("MEASURE PALETTE INIT SUCCESS");

  lcd = lcd_init(&lcd_cfg);
  delay(1000); // 3 second splash strart screen

#if USING_SAFE_STRINGS == 1 // safe - new
  nav_cfg->sounds_custom = sd_fetch_sounds();
  nav_cfg->effects = fetch_effects(); // static TODO
  nav_cfg->tracks_load = sd_fetch_tracks();
  nav_cfg->sounds_midi_melodic = fetch_midi_melodic_sounds();
  nav_cfg->sounds_midi_percussion = fetch_midi_percussion_sounds();
  nav_state = nav_init(nav_cfg);

  Serial.println("default Track stats:");
  Serial.print("filename: ");
  Serial.println(current_track->filename.c_str());
  Serial.print("id: ");
  Serial.println(current_track->id);
  Serial.print("bpm: ");
  Serial.println(current_track->bpm);
  Serial.print("measure steps: ");
  Serial.println(current_track->measure_steps);

#else // unsafe - old
  /* POPULATE DYNAMIC LISTS */
  nav_cfg->sounds_custom = sd_fetch_sounds();
  nav_cfg->effects = fetch_effects(); // static TODO
  nav_cfg->tracks_load = sd_fetch_tracks();
  nav_cfg->sounds_midi_melodic = fetch_midi_melodic_sounds();
  nav_cfg->sounds_midi_percussion = fetch_midi_percussion_sounds();
  nav_state = nav_init(nav_cfg);

  Serial.println("default Track stats:");
  Serial.print("filename: ");
  Serial.println(current_track->filename);
  Serial.print("id: ");
  Serial.println(current_track->id);
  Serial.print("bpm: ");
  Serial.println(current_track->bpm);
  Serial.print("measure steps: ");
  Serial.println(current_track->measure_steps);

#endif

  lcd_display(lcd, nav_state->lcd_state); // move to start nav

  measure_palette_init();
  step_timer.interval(60000 / (4 * 10)); // TESTING STATIC TEMPO
  // populate_default_measure();
  // testing_measure.beat = 0;
  // testing_measure.step = 0;
  Serial.println("PROGRAM LOOP BEGINS");
}

/* Main subroutine: follow software block diagram */
void loop()
{
  // Serial.println(".");
#if USING_NEW_DS == 1
  if (step_timer.check() == 1)
  {
    Serial.println("testing 0");
    last_step = active_step;
    Serial.println("testing 1");
    temp_last_step = current_measure->step;
    Serial.println("testing x");
    temp_last_beat = current_measure->beat;
    Serial.println("testing 2");

    if (current_measure->effect_mode)
    {
      run_effect(effect);
    }
    else
    {
      // DEFAULT BEHAVIOR
      active_step = next_step(current_measure);
    }
    Serial.println("testing 3");

    if (LED_mode == LED_DEFAULT_MODE)
    {

      // ON STEP, PLAY SOUNDS AND FLASH LED
      LED_Off(temp_last_beat, temp_last_step);
      LED_On(current_measure->beat, current_measure->step);
    }
    Serial.println("testing 4");

    // print_step(current_measure);
    print_step(active_step);

    stop_step(last_step);
    play_step(active_step);

    if (effect_return_state != DOUBLE_REPEAT)
    {
      current_track->bpm = read_tempo();
      Serial.println(current_track->bpm);
    }
    else if (evenodd == 1)
    {
      current_track->bpm = current_track->bpm * 2;
      // Serial.println(current_track->bpm);
    }

    if (splash_screen_active == false)
    {
      update_tempo(lcd);
    }
    Serial.println("testing 5");

    // UPDATE TIMER INTERVAL
    step_timer.interval(step_interval_calc(current_measure));
  }

  //  LED ASSIGN NAV TO PALETTE
  if (new_sound_assignment)
  {
    LED_mode = LED_PALETTE_SELECT;
  }

  //  PALETTE BUTTON PRESSED (SOUNDS)
  if (matrix_pressed(BUTTON_SOUND, BUTTON_NOT_HELD))
  {
    Serial.println("PALETTE SOUND PRESSED");
    if (LED_mode == LED_PALETTE_SELECT)
    {
      // UNSELECT PALETTE BUTTON
      LED_mode = LED_DEFAULT_MODE;
    }
    else
    {
      // SELECT PALETTE BUTTON
      LED_mode = LED_PALETTE_SELECT;
    }

    // get palette index
    for (int i = 0; i < 12; i++)
    {
      if (Palette_LEDMatrix[i][0] == matrix_button.row && Palette_LEDMatrix[i][1] == matrix_button.column)
      {
        palette_index = i;
      }
    }

    if (new_sound_assignment)
    {

      // SAVE NEW SOUND FROM NAV TO PALETTE BUTTON
      Serial.println("NAV TO PALETTE ASSIGNED");
      testing_palette[palette_index] = new_sound;

      new_sound_assignment = false;
      splash_screen_active = false;
      lcd_display(lcd, nav_state->lcd_state);
      print_palette(PALETTE_SIZE);
    }
    else
    {
      if (LED_mode == LED_PALETTE_SELECT)
      {
        // EVOKES add/remove sounds to measure steps
        measure_edit = true;
        LED_last_row = matrix_button.row;
        LED_last_column = matrix_button.column;
        LED_Off(temp_last_beat, temp_last_step);
        LED_On(matrix_button.row, matrix_button.column);
      }
    }
  }

  //  MEASURE BUTTON PRESSED
  if (measure_edit)
  {
    if (matrix_pressed(BUTTON_MEASURE, BUTTON_NOT_HELD))
    {
      // MEASURE BUTTON PRESSED
      Serial.println("PALETTE TO MEASURE ADD/REMOVE");
      print_palette(palette_index);

      LED_mode = LED_DEFAULT_MODE;
      LED_Off(LED_last_row, LED_last_column);

      if (add_remove_measure_sound(current_measure))
      {
        // ALLOCATED STEP SOUNDS FULL, CANNOT ADD PALETTE SOUND
      }

      measure_edit = false; // chain sound assignment in future starting here
    }
  }

  // EFFECT
  if (matrix_pressed(BUTTON_EFFECT, BUTTON_HELD))
  {
    // SET EFFECT TOGGLE FLAG ON
    if (current_measure->effect_mode == false)
    {
      Serial.println("BEGIN EFFECT");
      current_measure->effect_mode = true;
      effect = active_palette_effects[2 - (8 - matrix_button.column)];
    }
  }
  else
  {

    // SET EFFECT TOGGLE FLAG OFF
    if (current_measure->effect_mode)
    {
      Serial.println("END EFFECT");
      current_measure->effect_mode = false;

      // SET STEP STATE TO STEP WHEN EFFECT FIRST PRESSED
      if (effect_return_state == EFFECT_RETURN_SAVE)
      {
        current_measure->beat = saved_beat;
        current_measure->step = saved_step;
        active_step = &(current_measure->beat_list[saved_beat].step_list[saved_step]);
      }

      // SET STEP STATE TO BEAT=0 STEP=0
      else if (effect_return_state == EFFECT_RETURN_RESET)
      {
        current_measure->beat = 0;
        current_measure->step = 0;
      }

      // ELSE, LEAVE STEP STATE AT LAST EFFECT
    }
  }

#endif

  /*****************************************************************************
   ************************     READ MATRIX BUTTONS     ************************
   ****************************************************************************/
  currentMillis_matrix = millis();
  if (currentMillis_matrix - previousMillis >= interval)
  {

    if (Pressed == 0)
    {
      // cycles matrix, sets Current_Button_State to press button, sets pressed flag
      readMatrix();
    }

    if (Pressed == 1)
    {
      //
      Button_Pressed(Current_Button_State, Previous_Button_State);
    }

    Previous_Button_State[0] = Current_Button_State[0];
    Previous_Button_State[1] = Current_Button_State[1];
  }

  /*****************************************************************************
   **************************     READ DPAD INPUTS     *************************
   ****************************************************************************/

  dpad_pressed = dpad_read();

  dpad_nav_routine(dpad_pressed);
}

int serial_init(void)
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect.
  }
  return 0;
}

// void printMemory(void)
// {
//   // Get total memory size
//   uint32_t total_memory = 0;
//   uint32_t free_memory = 0;

//   total_memory = RAM_SIZE; // RAM_SIZE is a macro defined in Teensy's core libraries

//   // Get free memory size
//   free_memory = getFreeMemory();

//   // Print total and free memory
//   Serial.print("Total Memory: ");
//   Serial.print(total_memory);
//   Serial.println(" bytes");
//   Serial.print("Free Memory: ");
//   Serial.print(free_memory);
//   Serial.println(" bytes");
// }

// uint32_t getFreeMemory(void)
// {
//   // Calculate free memory
//   char *stack_top = (char *)0x20008000; // Teensy 4.1 stack pointer
//   char *heap_top = _sbrk(0);            // Current heap pointer

//   uint32_t free_memory = stack_top - heap_top;

//   return free_memory;
// }

void print_ptr(void *ptr)
{
  char str[20];
  sprintf(str, "%p", ptr); // Using sprintf to format the pointer address
  Serial.print(String(str));
}