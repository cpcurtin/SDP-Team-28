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
 *    PIN24:
 *    PIN25:
 *    PIN26:
 *    PIN27:
 *    PIN28:
 *    PIN29:
 *    PIN30:
 *    PIN1:
 *    PIN2:
 *    PIN3:
 *    PIN4:
 *    PIN5:
 *    PIN6:
 *    PIN7:
 *    PIN8:
 *    PIN9:
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



LiquidCrystal *lcd;

char** lcd_state = new char*[2];


// LiquidCrystal lcd;
// LiquidCrystal lcd(2, 3, 4, 5, 6, 8);

// LiquidCrystal lcd;
// #include <SD.h>
void setup()
{
  const struct lcd_pin_config lcd_cfg = {LCD_RS, LCD_EN, LCD_DIGITAL_4, LCD_DIGITAL_5, LCD_DIGITAL_6, LCD_DIGITAL_7, LCD_ROWS, LCD_COLUMNS};
  const struct dac_pin_config dac_cfg = {DAC_DIN, DAC_WS, DAC_BCK};

  serial_init();
  sd_init();
  lcd = lcd_init(lcd_cfg);
  
  

  const struct array_with_size test = parsefiles();
  Serial.println(test.size);
  for (size_t i=0; i <test.size; i++ ){
    Serial.println(test.arr[i]);
  }

  
  lcd_state = array_scroll(test, 0,1);
  lcd_display(lcd, lcd_state);

  // char**test=parsefiles();
}

void loop()
{

// Serial.println("testpls");
  
  // Print a message to the LCD.
  // lcd.print(" Hello world!");

  // // set the cursor to column 0, line 1
  // // (note: line 1 is the second row, since counting begins with 0):
  // lcd.setCursor(0, 1);
  // // Print a message to the LCD.
  // lcd.print(" LCD Tutorial");
}

void serial_init(void)
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect.
  }
  Serial.println("Serial Initialized");
}
