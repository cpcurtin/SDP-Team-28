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
  // welcome message
  char **lcd_init_message = new char *[2];
  lcd_init_message[0] = strdup("   Welcome to   ");
  lcd_init_message[1] = strdup("    Moduloop    ");

  // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
  LiquidCrystal *lcd = new LiquidCrystal(cfg.rs, cfg.en, cfg.dig4, cfg.dig5, cfg.dig6, cfg.dig7);
  lcd_rows = cfg.rows;
  lcd->begin(cfg.columns, cfg.rows);
  lcd->clear();
  lcd_display(lcd, lcd_init_message);

  return lcd;
}

void lcd_display(LiquidCrystal *lcd, char **print_arr)
{
  lcd->clear();
  for (int row = 0; row < lcd_rows; row++)
  {
    lcd->setCursor(0, row); // set cursor to row 0
    Serial.printf("expected row %d, print: %s\n", row, print_arr[row]);
    lcd->print(print_arr[row]); // print to row 0
  }
  lcd->home();
}

void array_scroll(struct array_with_size *full_array, int direction)
{
  // Serial.println(enumerate);

  int new_index;
  if (full_array->index + direction < 0)
  {
    new_index = (int)full_array->size - 1;
  }
  else
  {
    new_index = (full_array->index + direction) % full_array->size;
  }

  full_array->index = new_index;
  char *temp_str = (char *)malloc(strlen(full_array->arr[new_index]) + 3);
  char *temp_str2 = (char *)malloc(strlen(full_array->arr[new_index]));
  strcpy(temp_str, ">");
  strcpy(temp_str2, full_array->arr[new_index]);
  strcat(temp_str, temp_str2);
  full_array->lcd_state[0] = strdup(temp_str);
  Serial.printf(">>>>>>>>>>>TEST: %s\n", full_array->lcd_state[0]);
  free(temp_str);
  free(temp_str2);
  Serial.printf(">>>>>>>>>>>TEST2: %s\n", full_array->lcd_state[0]);
  for (int row = 1; row < lcd_rows; row++)
  {
    int temp_index = (new_index + row) % full_array->size;
    full_array->lcd_state[row] = (full_array->arr[temp_index]);
  }
}
