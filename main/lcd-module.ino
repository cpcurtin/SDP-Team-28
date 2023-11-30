/*
 *	LCD function library
 *
 *
 *
 */
#include "lcd-module.h"
// #include <LiquidCrystal.h>
int lcd_rows;

LiquidCrystal *lcd_init(const struct lcd_pin_config &cfg)
{
  // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
  // LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
  LiquidCrystal *lcd = new LiquidCrystal(cfg.rs, cfg.en, cfg.dig4, cfg.dig5, cfg.dig6, cfg.dig7);
  lcd_rows=cfg.rows;
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

void array_scroll(struct array_with_size *full_array, int direction){
    int new_index = full_array->index+direction;
    int lcd_row_adjusted_loop = full_array->size-lcd_rows;

    if ((0<= new_index)&&( new_index)<lcd_row_adjusted_loop){
      full_array->index=new_index;


      for(int row=0;row<lcd_rows;row++){
        Serial.println(full_array->arr[new_index+row]);
        
        full_array->lcd_state[row]=(full_array->arr[new_index+row]);
        Serial.println(full_array->lcd_state[row]);
      }
   
    }

}



