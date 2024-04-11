/*
 *	example function library
 *
 *
 *
 */
#include "button-ui-module.h"
// const struct dpad_pin_config dpad_cfg;
#if USING_CDR_PCB == 1 // CDR PCB uses external pulldown
int dpad_init()
{
  pinMode(BUTTON_DPAD_LEFT, INPUT);
  pinMode(BUTTON_DPAD_RIGHT, INPUT);
  pinMode(BUTTON_DPAD_DOWN, INPUT);
  pinMode(BUTTON_DPAD_UP, INPUT);

  return 0;
}
#endif

#if USING_CDR_PCB == 0 // New design uses internal pullup
int dpad_init()
{
  pinMode(BUTTON_DPAD_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_DPAD_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_DPAD_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_DPAD_UP, INPUT_PULLUP);

  return 0;
}
#endif
int button_matrix_init(void)
{
  digitalWrite(ENABLE_PIN_COLUMNS, LOW);
  digitalWrite(ENABLE_PIN_ROWS, LOW);

  for (int x = 0; x < MATRIX_ROWS; x++)
  {
    pinMode(rows[x], INPUT_PULLUP);
    Serial.println(rows[x]);
  }

  pinMode(BUTTON_COLUMN_8, INPUT_PULLUP);

  pinMode(LED_column8, OUTPUT);
  digitalWrite(LED_column8, LOW);

  pinMode(BUTTON_SELECT_PIN_1, OUTPUT);
  pinMode(BUTTON_SELECT_PIN_2, OUTPUT);
  pinMode(BUTTON_SELECT_PIN_3, OUTPUT);
  pinMode(ENABLE_PIN_COLUMNS, OUTPUT);
  digitalWrite(ENABLE_PIN_COLUMNS, HIGH);

  pinMode(selectPin1_LED, OUTPUT);
  pinMode(selectPin2_LED, OUTPUT);
  pinMode(selectPin3_LED, OUTPUT);

  pinMode(SELECT_ROW_PIN_1, OUTPUT);
  pinMode(SELECT_ROW_PIN_2, OUTPUT);
  pinMode(SELECT_ROW_PIN_3, OUTPUT);
  pinMode(ENABLE_PIN_ROWS, OUTPUT);
  digitalWrite(ENABLE_PIN_ROWS, HIGH);

  selectRow_LED(7);

  return 0;
}

int button_pressed(int pin)
{
  int buttonState = digitalRead(pin);
  if (buttonState == HIGH)
  {
    Serial.printf("pressed - %d\n", pin);
    while (buttonState == HIGH)
    {
      buttonState = digitalRead(pin);
    }
    Serial.println("depressed");
    return 1;
  }
  return 0;
}
// void palette_init(const struct button_maxtrix_pin_config *cfg)
void button_matrix_init(struct button_maxtrix_pin_config *cfg)

{
  pinMode(28, INPUT);
  pinMode(29, INPUT);
  pinMode(30, INPUT);
}

void measure_matrix_init(struct button_maxtrix_pin_config &button_cfg, struct button_maxtrix_pin_config &led_cfg)
{
  //  size_t width;  // The length of the array
  // size_t length; // The length of the array
  // int *rows;     // Flexible array member
  // int *columns;  // Flexible array member
  // init buttons

  for (size_t i = 0; i < button_cfg.width; i++)
  {
    pinMode(button_cfg.columns[i], INPUT_PULLUP);
  }
  for (size_t i = 0; i < button_cfg.length; i++)
  {
    pinMode(button_cfg.rows[i], INPUT_PULLUP);
  }

  // init LED
  for (size_t i = 0; i < led_cfg.width; i++)
  {
    pinMode(led_cfg.columns[i], OUTPUT);
    digitalWrite(led_cfg.columns[i], LOW);
  }
  for (size_t i = 0; i < led_cfg.length; i++)
  {
    pinMode(led_cfg.rows[i], OUTPUT);
    digitalWrite(led_cfg.rows[i], HIGH);
  }
}

