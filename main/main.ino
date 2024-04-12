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
  {

    Serial.println("SERIAL INIT FAILED");
  }
#endif

  if (sd_init())
  {
    Serial.println("SD INIT FAILED");
  }

  if (midi_init())
  {
    Serial.println("MIDI INIT FAILED");
  }
  if (dpad_init())
  {
    Serial.println("DPAD INIT FAILED");
  }
  if (button_matrix_init())
  {
    Serial.println("BUTTON MATRIX INIT FAILED");
  }

  if (dac_init())
  {
    Serial.println("DAC INIT FAILED");
  }

  lcd = lcd_init(&lcd_cfg);
  delay(1000); // 3 second splash strart screen

  /* POPULATE DYNAMIC LISTS */
  nav_cfg->sounds_custom = sd_fetch_sounds();
  nav_cfg->effects = fetch_effects(); // static TODO
  nav_cfg->tracks_load = sd_fetch_tracks();
  nav_cfg->sounds_midi_melodic = fetch_midi_melodic_sounds();
  nav_cfg->sounds_midi_percussion = fetch_midi_percussion_sounds();
  nav_state = nav_init(nav_cfg);

  Serial.println("default track stats:");
  Serial.print("filename: ");
  Serial.println(active_track.filename);
  Serial.print("id: ");
  Serial.println(active_track.id);
  Serial.print("bpm: ");
  Serial.println(active_track.bpm);
  Serial.print("measure steps: ");
  Serial.println(active_track.measure_steps);

  lcd_display(lcd, nav_state->lcd_state); // move to start nav

  measure_palette_init();
  step_timer.interval(60000 / (4 * 10)); // TESTING STATIC TEMPO
  populate_default_measure();
  // testing_measure.beat = 0;
  // testing_measure.step = 0;
  Serial.println("PROGRAM LOOP BEGINS");
}

