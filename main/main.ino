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

  /*if (sd_init())
  {
    Serial.println("SD INIT FAILED");
  }*/


   if (midi_init())
  {
     Serial.println("MIDI INIT FAILED");
   }
  // if (dpad_init(dpad_cfg))
  // {
  //   Serial.println("DPAD INIT FAILED");
  // }
   if (button_matrix_init())
  {
    Serial.println("BUTTON MATRIX INIT FAILED");
  }
  // if (dac_init())
  // {
  //   Serial.println("DAC INIT FAILED");
  // }

  // lcd = lcd_init(&lcd_cfg);
  delay(3000); // 3 second splash strart screen

  /* POPULATE DYNAMIC LISTS */
  //nav_cfg->sounds_custom = sd_fetch_sounds();
  // nav_cfg->sounds_midi = fetch_midi_sounds(); // static TODO
  // nav_cfg->effects = fetch_effects();         // static TODO
 // nav_cfg->tracks_load = sd_fetch_tracks();
  // nav_state = nav_init(nav_cfg);
  // nav_data_structure = nav_init(nav_cfg);
  // nav_state = (lcd_nav *)malloc(sizeof(lcd_nav));
  // nav_state = nav_data_structure;

  // lcd_display(lcd, nav_state->lcd_state); // move to start nav

  // for (int i = 0; i < 4; i++)
  // {
  //   cached_samples[i] = cache_sd_sound((nav_cfg->sounds_custom)->array[2]);
  // }
  Serial.println("PROGRAM LOOP BEGINS");
}

