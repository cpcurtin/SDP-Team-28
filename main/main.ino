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
#include "midi-sound-module.h"
#include <Metro.h>

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
#define LCD_I2C 0x27

// BUTTONS
#define BUTTON_DPAD_UP 26
#define BUTTON_DPAD_DOWN 25
#define BUTTON_DPAD_LEFT 24
#define BUTTON_DPAD_RIGHT 27

#define BUTTON_MATRIX_ROW_1 999
#define BUTTON_MATRIX_ROW_2 999
#define BUTTON_MATRIX_ROW_3 999
#define BUTTON_MATRIX_ROW_4 999

#define BUTTON_MATRIX_COLUMN_1 28
#define BUTTON_MATRIX_COLUMN_2 29
#define BUTTON_MATRIX_COLUMN_3 30
#define BUTTON_MATRIX_COLUMN_4 999
#define BUTTON_MATRIX_COLUMN_5 999
#define BUTTON_MATRIX_COLUMN_6 999
#define BUTTON_MATRIX_COLUMN_7 999
#define BUTTON_MATRIX_COLUMN_8 999
#define BUTTON_MATRIX_COLUMN_9 999

// MEASURE MATRIX BUTTONS
#define BUTTON_MEASURE_MATRIX_ROW_1 32
#define BUTTON_MEASURE_MATRIX_ROW_2 31
#define BUTTON_MEASURE_MATRIX_ROW_3 34
#define BUTTON_MEASURE_MATRIX_ROW_4 33

#define BUTTON_MEASURE_MATRIX_COLUMN_1 23
#define BUTTON_MEASURE_MATRIX_COLUMN_2 22
#define BUTTON_MEASURE_MATRIX_COLUMN_3 19
#define BUTTON_MEASURE_MATRIX_COLUMN_4 18
#define BUTTON_MEASURE_MATRIX_COLUMN_5 17
#define BUTTON_MEASURE_MATRIX_COLUMN_6 16

// MEASURE MATRIX LEDS

#define LED_MEASURE_MATRIX_ROW_1 38
#define LED_MEASURE_MATRIX_ROW_2 37
#define LED_MEASURE_MATRIX_ROW_3 36
#define LED_MEASURE_MATRIX_ROW_4 35

#define LED_MEASURE_MATRIX_COLUMN_1 15
#define LED_MEASURE_MATRIX_COLUMN_2 14
#define LED_MEASURE_MATRIX_COLUMN_3 13
#define LED_MEASURE_MATRIX_COLUMN_4 41
#define LED_MEASURE_MATRIX_COLUMN_5 40
#define LED_MEASURE_MATRIX_COLUMN_6 39

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
/**************************
Melodic Sounds
**************************/
#define AcoursticGrandPiano 1
#define BrightAcousticPiano 2
#define ElectricGrandPiano 3
#define HonkyTonkPiano 4
#define ElectricPiano1 5
#define ElectricPiano2 6
#define Harpsichord 7
#define Clavi 8
#define Celesta 9
#define Clockenspiel 10
#define MusicBox 11
#define Vibraphone 12
#define Marimba 13
#define Zylophone 14
#define TubularBells 15
#define Dulcimer 16
#define DrawbarOrgan 17
#define PercussiveOrgan 18
#define RockOrgan 19
#define ChurchOrgan 20
#define ReedOrgan 21
#define Accordion 22
#define Harmonica 23
#define TangoAccordian 24
#define AcousticGuitarNylon 25
#define AcoursticGuitarSteel 26
#define ElectricGuitarJazz 27
#define ElectricGuitarClean 28
#define ElectricGuitarMuted 29
#define OverdrivenGuitar 30
#define DistortionGuitar 31
#define GuitarHarmonics 32
#define AcousticBass 33
#define ElectricBassFinger 34
#define ElectricBassPick 35
#define FretlessBass 36
#define SlapBass1 37
#define SlapBass2 38
#define SynthBass1 39
#define SynthBass2 40
#define Violin 41
#define Viola 42
#define Cello 43
#define ContraBass 44
#define TremoloStrings 45
#define PizzicatoStrings 46
#define OrchestralHarp 47
#define Timpani 48
#define StringEnsemble1 49
#define StringEnsemble2 50
#define SythStrings1 51
#define SynthStrings2 52
#define ChoirAahs 53
#define Voice Oohs 54
#define SynthVoice 55
#define OrchestraHit 56
#define Trumpet 57
#define Trombone 58
#define Tuba 59
#define MuteTrumpet 60
#define FrenchHorn 61
#define BrassSelection 62
#define SymthBrass1 63
#define SynthBrass2 64
#define SopranoSax 65
#define AltoSax 66
#define TenorSax 67
#define BaritoneSax 68
#define Oboe 69
#define EnglishHorn 70
#define Bassoon 71
#define Clarinet 72
#define Piccolo 73
#define Flute 74
#define Recorder 75
#define PanFlute 76
#define BlownBottle 77
#define Shakuhachi 78
#define Whistle 79
#define Ocarina 80
#define SquareLead 81
#define SawLead 82
#define CalliopeLead 83
#define ChiffLead 84
#define CharangLead 85
#define VoiceLead 86
#define FifthsLead 87
#define BassLead 88
#define NewAge 89
#define WarmPad 90
#define Polysynth 91
#define Choir 92
#define Bowed 93
#define Metallic 94
#define Halo 95
#define Sweep 96
#define Rain 97
#define SoundTrack 98
#define Crystal 99
#define Atmosphere 100
#define Brightness 101
#define Goblins 102
#define Echoes 103
#define SciFi 104
#define Sitar 105
#define Banjo 106
#define Shamisen 107
#define Koto 108
#define Kalimba 109
#define BagPipe 110
#define Fiddle 111
#define Shanai 112
#define TimkleBell 113
#define Agogo 114
#define PitchedPercussion 115
#define WoodBlock 116
#define TaikoDrum 117
#define MelodicTom 118
#define SynthDrum 119
#define ReverseCymbal 120
#define GuitarFretNoise 121
#define BreathNoise 122
#define Seashore 123
#define BirdTweet 124
#define TelephoneRing 125
#define Helicopter 126
#define Applause 127
#define Gunshot 128

