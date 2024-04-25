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

  lcd_display(lcd, nav_state->lcd_state); // move to start nav
  lcd_display_banner(lcd, BANNER_DEFAULT, LCD_PERSIST);
  step_timer = Metro(step_interval_calc(current_measure)); // starting tmepo
  Serial.println("PROGRAM LOOP BEGINS");
}

/* Main subroutine: follow software block diagram */
void loop()
{
  /******************************************************************************************************/
  if (step_timer.check() && play_pause_toggle)
  {

    last_step = active_step;
    // temp_last_step = current_measure->step;
    // temp_last_beat = current_measure->beat;

    if (effect_mode)
    {
      run_effect(effect);
    }
    else
    {
      active_step = next_step(current_measure); // DEFAULT BEHAVIOR
    }

    Serial.print("LED_MODE=");
    Serial.println(LED_mode);

    if (LED_mode == LED_DEFAULT_MODE)
    {
      // ON STEP, PLAY SOUNDS AND FLASH LED
      // LED_Off(temp_last_beat, temp_last_step);
      // LED_On(current_measure->beat, current_measure->step);
      LED_routine(current_measure->beat, current_measure->step);
    }
    else if (LED_mode == LED_SOUND_SWEEP)
    {
      if (check_palette_sound(active_step))
      {
        // LED_Off(last_beat_mat, last_step_mat);
        // LED_On(current_measure->beat, current_measure->step);
        // last_beat_mat = current_measure->beat;
        // last_step_mat = current_measure->step;
        LED_routine(current_measure->beat, current_measure->step);
      }
    }
#if DEBUG_PRINT == 1 // VERBOSE PRINT
                     // print_step(active_step);
#endif

    stop_step(last_step);
    play_step(active_step);

    if (effect_return_state != DOUBLE_REPEAT)
    {
#if DYNAMIC_TEMPO == 1 // STATIC

      current_track->bpm = read_tempo();
#endif
    }
    else if (evenodd == 1)
    {
      current_track->bpm = current_track->bpm * 2;
    }

    // UPDATE TIMER INTERVAL
    step_timer.interval(step_interval_calc(current_measure));
  }
  /******************************************************************************************************/

  /**************************     PALETTE PRESSED     *************************/
  if (matrix_pressed(BUTTON_PALETTE, BUTTON_NOT_HELD)) // PALETTE BUTTON PRESSED
  {
    Serial.println("PALETTE PRESSED");

    if (effect_mode)
    {
      effect_end(); // DISABLE EFFECT TOGGLE FLAG
    }
    if (palette_assignment == PALETTE_ASSIGNMENT_DEFAULT)
    {
      lcd_splash_palette(lcd, testing_palette_combined[palette_index]); // avoid clearing select splash
      Serial.print("LED_MODE=");
      Serial.println(LED_mode);

      if (measure_edit)
      {
        LED_mode = LED_DEFAULT_MODE; // UNSELECT PALETTE BUTTON
        measure_edit = false;
        LED_routine(current_measure->beat, current_measure->step);
        lcd_mode = LCD_DEFAULT;                 // end splash screen
        lcd_display(lcd, nav_state->lcd_state); // refresh LCD from splash screen
      }
      else
      {
        if (!testing_palette_combined[palette_index].is_empty && testing_palette_combined[palette_index].effect == -1)
        {
          measure_edit = true;
          // LED_mode = LED_PALETTE_SELECT; // SELECT PALETTE BUTTON
          LED_mode = LED_DEFAULT_MODE; // UNSELECT PALETTE BUTTON
          LED_routine(matrix_button.row, matrix_button.column);
        }
      }
    }
    else
    {
      // SAVE NEW SOUND FROM NAV TO PALETTE BUTTON
      Serial.println("NAV TO PALETTE ASSIGNED");
      testing_palette_combined[palette_index] = new_palette_slot;
      palette_assignment = PALETTE_ASSIGNMENT_DEFAULT;
      lcd_mode = LCD_DEFAULT;                 // end splash screen
      lcd_display(lcd, nav_state->lcd_state); // refresh LCD from splash screen
    }
    // if (!testing_palette_combined[palette_index].is_empty)
    // {
    //   if (LED_mode == LED_PALETTE_SELECT && testing_palette_combined[palette_index].effect == -1)
    //   {
    //     measure_edit = true; // EVOKES add/remove sounds to measure steps
    //                          // LED_Off(temp_last_beat, temp_last_step);
    //                          // LED_On(matrix_button.row, matrix_button.column);
    //   }
    // }
  }

  /**************************     MEASURE PRESSED     *************************/
  if (matrix_pressed(BUTTON_MEASURE, BUTTON_NOT_HELD)) // MEASURE BUTTON PRESSED
  {
    if (!delete_step)
    {
      if (measure_edit)
      {
        // MEASURE BUTTON PRESSED
        Serial.println("PALETTE TO MEASURE ADD/REMOVE");
        if (add_remove_measure_sound(current_measure))
        {
          // ALLOCATED STEP SOUNDS FULL, CANNOT ADD PALETTE SOUND
        }
      }
      else
      {
        lcd_splash_step(lcd, button_step_lookup(current_measure));
      }
    }
    else
    {
      wipe_step(button_step_lookup(current_measure));
    }
  }

  /**************************     PALETTE HELD        *************************/
  if (matrix_pressed(BUTTON_PALETTE, BUTTON_HELD)) // PALETTE BUTTON HELD
  {
    if (effect_mode == false)
    {
      for (int i = 0; i < 12; i++)
      {
        // get palette index
        if (Palette_LEDMatrix[i][0] == matrix_button.row && Palette_LEDMatrix[i][1] == matrix_button.column)
        {
          palette_index = i;
        }
      }
      if (!testing_palette_combined[palette_index].is_empty)
      {
        effect_begin(); // ENABLE EFFECT TOGGLE FLAG
      }
    }
  }
  /******************************************************************************************************/

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
#if DEBUG_PRINT == 1 // VERBOSE PRINT

      // Serial.print("BUTTON PRESSED\t\tROW:");
      // Serial.print(matrix_button.row);
      // Serial.print("\tCOLUMN:");
      // Serial.println(matrix_button.column);

      // print_palette(PALETTE_SIZE);
#endif
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

  switch (lcd_mode)
  {
  case LCD_DEFAULT:
  {
    // banner stats: bpm, measure id, track id
    if (millis() - banner_refresh_start > 100)
    {
      banner_refresh_start = millis();
      lcd_display_banner(lcd, BANNER_DEFAULT, LCD_PERSIST);
    }
    break;
  }

  case LCD_SPLASH_TIMED:
  {
    if (millis() - timed_splash_start > VANISH_PERIOD)
    {
      lcd_mode = LCD_DEFAULT;
      lcd_display(lcd, nav_state->lcd_state);
    }
    break;
  }
  case LCD_BANNER_TIMED:
  {
    if (millis() - timed_banner_start > VANISH_PERIOD)
    {
      lcd_mode = LCD_DEFAULT;
    }
    break;
  }
  }
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
  char str[LCD_COLUMNS];
  sprintf(str, "%p", ptr); // Using sprintf to format the pointer address
  Serial.print(String(str));
}