/* Main subroutine: follow software block diagram */
void loop()
{


if (ledMetro.check() == 1)
  {

    // turning off all midi sounds on last step
    if (count_temp == 0){
      prevCount = 23;
    }
    else
    {
      prevCount = count_temp - 1;
    }
    for (int i = 0; i < 6; i+=3) 
    {
      currBank = meMat[prevCount][i];
      if (currBank == 0)
      {
        currNote = meMat[prevCount][i+1];
      }
      if (currBank == 1)
      {
        currNote = meMat[prevCount][i+2];
      }
      midiNoteOff(currBank, currNote, 127);
    }

    // Turn on and off measure matrix LEDs
    LED_Off(MeMat_LEDindex[prevCount][0],MeMat_LEDindex[prevCount][1]);

    if (stop==1){
      LED_On(MeMat_LEDindex[count_temp][0],MeMat_LEDindex[count_temp][1]);
    }

    // play sounds on measure matrix
    for (int i = 0; i < 6; i+=3) {
      if (meMat[count_temp][i] == 0)
      {
        midiSetInstrument(0,128);
        int channel = meMat[count_temp][i];
        int note = meMat[count_temp][i+1];
        midiNoteOn(channel, note, 127);
        //currNote = note;
       // currBank = 0;
      }
      if (meMat[count_temp][i] == 1) {
        int instrum = meMat[count_temp][i+1];
        midiSetInstrument(1,instrum);
        int channel = meMat[count_temp][i];
        int note = meMat[count_temp][i+2];
        midiNoteOn(channel, note, 127);
       // currNote = note;
        //currBank = 1;
      }
    }
    count_temp++;
    if (count_temp == 24)
    {
      count_temp = 0;
    }
    ledMetro.reset();
  }  

  if(Current_Button_State[1]>=5 && Current_Button_State[1]!=9){
    
    Current_Row = Current_Button_State[0];
    Current_Column = Current_Button_State[1];
    LED_On(Current_Row, Current_Column);
    stop = 0;

    for(int i=0 ; i<12 ; i++){
      if(Palette_LEDMatrix[i][0] == Current_Row && Palette_LEDMatrix[i][1] == Current_Column ){
        palbut=i;
      }
    }
  }
  if(Current_Button_State[1]<5 && Current_Button_State[1]!=9 && palbut!= -1){
    LED_Off(Current_Row,Current_Column);
    int channel = palette[palbut][0];
    int instr = palette[palbut][1];
    int note = palette[palbut][2];
    for (int i = 0; i < 6; i+=3)
    {
      if (meMat[0][i] == channel && meMat[0][i+1] == instr && meMat[0][i+2] == note && stop == 0)
      {
        meMat[0][i] = -1;
        meMat[0][i+1] = -1;
        meMat[0][i+2] = -1;
        stop = 1;
      }
    }
    for (int i = 0; i < 6; i+=3)
    {
      if (meMat[0][i] == -1 && stop == 0)
      {
        meMat[0][i] = channel;
        meMat[0][i+1] = instr;
        meMat[0][i+2] = note;
        stop = 1;
        //Serial.println("here");
      }
    }
    palbut = -1;
  }


unsigned long currentMillis_matrix = millis();

  if (currentMillis_matrix - previousMillis >= interval) {
    
    if (Pressed==0){
      readMatrix();
    }

    if (Pressed==1){
      Button_Pressed(Current_Button_State, Previous_Button_State);
    }

    Previous_Button_State[0] = Current_Button_State[0];
    Previous_Button_State[1] = Current_Button_State[1];
  }

  // if (button_pressed(BUTTON_DPAD_LEFT)) // return / exit
  // {
  //   nav_state = nav_selection(nav_state, NAV_BACKWARD);

  //   lcd_display(lcd, nav_state->lcd_state);
  // }
  // if (button_pressed(BUTTON_DPAD_DOWN)) // scroll down
  // {
  //   array_scroll(nav_state, NAV_DOWN);
  //   lcd_display(lcd, nav_state->lcd_state);
  // }
  // if (button_pressed(BUTTON_DPAD_UP)) // scroll up
  // {
  //   array_scroll(nav_state, NAV_UP);
  //   lcd_display(lcd, nav_state->lcd_state);
  // }
  // if (button_pressed(BUTTON_DPAD_RIGHT)) // select
  // {
  //   if (strcmp(nav_state->data_array[nav_state->index], "Save Track") == 0)
  //   {
  //     char *new_track_filename = (char *)malloc(20 + 1);
  //     snprintf(new_track_filename, 20 + 1, "TRACK%d.json", (nav_state->child[2])->size);
  //     strncpy(active_track.filename, new_track_filename, 63); // Copy up to 63 characters to ensure null-termination
  //     active_track.filename[63] = '\0';

  //     active_track.id = (nav_state->child[2])->size;
  //     save_track(new_track_filename, active_track);

  //     track_list = sd_fetch_tracks();
  //     (nav_state->child[2])->data_array = track_list->array;
  //     (nav_state->child[2])->size = track_list->size;
  //     (nav_state->child[2])->index = 0;
  //     array_scroll(nav_state->child[2], 0);
  //     free(new_track_filename);
  //   }
  //   else if (strcmp(nav_state->data_array[nav_state->index], "Delete Track") == 0)
  //   {
  //     char *delete_track_filename = (char *)malloc(20 + 1);
  //     snprintf(delete_track_filename, 20 + 1, "TRACK%d.json", active_track.id);

  //     if (sd_delete_track(delete_track_filename))
  //     {
  //       Serial.print("FAILED TO DELETE: ");
  //       Serial.println(active_track.filename);
  //     }
  //     else
  //     {
  //       track_list = sd_fetch_tracks();
  //       (nav_state->child[2])->data_array = track_list->array;
  //       (nav_state->child[2])->size = track_list->size;
  //       (nav_state->child[2])->index = 0;
  //       array_scroll(nav_state->child[2], 0);
  //       read_track((nav_state->child[2])->data_array[(nav_state->child[2])->size - 1], active_track);
  //     }
  //     free(delete_track_filename);
  //   }
  //   else if (strcmp(nav_state->name, "tracks_load") == 0)
  //   {
  //     read_track(nav_state->data_array[nav_state->index], active_track);
  //   }
  //   else if (strcmp(nav_state->name, "tracks_set_steps") == 0)
  //   {
  //     active_track.measure_steps = nav_state->index + 1;
  //   }
  //   else
  //   {
  //     nav_state = nav_selection(nav_state, NAV_FORWARD);
  //   }
  //   lcd_display(lcd, nav_state->lcd_state);
  // }





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