/**************************
Percussive Sounds
**************************/
#define HighQ 27
#define Slap 28
#define ScratchPush 29
#define ScratchPull 30
#define Sticks 31
#define SquareClick 32
#define MetClick 33
#define MetBell 34
#define AcousticBassDrum 35
#define BassDrum 36
#define SideStick 37
#define AcousticSnare 38
#define Clap 39
#define ElectricSnare 40
#define LowFloorTom 41
#define ClosedHiHat 42
#define HighFloorTom 43
#define PedalHiHat 44
#define LowTom 45
#define OpenHiHat 46
#define LowMidTom 47
#define HighMidTom 48
#define Crash1 49
#define HighTom 50
#define Ride1 51
#define China 52
#define RideBell 53
#define Tambourine 54
#define Splash 55
#define Cowbell 56
#define Crash2
#define Vibraslap 58
#define Ride2 59
#define HighBongo 60
#define LowBogo 61
#define MuteHighConga 62
#define HighConga 63
#define LowConga 64
#define HighTimbale 65
#define LowTimbale 66
#define HighAgogo 67
#define LowAgogo 68
#define Cabasa 69
#define Maracas 70
#define ShortWhistle 71
#define LongWhistle 72
#define ShortGuiro 73
#define LongGuiro 74
#define Claves 75
#define HighWoodBlock 76
#define LowWoodBlock 77
#define Mute Cuica 78
#define Cuica 79
#define MuteTriangle 80
#define Triangle 81
#define Shaker 82
#define JingleBell 83
#define BellTree 84
#define Castanets 85
#define MuteSurdo 86
#define Surdo 87

// DYNAMIC OPTIONS
#define LCD_ROWS 4
#define LCD_COLUMNS 20
#define MATRIX_ROWS 4
#define MATRIX_COLUMNS 9

#define MEASURE_MATRIX_ROWS 4
#define MEASURE_MATRIX_COLUMNS 6

#define PALETTE_MATRIX_ROWS 4
#define PALETTE_MATRIX_COLUMNS 3

// struct lcd_pin_config
// {
//   const int rs;
//   const int en;
//   const int dig4;
//   const int dig5;
//   const int dig6;
//   const int dig7;
//   const int rows;
//   const int columns;
// };
struct lcd_pin_config
{
  const int i2c;
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

struct button_maxtrix_pin_config
{
  size_t width;   // The length of the array
  size_t length;  // The length of the array
  int rows[5];    // Flexible array member
  int columns[7]; // Flexible array member
};

struct palette_cell
{
  char *sound;
  int available;
};

struct palette_matrix
{
  struct palette_cell ***cells;
  int rows;
  int columns;
};

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
  /* create hardware configuration objects */
  // struct button_maxtrix_pin_config *matrix_cfg = (struct button_maxtrix_pin_config *)malloc(sizeof(struct button_maxtrix_pin_config));
  // int matrix_rows[MATRIX_ROWS] = {BUTTON_MATRIX_ROW_1, BUTTON_MATRIX_ROW_2, BUTTON_MATRIX_ROW_3, BUTTON_MATRIX_ROW_4};
  // int matrix_columms[MATRIX_COLUMNS] = {BUTTON_MATRIX_COLUMN_1, BUTTON_MATRIX_COLUMN_2, BUTTON_MATRIX_COLUMN_3, BUTTON_MATRIX_COLUMN_4, BUTTON_MATRIX_COLUMN_5, BUTTON_MATRIX_COLUMN_6, BUTTON_MATRIX_COLUMN_7, BUTTON_MATRIX_COLUMN_8, BUTTON_MATRIX_COLUMN_9};
  // matrix_cfg->width = MATRIX_COLUMNS;
  // matrix_cfg->length = MATRIX_ROWS;
  // matrix_cfg->rows = matrix_rows;
  // matrix_cfg->columns = matrix_columms;

