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
 *    PIN28:
 *    PIN29:
 *    PIN30:
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

LiquidCrystal *lcd;

char **lcd_state = new char *[2];
int lcd_index = 0;
struct array_with_size *sounds;

const struct lcd_pin_config lcd_cfg = {LCD_RS, LCD_EN, LCD_DIGITAL_4, LCD_DIGITAL_5, LCD_DIGITAL_6, LCD_DIGITAL_7, LCD_ROWS, LCD_COLUMNS};
const struct dac_pin_config dac_cfg = {DAC_DIN, DAC_WS, DAC_BCK};
const struct dpad_pin_config dpad_cfg = {BUTTON_DPAD_LEFT, BUTTON_DPAD_DOWN, BUTTON_DPAD_UP, BUTTON_DPAD_RIGHT};

void setup()
{

  /* Intialize hardware */
  serial_init();
  sd_init();
  dpad_init(dpad_cfg);
  onboard_dac_init();

  lcd = lcd_init(lcd_cfg);

  sounds = parsefiles();
  Serial.println(sounds->size);
  for (size_t i = 0; i < sounds->size; i++)
  {
    Serial.println(sounds->arr[i]);
  }

  Serial.println("PROGRAM LOOP BEGINS");
  array_scroll(sounds, 0);
  lcd_display(lcd, sounds->lcd_state);
  // lcd->cursor();
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
    playFile(sounds->arr[sounds->index]);
  }
  if (button_pressed(BUTTON_DPAD_DOWN))
  {
    array_scroll(sounds, 1);
     Serial.printf("%d %s\n",sounds->index,sounds->arr[sounds->index]);
  lcd_display(lcd, sounds->lcd_state);
  }
  if (button_pressed(BUTTON_DPAD_UP))
  {
    array_scroll(sounds, -1);
     Serial.printf("%d %s\n",sounds->index,sounds->arr[sounds->index]);
  lcd_display(lcd, sounds->lcd_state);
  }
  if (button_pressed(BUTTON_DPAD_RIGHT))
  {
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
  Serial.println("\n\n\n\n\nSerial Initialized");
}
