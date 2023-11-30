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

char **array_scroll(struct array_with_size full_array, size_t direction){
// char** result = new char*[2];
// if (full_array.size>0){
//   full_array.state=full_array.state+1;
//   result[0]=full_array.arr[full_array.state];

// return full_array.lcd_state;
// }
  
  // Serial.println("1");
  // char** result = new char*[2];
  // Serial.println("2");
  // Serial.printf("%d %d",index, full_array.size);
  // Serial.println("XXX");
  // result[0] = full_array.arr[full_array.state];
  // Serial.println("3");
  // result[1] = full_array.arr[full_array.state + 1];
  // Serial.println("4");
  // index=index+direction;
  // Serial.println("5");
  // if ((index>=0)&&(index<full_array.size+1)){
  //   Serial.println("6");
  //   result[0] = full_array.arr[index];
  //   Serial.println("7");
  //   result[1] = full_array.arr[index + 1];
  //   Serial.println("8");
  // }
  // Serial.println("9");
  return full_array.lcd_state;
  

}