  //
  //
  // button & led measure matrix
  //
  //
   // measure_matrix_button = (struct button_maxtrix_pin_config *)malloc(sizeof(struct button_maxtrix_pin_config));
  int button_matrix_rows[MEASURE_MATRIX_ROWS+1] = {BUTTON_MEASURE_MATRIX_ROW_1, BUTTON_MEASURE_MATRIX_ROW_2, BUTTON_MEASURE_MATRIX_ROW_3, BUTTON_MEASURE_MATRIX_ROW_4};
  int button_matrix_columms[MEASURE_MATRIX_COLUMNS+1] = {BUTTON_MEASURE_MATRIX_COLUMN_1, BUTTON_MEASURE_MATRIX_COLUMN_2, BUTTON_MEASURE_MATRIX_COLUMN_3, BUTTON_MEASURE_MATRIX_COLUMN_4, BUTTON_MEASURE_MATRIX_COLUMN_5, BUTTON_MEASURE_MATRIX_COLUMN_6};
  measure_matrix_button.width = MEASURE_MATRIX_COLUMNS;
  measure_matrix_button.length = MEASURE_MATRIX_ROWS;

 for (size_t i = 0; i < MEASURE_MATRIX_ROWS; ++i) {
  measure_matrix_button.rows[i] = button_matrix_rows[i];
}
for (size_t i = 0; i < MEASURE_MATRIX_COLUMNS; ++i) {
  measure_matrix_button.columns[i] = button_matrix_columms[i];
}

  // measure_matrix_led = (struct button_maxtrix_pin_config *)malloc(sizeof(struct button_maxtrix_pin_config));
  int led_matrix_rows[MEASURE_MATRIX_ROWS+1] = {LED_MEASURE_MATRIX_ROW_1, LED_MEASURE_MATRIX_ROW_2, LED_MEASURE_MATRIX_ROW_3, LED_MEASURE_MATRIX_ROW_4};
  int led_matrix_columms[MEASURE_MATRIX_COLUMNS+1] = {LED_MEASURE_MATRIX_COLUMN_1, LED_MEASURE_MATRIX_COLUMN_2, LED_MEASURE_MATRIX_COLUMN_3, LED_MEASURE_MATRIX_COLUMN_4, LED_MEASURE_MATRIX_COLUMN_5, LED_MEASURE_MATRIX_COLUMN_6};
  measure_matrix_led.width = MEASURE_MATRIX_COLUMNS;
  measure_matrix_led.length = MEASURE_MATRIX_ROWS;
  for (size_t i = 0; i < MEASURE_MATRIX_ROWS; ++i) {
  measure_matrix_led.rows[i] = led_matrix_rows[i];
}
for (size_t i = 0; i < MEASURE_MATRIX_COLUMNS; ++i) {
  measure_matrix_led.columns[i] = led_matrix_columms[i];
}
//   //
//   //
//   //
//   //
//   //