/* Main subroutine: follow software block diagram */
void loop()
{

#if USING_NEW_DS == 1
  if (step_timer.check() == 1)
  {
    last_step = active_step;
    temp_last_step = testing_measure.step;
    temp_last_beat = testing_measure.beat;

    if (testing_measure.effect_mode)
    {
      run_effect(effect);
    }
    else
    {
      // DEFAULT BEHAVIOR
      active_step = next_step(&testing_measure);
    }

    if (LED_mode == LED_DEFAULT_MODE)
    {

      // ON STEP, PLAY SOUNDS AND FLASH LED
      LED_Off(temp_last_beat, temp_last_step);
      LED_On(testing_measure.beat, testing_measure.step);
    }

    stop_step(last_step);
    play_step(active_step);

    if (effect_return_state != DOUBLE_REPEAT)
    {
      active_track.bpm = read_tempo();
      Serial.println(active_track.bpm);
    }
    else if (evenodd == 1)
    {
      active_track.bpm = active_track.bpm*2;
      Serial.println(active_track.bpm);
    }

    if (splash_screen_active == false)
    {
      update_tempo(lcd);
    }

    // UPDATE TIMER INTERVAL

    step_timer.interval(step_interval_calc(&testing_measure));

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

      if (add_remove_measure_sound(&testing_measure))
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
    if (testing_measure.effect_mode == false)
    {
      Serial.println("BEGIN EFFECT");
      testing_measure.effect_mode = true;
      effect = active_palette_effects[2 - (8 - matrix_button.column)];
    }
  }
  else
  {
    // SET EFFECT TOGGLE FLAG OFF
    if (testing_measure.effect_mode)
    {
      Serial.println("END EFFECT");
      testing_measure.effect_mode = false;

      // SET STEP STATE TO STEP WHEN EFFECT FIRST PRESSED
      if (effect_return_state == EFFECT_RETURN_SAVE)
      {
        testing_measure.beat = saved_beat;
        testing_measure.step = saved_step;
        active_step = &testing_measure.beat_list[saved_beat].step_list[saved_step];
      }

      // SET STEP STATE TO BEAT=0 STEP=0
      else if (effect_return_state == EFFECT_RETURN_RESET)
      {
        testing_measure.beat = 3;
        testing_measure.step = 5;
      }

      else if (effect_return_state == ECHO)
      {
        volume = 127;
        dac_vol = 1.0;
        amp1.gain(dac_vol);
        amp2.gain(dac_vol);
      }

      else if (effect_return_state == PAUSE_SOUNDS)
      {
        silent = 0;
      }

      else if (effect_return_state == SCRATCH)
      {
        evenodd = 0;
      }
      else if (effect_return_state == DOUBLE_REPEAT)
      {
        evenodd = 0;
        effect_return_state = -1;
      }

      // ELSE, LEAVE STEP STATE AT LAST EFFECT
    }
  }

  // // EXAMPLES OF BUTTON PRESSES TYPES AMD HOLDS
  // if (matrix_pressed(PALETTE_EFFECT, BUTTON_HELD))
  // {
  //   Serial.println("\nPALETTE EFFECT HELD\n");
  // }
  // if (matrix_pressed(BUTTON_MEASURE, BUTTON_HELD))
  // {
  //   Serial.println("\nBUTTON_MEASURE HELD\n");
  // }
  // if (matrix_pressed(BUTTON_PALETTE, BUTTON_HELD))
  // {
  //   Serial.println("\nBUTTON_PALETTE HELD\n");
  // }
  // if (matrix_pressed(BUTTON_SOUND, BUTTON_HELD))
  // {
  //   Serial.println("\nBUTTON_SOUND HELD\n");
  // }
  // if (matrix_pressed(BUTTON_EFFECT, BUTTON_HELD))
  // {
  //   Serial.println("\nBUTTON_EFFECT HELD\n");
  // }
  // single press
  // if (matrix_pressed(BUTTON_MEASURE, BUTTON_NOT_HELD))
  // {
  //   Serial.println("\nBUTTON_MEASURE BUTTON_NOT_HELD\n");
  // }
  // if (matrix_pressed(BUTTON_PALETTE, BUTTON_NOT_HELD))
  // {
  //   Serial.println("\nBUTTON_PALETTE BUTTON_NOT_HELD\n");
  // }
  // if (matrix_pressed(BUTTON_SOUND, BUTTON_NOT_HELD))
  // {
  //   Serial.println("\nBUTTON_SOUND BUTTON_NOT_HELD\n");
  // }
  // if (matrix_pressed(BUTTON_EFFECT, BUTTON_NOT_HELD))
  // {
  //   Serial.println("\nBUTTON_EFFECT BUTTON_NOT_HELD\n");
  // }

#endif

#if USING_NEW_DS == 0

  // Serial.println(SDmeMat[0][0]);
  if (step_timer.check() == 1)
  {
    // turning off all midi sounds on last step
    if (effectReverse == 0)
    {
      if (count_temp == 0)
      {
        // prevCount = 23;
        // 0  1  2  3  4  5
        // 6  7  8  9  10 11
        // 12 13 14 15 16 17
        // 18 19 20 21 22 23

        prevCount = 17 + active_track.measure_steps;
      }
      else if ((count_temp) % 6 == 0)
      {
        prevCount = count_temp - (6 - (active_track.measure_steps - 1));
      }
      else
      {
        prevCount = count_temp - 1;
      }
    }
    if (effectReverse == 1)
    {
      if (count_temp == 23 - (6 - active_track.measure_steps))
      {
        // prevCount = 23;
        // 0  1  2  3  4  5
        // 6  7  8  9  10 11
        // 12 13 14 15 16 17
        // 18 19 20 21 22 23

        prevCount = 0;
      }
      else if (count_temp % 6 > active_track.measure_steps - 1)
      {
        prevCount = count_temp + (6 - (active_track.measure_steps));
      }
      else
      {
        prevCount = count_temp + 1;
      }
    }
    if (effectReverseprevcount == 0 && effectReverse == 1)
    {
      Serial.print("here");
      int newPrevCount = prevCount - 2;
      LED_Off(MeMat_LEDindex[newPrevCount][0], MeMat_LEDindex[newPrevCount][1]);
    }
    Serial.print("count ");
    Serial.println(count_temp);
    Serial.print("prevcount ");
    Serial.println(prevCount);

    // TURN MIDI NOTES OFF
    for (int i = 0; i < 12; i += 3)
    {
      if (meMat[prevCount][i] == -1)
      {
        // SOUND NOT SET
        continue;
      }
      currBank = meMat[prevCount][i];
      if (currBank == 0)
      {
        currNote = meMat[prevCount][i + 1];
      }
      if (currBank == 1)
      {
        currNote = meMat[prevCount][i + 2];
      }
      midiNoteOff(currBank, currNote, 127);
    }

    // Turn on and off measure matrix LEDs

    if (stop == 1 && stopSD == 1 && dispFlag == 1)
    {
      LED_Off(MeMat_LEDindex[prevCount][0], MeMat_LEDindex[prevCount][1]);
      LED_On(MeMat_LEDindex[count_temp][0], MeMat_LEDindex[count_temp][1]);
    }

    if (stop == 1 && stopSD == 1 && dispFlag == 4)
    {
      LED_Off(MeMat_LEDindex[prevCount][0], MeMat_LEDindex[prevCount][1]);
      LED_On(MeMat_LEDindex[count_temp][0], MeMat_LEDindex[count_temp][1]);
    }

    // turning off all SD sounds on last step
    stopFile(0);

    // play SD sounds on measure matrix
    for (int i = 0; i < 4; i++)
    {
      if (cached_samples_sd[count_temp][i] != nullptr)
      {
        playFile(cached_samples_sd[count_temp][i]);
      }
    }
    // play MIDI sounds on measure matrix
    for (int i = 0; i < 12; i += 3)
    {
      if (meMat[count_temp][i] == 0)
      {
        // play percussion
        midiSetInstrument(0, 128);
        int channel = meMat[count_temp][i];
        int note = meMat[count_temp][i + 1];
        midiNoteOn(channel, note, 127);
        // currNote = note;
        // currBank = 0;
      }
      if (meMat[count_temp][i] == 1)
      {
        // play melodic
        int instrum = meMat[count_temp][i + 1];
        midiSetInstrument(1, instrum);
        int channel = meMat[count_temp][i];
        int note = meMat[count_temp][i + 2];
        midiNoteOn(channel, note, 127);
        // currNote = note;
        // currBank = 1;
      }
    }

    // step_timer.reset();

    //  == ((active_track.measure_steps-1) % 6)
    // steps =4
    //              V
    // 0  1  2  3  4  5
    // 6  7  8  9  10 11
    // 12 13 14 15 16 17
    // 18 19 20 21 22 23
    if (effectReverse == 0)
    {
      count_temp++;
      if (count_temp % 6 > active_track.measure_steps - 1) // If it is the last step in the beat
      {
        count_temp = count_temp + (6 - active_track.measure_steps);
      }
      if (count_temp == 24)
      // if (count_temp == (active_track.measure_steps * 4))
      {
        count_temp = 0;
      }
    }
    if (effectReverse == 1)
    {
      if (effectReverseprevcount == 0)
      {
        // count_temp--;
      }
      count_temp--;
      if ((count_temp + 1) % 6 == 0)
      {
        count_temp = count_temp - (6 - active_track.measure_steps);
      }
      if (count_temp < 0)
      {
        count_temp = 23 - (6 - active_track.measure_steps);
      }
      effectReverseprevcount++;
    }
    if (effectReverse == 2)
    {
    }
    /*************************     UPDATE TEMPO     *************************/

    // active_track.bpm = read_tempo();
    active_track.bpm = 50;

    if (splash_screen_active == false)
    {
      update_tempo(lcd);
    }
    metro_active_tempo = (60000 / (active_track.bpm * active_track.measure_steps));
    step_timer.interval(metro_active_tempo);
    /*************************     STEP STATEMENT ENDS     *************************/

    // delay(5000);
  }

  // palette effect button pressed
  if (Current_Button_State[1] > 5 && Current_Button_State[1] != 9 && Current_Button_State[0] == 3)
  {
    if (Current_Button_State[1] == 6)
    {
      effectReverse = 1;
      dispFlag = 4;
    }
    if (Current_Button_State[1] == 7)
    {
      effectReverse = 2;
      dispFlag = 5;
    }
    if (Current_Button_State[1] == 8)
    {
      effectReverse = 2;
      dispFlag = 6;
    }
  }

  // palette sound button pressed
  if (Current_Button_State[1] > 5 && Current_Button_State[1] != 9 && Current_Button_State[0] < 3)
  {
    // Serial.println("palette pushed");
    Current_Row = Current_Button_State[0];
    Current_Column = Current_Button_State[1];

    for (int i = 0; i < 12; i++)
    {
      if (Palette_LEDMatrix[i][0] == Current_Row && Palette_LEDMatrix[i][1] == Current_Column)
      {
        palette_index = i;
      }
    }

    // palette effect button pressed
    if (dispFlag == 1)
    {
      LED_On(Current_Row, Current_Column);
      stop = 0;
      stopSD = 0;
    }
    if (dispFlag == 0)
    {
      palette[palette_index][0] = dispBank;
      palette[palette_index][1] = dispInstrum;
      palette[palette_index][2] = dispNote;
      sd_palette[palette_index] = nullptr;
      splash_screen_active = false;
      lcd_display(lcd, nav_state->lcd_state);
      dispFlag = 2;
    }
    if (dispFlag == 3)
    {
      sd_palette[palette_index] = temp_sample;
      splash_screen_active = false;
      palette[palette_index][0] = -1;
      palette[palette_index][1] = -1;
      palette[palette_index][2] = -1;
      lcd_display(lcd, nav_state->lcd_state);
      dispFlag = 2;
    }
  }

  if (dispFlag == 2 && Current_Button_State[0] == 9 && Current_Button_State[1] == 9)
  {
    dispFlag = 1;
  }

  if (dispFlag == 4 && Current_Button_State[0] == 9 && Current_Button_State[1] == 9)
  {
    effectReverse = 0;
    effectReverseprevcount = 0;
    dispFlag = 1;
  }
  if (dispFlag == 5 && Current_Button_State[0] == 9 && Current_Button_State[1] == 9)
  {
    effectReverse = 0;
    dispFlag = 1;
    count_temp = 0;
  }
  if (dispFlag == 6 && Current_Button_State[0] == 9 && Current_Button_State[1] == 9)
  {
    effectReverse = 0;
    dispFlag = 1;
  }

  if (Current_Button_State[1] <= 5 && Current_Button_State[1] != 9 && palette_index != -1)
  {
    // Serial.println("measure pushed");
    Serial.println("test");
    int Current_Row1 = Current_Button_State[0];
    int Current_Column1 = Current_Button_State[1];
    int meMatConv = 6 * Current_Row1 + Current_Column1;
    LED_Off(Current_Row, Current_Column);
    int channel = palette[palette_index][0];
    int instr = palette[palette_index][1];
    int note = palette[palette_index][2];

    // Assigning SD sounds to measure matrix
    for (int i = 0; i < 4; i++)
    {
      if (cached_samples_sd[meMatConv][i] == sd_palette[palette_index] && stopSD == 0)
      {
        Serial.println("deleting");
        cached_samples_sd[meMatConv][i] = nullptr;
        stopSD = 1;
      }
    }
    for (int i = 0; i < 4; i++)
    {
      if (cached_samples_sd[meMatConv][i] == nullptr && stopSD == 0)
      {
        Serial.println("adding");
        cached_samples_sd[meMatConv][i] = sd_palette[palette_index];
        stopSD = 1;
      }
    }

    // Assigning MIDI sounds to measure matrix
    for (int i = 0; i < 12; i += 3)
    {
      if (meMat[meMatConv][i] == channel && meMat[meMatConv][i + 1] == instr && meMat[meMatConv][i + 2] == note && stop == 0)
      {
        meMat[meMatConv][i] = -1;
        meMat[meMatConv][i + 1] = -1;
        meMat[meMatConv][i + 2] = -1;
        stop = 1;
      }
    }
    for (int i = 0; i < 12; i += 3)
    {
      if (meMat[meMatConv][i] == -1 && stop == 0)
      {
        meMat[meMatConv][i] = channel;
        meMat[meMatConv][i + 1] = instr;
        meMat[meMatConv][i + 2] = note;
        stop = 1;
        // Serial.println("here");
      }
    }

    // Play note at current palette to see whats there
    // MUST ADD STOPING CURRENT MIDI AND CUSTOM SOUNDS TO WORK
    // if (palette[palette_index][0] != -1)
    // {

    //   if (palette[palette_index][0])
    //   {
    //     // Play melodic
    //     midiSetInstrument(palette[palette_index][0], palette[palette_index][1]);
    //     midiNoteOn(palette[palette_index][0], palette[palette_index][2], 127);
    //   }
    //   else
    //   {
    //     // play percussion
    //     midiSetInstrument(palette[palette_index][0], 128);
    //     midiNoteOn(palette[palette_index][0], palette[palette_index][1], 127);
    //   }
    // }
    // else if (sd_palette[palette_index] != nullptr)
    // {
    //   // play custom sound
    //   playFile(sd_palette[palette_index]);
    // }
    palette_index = -1;
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

  /****************************     DPAD LEFT      ****************************/
  if (dpad_pressed == BUTTON_DPAD_LEFT) // return / exit
  {
    nav_state = nav_selection(nav_state, NAV_BACKWARD);

    lcd_display(lcd, nav_state->lcd_state);
  }

  /****************************     DPAD DOWN      ****************************/
  if (dpad_pressed == BUTTON_DPAD_DOWN) // scroll down
  {
    array_scroll(nav_state, NAV_DOWN);
    lcd_display(lcd, nav_state->lcd_state);
  }

  /****************************     DPAD UP        ****************************/
  if (dpad_pressed == BUTTON_DPAD_UP) // scroll up
  {
    array_scroll(nav_state, NAV_UP);
    lcd_display(lcd, nav_state->lcd_state);
  }

  /****************************     DPAD RIGHT      ***************************/
  if (dpad_pressed == BUTTON_DPAD_RIGHT) // select
  {

    /*************************     TRACKS OPTIONS     *************************/

    /*
    SAVE TRACK
    */
    if (strcmp(nav_state->data_array[nav_state->index], "Save Track") == 0)
    {
      char *new_track_filename = (char *)malloc(LCD_COLUMNS + NULL_TERMINATION);
      snprintf(new_track_filename, LCD_COLUMNS + NULL_TERMINATION, "TRACK%d.json", (nav_state->child[2])->size);
      strncpy(active_track.filename, new_track_filename, 63); // Copy up to 63 characters to ensure null-termination
      active_track.filename[63] = '\0';

      active_track.id = (nav_state->child[2])->size;
      save_track(new_track_filename, active_track);

      track_list = sd_fetch_tracks();
      (nav_state->child[2])->data_array = track_list->array;
      (nav_state->child[2])->size = track_list->size;
      (nav_state->child[2])->index = 0;
      array_scroll(nav_state->child[2], 0);
      free(new_track_filename);
    }
    /*
    DELETE TRACK
    */
    else if (strcmp(nav_state->data_array[nav_state->index], "Delete Track") == 0)
    {
      char *delete_track_filename = (char *)malloc(LCD_COLUMNS + NULL_TERMINATION);
      snprintf(delete_track_filename, LCD_COLUMNS + NULL_TERMINATION, "TRACK%d.json", active_track.id);

      if (sd_delete_track(delete_track_filename))
      {
        // Serial.print("FAILED TO DELETE: ");
        Serial.println(active_track.filename);
      }
      else
      {
        track_list = sd_fetch_tracks();
        (nav_state->child[2])->data_array = track_list->array;
        (nav_state->child[2])->size = track_list->size;
        (nav_state->child[2])->index = 0;
        array_scroll(nav_state->child[2], 0);
        read_track((nav_state->child[2])->data_array[(nav_state->child[2])->size - 1], active_track);
      }
      free(delete_track_filename);
    }
    /*
    LOAD TRACKS
    */
    else if (strcmp(nav_state->name, "tracks_load") == 0)
    {
      read_track(nav_state->data_array[nav_state->index], active_track);
    }
    /*
    SET TRACK STEPS
    */
    else if (strcmp(nav_state->name, "tracks_set_steps") == 0)
    {
      active_track.measure_steps = nav_state->index + 1;
      for (int i = 0; i < 4; i++)
      {
        testing_measure.beat_list[i].active_steps = active_track.measure_steps;
      }
    }
    /*************************     SOUNDS SELECT     **************************/
    /*
    SELECTED MELODIC MIDI SOUND
    */
    else if (strcmp(nav_state->name, "sounds_midi_notes") == 0)
    {
      Serial.println("MIDI SOUNDS SELECTION:");
      Serial.print("BANK: ");
      Serial.println(((nav_state->parent)->parent)->name);
      Serial.print("Octave: ");
      Serial.println((nav_state->parent)->data_array[(nav_state->parent)->index]);
      Serial.print("Note: ");
      Serial.println(nav_state->data_array[nav_state->index]);
      Serial.print("TEST: ");
      Serial.println(midi_mapping[nav_state->index][(nav_state->parent)->index]);

      dispBank = sounds_midi_nav->index;
      dispInstrum = midi_melodic_values[sounds_midi_melodic_nav->index];
      dispNote = midi_mapping[sounds_midi_notes_nav->index][sounds_midi_octaves_nav->index];
      dispFlag = 0;

      new_sound.bank = sounds_midi_nav->index;
      new_sound.instrument = midi_melodic_values[sounds_midi_melodic_nav->index];
      new_sound.note = midi_mapping[sounds_midi_notes_nav->index][sounds_midi_octaves_nav->index];
      new_sound.sd_cached_sound = nullptr;
      new_sound_assignment = true;

      Serial.println(dispBank);
      Serial.println(dispInstrum);
      Serial.println(dispNote);

      // BANK
      // sounds_midi_nav->index;

      // melodic sound
      // midi_melodic_values[sounds_midi_melodic_nav->index];

      // midi standard mapping (octave & note)
      // midi_mapping[sounds_midi_notes_nav->index][sounds_midi_octaves_nav->index]
      lcd_splash(lcd, nav_state, selected_sound);
      nav_state = main_nav;
    }
    /*
    SELECTED PERCUSSION MIDI SOUND
    */
    else if (strcmp(nav_state->name, "sounds_midi_percussion") == 0)
    {
      // BANK
      // sounds_midi_nav->index;

      // currently selected midi sound
      // midi_percussion_values[sounds_midi_percussion_nav->index];
      dispBank = sounds_midi_nav->index;
      dispInstrum = midi_percussion_values[sounds_midi_percussion_nav->index];
      dispNote = -1;
      dispFlag = 0;

      new_sound.bank = sounds_midi_nav->index;
      new_sound.instrument = midi_percussion_values[sounds_midi_percussion_nav->index];
      new_sound.note = -1;
      new_sound.sd_cached_sound = nullptr;
      new_sound_assignment = true;

      Serial.println(dispBank);
      Serial.println(dispInstrum);
      Serial.println(dispNote);

      lcd_splash(lcd, nav_state, selected_sound);
      nav_state = main_nav;
    }
    /*
    SELECTED CUSTOM SOUND
    */
    else if (strcmp(nav_state->name, "custom_sounds") == 0)
    {
      // currently selected custom sound
      // sounds_custom_nav->data_array[sounds_custom_nav->index];
      temp_sample = cache_sd_sound(sounds_custom_nav->data_array[sounds_custom_nav->index]);
      // new_sound.sd_cached_sound = cache_sd_sound(sounds_custom_nav->data_array[sounds_custom_nav->index]);
      if (temp_sample != nullptr)
      {
        dispFlag = 3;

        new_sound.bank = -1;
        new_sound.instrument = -1;
        new_sound.note = -1;
        new_sound_assignment = true;
        new_sound.sd_cached_sound = temp_sample;
        lcd_splash(lcd, nav_state, selected_sound);

        char str[20];
        sprintf(str, "%p", (void *)temp_sample); // Using sprintf to format the pointer address
        Serial.println("\tEXPECTED: " + String(str));

        sprintf(str, "%p", (void *)new_sound.sd_cached_sound); // Using sprintf to format the pointer address
        Serial.println("\tACTUAL: " + String(str));
      }
      else
      {
        new_sound.sd_cached_sound = nullptr;
        // NO SIZE ON PSRAM TO CACHE SOUND
        lcd_splash(lcd, nullptr, error_psram_full);
      }
      nav_state = main_nav;
    }
    /************************     DEFAULT BEHAVIOR     ************************/
    /*
    SELECT NEXT NAV
    */
    else
    {
      nav_state = nav_selection(nav_state, NAV_FORWARD);
    }
    if (splash_screen_active == false)
    {
      lcd_display(lcd, nav_state->lcd_state);
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
  Serial.println("\n\n\n\n\nSerial Initialized<<<<<<<<<<<<<<");
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