void readMatrix()
{
  // iterate the columns
  for (int colIndex = 0; colIndex < MATRIX_COLUMNS; colIndex++)
  {

    if (colIndex == 8)
    {

      digitalWrite(ENABLE_PIN_COLUMNS, LOW);
      pinMode(BUTTON_COLUMN_8, OUTPUT);
      digitalWrite(BUTTON_COLUMN_8, LOW);
      check_rows(colIndex);

      pinMode(BUTTON_COLUMN_8, INPUT_PULLUP);
      digitalWrite(ENABLE_PIN_COLUMNS, HIGH);
    }

    else
    {

      selectColumn_button(colIndex);
      check_rows(colIndex);
    }
  }
}

void check_rows(int colIndex)
{
  // row: interate through the rows
  for (int rowIndex = 0; rowIndex < MATRIX_ROWS; rowIndex++)
  {
    // byte rowCol = rows[rowIndex];

    if (digitalRead(rows[rowIndex]) == 0)
    {

      Current_Button_State[0] = rowIndex;
      Current_Button_State[1] = colIndex;

      matrix_button.waiting = true;
      matrix_button.row = rowIndex;
      matrix_button.column = colIndex;
      Pressed = 1;

      // Serial.print("Push");
      // Serial.print(rowIndex);
      Serial.println(colIndex);
    }
  }
}

void Button_Pressed(int Current_State[], int Previous_State[])
{
  previousMillis = millis();
  // LED_On(Current_State[0], Current_State[1]);

  if (loop_counter == 3)
  {

    if (Current_State[1] == 8)
    {

      pinMode(BUTTON_COLUMN_8, OUTPUT);
      digitalWrite(BUTTON_COLUMN_8, LOW);

      if (digitalRead(rows[Current_State[0]]) == 1)
      {

        Pressed = 0;
        Button_Released(Current_State, Previous_State);
      }

      pinMode(BUTTON_COLUMN_8, INPUT_PULLUP);
    }
    else
    {

      selectColumn_button(Current_State[1]);

      if (digitalRead(rows[Current_State[0]]) == 1)
      {

        Pressed = 0;
        Button_Released(Current_State, Previous_State);
      }
    }
    loop_counter = 0;
  }

  loop_counter += 1;
}

void Button_Released(int Current_State[], int Previous_State[])
{

  previousMillis = millis();
  // LED_Off(Previous_State[0], Previous_State[1]);

  // NEW BUTTON TESTING ABSTRACTION

  matrix_button.current_interval = previousMillis;
  matrix_button.waiting = false;
  matrix_button.valid = true;

  Current_State[0] = 9;
  Current_State[1] = 9;
}

void selectColumn_button(int Column)
{

  digitalWrite(BUTTON_SELECT_PIN_1, bitRead(Column, 0));
  digitalWrite(BUTTON_SELECT_PIN_2, bitRead(Column, 1));
  digitalWrite(BUTTON_SELECT_PIN_3, bitRead(Column, 2));
}