  palette = (struct palette_matrix *)malloc(sizeof(struct palette_matrix));
  palette->cells = (struct palette_cell ***)malloc(PALETTE_MATRIX_ROWS * sizeof(struct palette_cell *));
  for (int m = 0; m < PALETTE_MATRIX_ROWS; m++)
  {
    palette->cells[m] = (struct palette_cell **)malloc(PALETTE_MATRIX_COLUMNS * sizeof(struct palette_cell));
    for (int n = 0; n < PALETTE_MATRIX_COLUMNS; n++)
    {
      palette->cells[m][n] = (struct palette_cell *)malloc(sizeof(struct palette_cell));
      (palette->cells[m][n])->sound = NULL;
      (palette->cells[m][n])->available = 1;
    }
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
  Serial.println("parsed files size");
  Serial.println((nav_cfg->sounds_custom)->size);
  playFile((nav_cfg->sounds_custom)->array[1]);

  lcd = lcd_init(lcd_cfg);
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

  /* play music config no matter what */

  /* on input */
  // check input - beat matrix, palette, dpad
  //  if(check_ninput(dpad_cfg)){

  //   }
  // Main Timing Loop for 4x4 Measure Matrix
  // if (ledMetro.check() == 1)
  // {
  //   if (count == 0)
  //   {
  //     Serial.println("Hit");
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_4, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
  //     midiNoteOn(0, Crash1, 127);
  //     midiNoteOn(0, AcousticBassDrum, 127);
  //     playFile((nav_cfg->sounds_custom)->array[6]);
  //   }

  //   if (count == 1)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
  //     midiNoteOn(0, ClosedHiHat, 127);
  //   }

  //   if (count == 2)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
  //     midiNoteOn(0, ClosedHiHat, 127);
  //     midiNoteOn(0, AcousticSnare, 127);
  //   }

  //   if (count == 3)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_1, LOW);
  //     midiNoteOn(0, ClosedHiHat, 127);
  //   }

  //   if (count == 4)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_1, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);

  //     midiNoteOn(0, ClosedHiHat, 127);
  //     midiNoteOn(0, AcousticBassDrum, 127);
  //   }

  //   if (count == 5)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);
  //     midiNoteOn(0, ClosedHiHat, 127);
  //   }

  //   if (count == 6)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);

  //     midiNoteOn(0, ClosedHiHat, 127);
  //     midiNoteOn(0, AcousticSnare, 127);
  //   }

  //   if (count == 7)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_2, LOW);
  //     midiNoteOn(0, ClosedHiHat, 127);
  //   }

  //   if (count == 8)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_2, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

  //     midiNoteOn(0, ClosedHiHat, 127);
  //     midiNoteOn(0, AcousticBassDrum, 127);
  //   }

  //   if (count == 9)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

  //     midiNoteOn(0, ClosedHiHat, 127);
  //     midiNoteOn(0, AcousticBassDrum, 127);
  //   }

  //   if (count == 10)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

  //     midiNoteOn(0, ClosedHiHat, 127);
  //     midiNoteOn(0, AcousticSnare, 127);
  //   }

  //   if (count == 11)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_3, LOW);

  //     midiNoteOn(0, ClosedHiHat, 127);
  //     midiNoteOn(0, AcousticBassDrum, 127);
  //   }

  //   if (count == 12)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_3, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);

  //     midiNoteOn(0, ClosedHiHat, 127);
  //     midiNoteOn(0, AcousticBassDrum, 127);
  //   }

  //   if (count == 13)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_1, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);
  //     midiNoteOn(0, ClosedHiHat, 127);
  //   }

  //   if (count == 14)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_2, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);

  //     midiNoteOn(0, ClosedHiHat, 127);
  //     midiNoteOn(0, AcousticSnare, 127);
  //   }

  //   if (count == 15)
  //   {
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_3, LOW);
  //     digitalWrite(LED_MEASURE_MATRIX_COLUMN_4, HIGH);
  //     digitalWrite(LED_MEASURE_MATRIX_ROW_4, LOW);

  //     midiNoteOn(0, ClosedHiHat, 127);
  //   }

  //   count++;
  //   if (count == 16)
  //   {
  //     count = 0;
  //   }
  // }

  if (button_pressed(BUTTON_DPAD_LEFT)) // return / exit
  {
    nav_state = nav_selection(nav_state, -1);

    lcd_display(lcd, nav_state->lcd_state);
  }
  if (button_pressed(BUTTON_DPAD_DOWN)) // scroll down
  {
    array_scroll(nav_state, 1);
    lcd_display(lcd, nav_state->lcd_state);
  }
  if (button_pressed(BUTTON_DPAD_UP)) // scroll up
  {
    array_scroll(nav_state, -1);
    lcd_display(lcd, nav_state->lcd_state);
  }
  if (button_pressed(BUTTON_DPAD_RIGHT)) // select
  {
    if (strcmp(nav_state->name, "custom_sounds") == 0)
    {
      Serial.println("made it here");
      palette_assign(palette, nav_state->ptr_str_array[nav_state->index]);
      for (int m = 0; m < PALETTE_MATRIX_ROWS; m++)
      {

        for (int n = 0; n < PALETTE_MATRIX_COLUMNS; n++)
        {
          Serial.println((palette->cells[m][n])->sound);
        }
      }
    }
    else
    {
      nav_state = nav_selection(nav_state, 1);
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
