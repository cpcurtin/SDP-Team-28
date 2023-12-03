/*
 *       MODULOOP main
 *
 *    PINOUT:
 *    PIN0:   MIDI RX (not used)
 *    PIN1:   MIDI TX - Serial1
 *    PIN2:   LCD - Register Select
 *    PIN3:   LCD - Enable display
 *    PIN4:   LCD - Digital 4
 *    PIN5:   LCD - Digital 5
 *    PIN6:   LCD - Digital 6
 *    PIN7:   DAC - DIN
 *    PIN8:   LCD - Digital 7
 *    PIN9:   MIDI Reset
 *    PIN10:
 *    PIN11:
 *    PIN12:
 *    PIN13:
 *    PIN14:
 *    PIN15:
 *    PIN16:
 *    PIN17:
 *    PIN18:
 *    PIN19:
 *    PIN20:   DAC - WS
 *    PIN21:   DAC - BCK
 *    PIN22:
 *    PIN23:
 *    PIN24:   DPAD - LEFT
 *    PIN25:   DPAD - DOWN
 *    PIN26:   DPAD - UP
 *    PIN27:   DPAD - RIGHT
 *    PIN28:   PALLET - TEST1
 *    PIN30:   PALLET - TEST2
 *    PIN29:   PALLET - TEST3
 *    PIN31:
 *    PIN32:
 *    PIN33:
 *    PIN34:
 *    PIN35:
 *    PIN36:
 *    PIN37:
 *    PIN38:
 *    PIN39:
 *    PIN40:
 *    PIN41:
 *    PIN42:
 *    PIN43:
 *    PIN44:
 *    PIN45:
 *    PIN46:
 *    PIN47:
 *    PIN48:
 *    PIN49:
 *    PIN50:
 *    PIN51:
 *    PIN52:
 *
 */

/* MODULE LINKING */
// #include "example-module.h"
#include "lcd-module.h"
#include "sd-storage-module.h"
#include "button-ui-module.h"
#include "custom-sound-module.h"
#include "test-module.h"
// #include <LiquidCrystal.h>

/* PIN MACROS */
// DAC
#define DAC_DIN 7
#define DAC_WS 20
#define DAC_BCK 21

// LCD
#define LCD_RS 2
#define LCD_EN 3
#define LCD_DIGITAL_4 4
#define LCD_DIGITAL_5 5
#define LCD_DIGITAL_6 6
#define LCD_DIGITAL_7 8
#define LCD_ROWS 2
#define LCD_COLUMNS 16

// BUTTONS
#define BUTTON_DPAD_UP 26
#define BUTTON_DPAD_DOWN 25
#define BUTTON_DPAD_LEFT 24
#define BUTTON_DPAD_RIGHT 27

#define BUTTON_PALETTE_1 28
#define BUTTON_PALETTE_2 29
#define BUTTON_PALETTE_3 30

/**************************
Midi Definitions 
**************************/
// Pins
#define VS1053 1
#define VS1053_RST 9
// Sound Banks
#define DEFAULT 0x00
#define Melody 0x79
#define Drums1 0x78
#define Drums2 0x7F
// Commands
#define NoteOn 0x90
#define NoteOff 0x80
#define MESSAGE 0xB0
#define BANK 0x00
#define VOLUME 0x07
#define PROGRAM 0xC0
#define OFF1 0x7B
#define OFF2 0x7C
#define OFF3 0x7D
// Define Serial Bus
#define MIDI Serial1
// Sound Addresses
#define Snare 38
#define Kick 35
#define HiHat 42
#define Crash 49

struct lcd_pin_config
{
  const int rs;
  const int en;
  const int dig4;
  const int dig5;
  const int dig6;
  const int dig7;
  const int rows;
  const int columns;
};

struct dac_pin_config
{
  const int din;
  const int ws;
  const int bck;
};

struct dpad_pin_config
{
  const int up;
  const int down;
  const int left;
  const int right;
  const int select;
};
struct lcd_nav
{
  char *name;
  char **ptr_str_array;
  struct lcd_nav *parent;
  struct lcd_nav **child;
  size_t size;
  char **lcd_state;
  int index;
  int depth;
};

struct nav_config
{
  struct array_with_size *sounds_custom;
  struct array_with_size *sounds_midi;
  struct array_with_size *effects;
};
// struct lcd_nav
// {
//   char *name;
//   char **ptr_str_array;
//   struct lcd_nav **parent;
//   struct lcd_nav **child;
//   size_t size;
//   char **lcd_state;
//   int index;
//   int depth;
// };

LiquidCrystal *lcd;

char **lcd_state = new char *[LCD_ROWS];
int lcd_index = 0;
struct lcd_nav *sounds;
struct lcd_nav *nav_data_structure;
struct lcd_nav *nav_state;

char *selection;

const struct lcd_pin_config lcd_cfg = {LCD_RS, LCD_EN, LCD_DIGITAL_4, LCD_DIGITAL_5, LCD_DIGITAL_6, LCD_DIGITAL_7, LCD_ROWS, LCD_COLUMNS};
const struct dac_pin_config dac_cfg = {DAC_DIN, DAC_WS, DAC_BCK};
const struct dpad_pin_config dpad_cfg = {BUTTON_DPAD_LEFT, BUTTON_DPAD_DOWN, BUTTON_DPAD_UP, BUTTON_DPAD_RIGHT, BUTTON_PALETTE_1};

