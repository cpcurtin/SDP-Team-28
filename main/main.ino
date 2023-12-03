/*
 *       MODULOOP main
 *
 *    PINOUT:
 *    PIN0:
 *    PIN1:
 *    PIN2:   LCD - Register Select
 *    PIN3:   LCD - Enable display
 *    PIN4:   LCD - Digital 4
 *    PIN5:   LCD - Digital 5
 *    PIN6:   LCD - Digital 6
 *    PIN7:   DAC - DIN
 *    PIN8:   LCD - Digital 7
 *    PIN9:
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
const struct dpad_pin_config dpad_cfg = {BUTTON_DPAD_LEFT, BUTTON_DPAD_DOWN, BUTTON_DPAD_UP, BUTTON_DPAD_RIGHT};

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
