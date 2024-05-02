#include "main.h"

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

    if (effect_mode)
    {
      run_effect(effect);
    }
    else
    {
      active_step = next_step(current_measure); // DEFAULT BEHAVIOR
    }

    if (LED_mode == LED_DEFAULT_MODE)
    {
      LED_routine(current_measure->beat, current_measure->step);
    }
    else if (LED_mode == LED_SOUND_SWEEP)
    {
      if (check_palette_sound(active_step))
      {
        LED_routine(current_measure->beat, current_measure->step);
      }
    }
#if DEBUG_PRINT == 1 // VERBOSE PRINT
    print_step(active_step, false);
#endif

    stop_step(last_step);
    play_step(active_step);

    if (effect_return_state != EFFECT_DOUBLE_REPEAT)
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
      effect_end();           // DISABLE EFFECT TOGGLE FLAG
      lcd_mode = LCD_DEFAULT; // end splash screen
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
      delete_step = false;
      wipe_step(button_step_lookup(current_measure));
      lcd_display(lcd, nav_state->lcd_state); // refresh LCD from splash screen
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
        if (testing_palette_combined[palette_index].effect != EFFECT_NULL)
        {
          effect_begin(); // ENABLE EFFECT TOGGLE FLAG
        }
        LED_routine(matrix_button.row, matrix_button.column); // light up pressed palette button
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

void print_ptr(void *ptr)
{
  char str[LCD_COLUMNS];
  sprintf(str, "%p", ptr); // Using sprintf to format the pointer address
  Serial.print(String(str));
}