void setup()
{

  /* Intialize hardware */
  serial_init();
  sd_init();
  dpad_init(dpad_cfg);
  test_init();
  onboard_dac_init();

  struct nav_config *nav_cfg = (struct nav_config *)malloc(sizeof(struct nav_config));
  nav_cfg->effects = (struct array_with_size *)malloc(sizeof(struct array_with_size));
  nav_cfg->sounds_custom = (struct array_with_size *)malloc(sizeof(struct array_with_size));
  nav_cfg->sounds_midi = (struct array_with_size *)malloc(sizeof(struct array_with_size));

  Serial.println("made it here1");

  char **nav_effects = new char *[2];
  nav_effects[0] = strdup("effect1");
  nav_effects[1] = strdup("effect2");
  ((nav_cfg->effects)->array) = nav_effects;
  ((nav_cfg->effects)->size) = 2;

  Serial.println("made it here2");

  char **nav_sounds_midi = new char *[2];
  nav_sounds_midi[0] = strdup("midi1");
  nav_sounds_midi[1] = strdup("midi2");
  (nav_cfg->sounds_midi)->array = nav_sounds_midi;
  (nav_cfg->sounds_midi)->size = 2;

  Serial.println("made it here3");

  (nav_cfg->sounds_custom = parsefiles());
  Serial.println("made it here");

  lcd = lcd_init(lcd_cfg);
  nav_data_structure = nav_init(nav_cfg);
  nav_state = (struct lcd_nav *)malloc(sizeof(struct lcd_nav));
  nav_state = nav_data_structure;

  lcd_display(lcd, nav_state->lcd_state);

  Serial.println("PROGRAM LOOP BEGINS");
  delay(3000);

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

}

/* Main subroutine: follow software block diagram */
void loop()
{

  /* play music config no matter what */

  /* on input */
  // check input - beat matrix, palette, dpad
  //  if(check_ninput(dpad_cfg)){

  //   }


  if (button_pressed(BUTTON_DPAD_LEFT))
  {
    nav_state = nav_selection(nav_state, -1);

    Serial.println("got here?");
    lcd_display(lcd, nav_state->lcd_state);
  }
  if (button_pressed(BUTTON_DPAD_DOWN))
  {
    array_scroll(nav_state, 1);
    Serial.printf("%d %s\n", nav_state->index, nav_state->ptr_str_array[nav_state->index]);
    lcd_display(lcd, nav_state->lcd_state);
  }
  if (button_pressed(BUTTON_DPAD_UP))
  {
    array_scroll(nav_state, -1);
    Serial.printf("%d %s\n", nav_state->index, nav_state->ptr_str_array[nav_state->index]);
    lcd_display(lcd, nav_state->lcd_state);
  }
  if (button_pressed(BUTTON_DPAD_RIGHT))
  {
    nav_state = nav_selection(nav_state, 1);
    // Serial.printf("forward, %s\n",selection);
    lcd_display(lcd, nav_state->lcd_state);
  }

  /* RIGHT SECTION OF TEST BUTTONS (3 LEFTMOST BUTTONS) */
  if (button_pressed(28)) // left button
  {
    Serial.println("Button 28");
    // example playing a sd file at current leaf string array index
    if (nav_state->name==strdup("custom_sounds")){
      // makes sure custom sound leaf node
      Serial.println("Custom Sounds");
      playFile(nav_state->ptr_str_array[nav_state->index]);
    }
    
    
    // Serial.printf("forward, %s\n",selection);
    lcd_display(lcd, nav_state->lcd_state);
  }
  if (button_pressed(29)) // middle button
  {
    
    // Serial.printf("forward, %s\n",selection);
    lcd_display(lcd, nav_state->lcd_state);
  }
  if (button_pressed(30)) // right button
  {
    
    // Serial.printf("forward, %s\n",selection);
    lcd_display(lcd, nav_state->lcd_state);
  }

  // delay(1000);
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

void midiSetInstrument(uint8_t chan, uint8_t inst) {
  if (chan > 15) return;
  inst --; // page 32 has instruments starting with 1 not 0 :(
  if (inst > 127) return;
  
  MIDI.write(PROGRAM | chan);  
  MIDI.write(inst);
}


void midiSetChannelVolume(uint8_t chan, uint8_t vol) {
  if (chan > 15) return;
  if (vol > 127) return;
  
  MIDI.write(MESSAGE | chan);
  MIDI.write(VOLUME);
  MIDI.write(vol);
}

void midiSetChannelBank(uint8_t chan, uint8_t bank) {
  if (chan > 15) return;
  if (bank > 127) return;
  
  MIDI.write(MESSAGE | chan);
  MIDI.write((uint8_t)BANK);
  MIDI.write(bank);
  Serial.println("Bank change");
  Serial.println(bank);
}

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  MIDI.write(NoteOn | chan);
  MIDI.write(n);
  MIDI.write(vel);
}

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  MIDI.write(NoteOff | chan);
  MIDI.write(n);
  MIDI.write(vel);
}