int read_tempo(void)
{
  int raw_avg = 0;
  for (int i = 0; i < SMOOTHING_SAMPLES; i++)
  {
    raw_avg += analogRead(TEMPO_KNOB);
  }
  raw_avg = raw_avg / SMOOTHING_SAMPLES;

  int voltage = (int)raw_avg * ((TEMPO_KNOB_MAX - TEMPO_KNOB_MIN) / 1023.0); // Assuming 5V Arduino board

  return TEMPO_KNOB_MIN + voltage;

  // Delay for readability (adjust as needed)
}
#if USING_CDR_PCB == 1 // Button has external pulldown. When high, button pushed
int dpad_read(void)
{

  if (dpad_button_pressed)
  {
    if (millis() - dpad_last_pressed > 50 && digitalRead(dpad_button_pressed) == LOW)
    {
      dpad_press_return = dpad_button_pressed;
      dpad_button_pressed = 0;
      return dpad_press_return;
    }
  }
  else
  {
    if (digitalRead(BUTTON_DPAD_LEFT) == HIGH)
    {
      dpad_button_pressed = BUTTON_DPAD_LEFT;
    }
    if (digitalRead(BUTTON_DPAD_DOWN) == HIGH)
    {
      dpad_button_pressed = BUTTON_DPAD_DOWN;
    }
    if (digitalRead(BUTTON_DPAD_UP) == HIGH)
    {
      dpad_button_pressed = BUTTON_DPAD_UP;
    }
    if (digitalRead(BUTTON_DPAD_RIGHT) == HIGH)
    {
      dpad_button_pressed = BUTTON_DPAD_RIGHT;
    }
    dpad_last_pressed = millis();
  }

  /*
unsigned long start_time = millis();
unsigned long end_time = millis();
Calculate the difference in time unsigned long time_diff = end_time - start_time;
  */

  return 0;
}
#endif

#if USING_CDR_PCB == 0 // Button using input pullup. When low, button pushed
int dpad_read(void)
{

  if (dpad_button_pressed)
  {
    if (millis() - dpad_last_pressed > 50 && digitalRead(dpad_button_pressed) == LOW)
    {
      dpad_press_return = dpad_button_pressed;
      dpad_button_pressed = 0;
      return dpad_press_return;
    }
  }
  else
  {
    if (digitalRead(BUTTON_DPAD_LEFT) == LOW)
    {
      dpad_button_pressed = BUTTON_DPAD_LEFT;
    }
    if (digitalRead(BUTTON_DPAD_DOWN) == LOW)
    {
      dpad_button_pressed = BUTTON_DPAD_DOWN;
    }
    if (digitalRead(BUTTON_DPAD_UP) == LOW)
    {
      dpad_button_pressed = BUTTON_DPAD_UP;
    }
    if (digitalRead(BUTTON_DPAD_RIGHT) == LOW)
    {
      dpad_button_pressed = BUTTON_DPAD_RIGHT;
    }
    dpad_last_pressed = millis();
  }

  /*
unsigned long start_time = millis();
unsigned long end_time = millis();
Calculate the difference in time unsigned long time_diff = end_time - start_time;
  */

  return 0;
}
#endif
bool matrix_pressed(int type, int held)
{
  switch (type)
  {
  case BUTTON_MEASURE:
    if (matrix_button.column <= LAST_MEASURE_COLUMN)
    {
      if ((held == BUTTON_NOT_HELD) && matrix_button.valid)
      {
        matrix_button.valid = false;
        return true;
      }
      else if ((held == BUTTON_HELD) && matrix_button.waiting)
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    break;
  case BUTTON_PALETTE:
    if (matrix_button.column > LAST_MEASURE_COLUMN)
    {
      if ((held == BUTTON_NOT_HELD) && matrix_button.valid)
      {
        matrix_button.valid = false;
        return true;
      }
      else if ((held == BUTTON_HELD) && matrix_button.waiting)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    break;
  case BUTTON_SOUND:
    if ((matrix_button.column > LAST_MEASURE_COLUMN) && (matrix_button.row != EFFECT_PALETTE_ROW))
    {
      if ((held == BUTTON_NOT_HELD) && matrix_button.valid)
      {
        matrix_button.valid = false;
        return true;
      }
      else if ((held == BUTTON_HELD) && matrix_button.waiting)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    break;
  case BUTTON_EFFECT:
    if ((matrix_button.column > LAST_MEASURE_COLUMN) && (matrix_button.row == EFFECT_PALETTE_ROW))
    {
      if ((held == BUTTON_NOT_HELD) && matrix_button.valid)
      {
        matrix_button.valid = false;
        return true;
      }
      else if ((held == BUTTON_HELD) && matrix_button.waiting)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    break;
  default:
    return false;
    break;
  }
  return false;
}
