/*
 *	LCD function library
 *
 *
 *
 */
#include "lcd-module.h"
// #include <LiquidCrystal.h>

LiquidCrystal *lcd_init(const struct lcd_pin_config &cfg)
{
  // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
  // LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
  LiquidCrystal *lcd = new LiquidCrystal(cfg.rs, cfg.en, cfg.dig4, cfg.dig5, cfg.dig6, cfg.dig7);
  // set up the LCD's number of columns and rows:
  // lcd.begin(cfg.columns, cfg.rows);
  lcd->begin(cfg.columns, cfg.rows);
  // Clears the LCD scree
  lcd->clear();

  return lcd;
}

void lcd_display(LiquidCrystal *lcd, char **print_arr)
{
  lcd->print(print_arr[0]); // print to row 0
  lcd->setCursor(0, 1);     // set cursor to row 1
  lcd->print(print_arr[1]); // print to row 1
  lcd->home();
  
}

char **array_scroll(const struct array_with_size &full_array, size_t index, size_t direction){
  char** result = new char*[2];
  result[0] = full_array.arr[index];
  result[1] = full_array.arr[index + 1];
  index=index+direction;
  if ((index>=0)&&(index<full_array.size+1)){
    result[0] = full_array.arr[index];
    result[1] = full_array.arr[index + 1];
    
  }
  return result;
  

}



void dpad_init(void)
{
}

int read_dpad(void)
{
  return 0;
}
