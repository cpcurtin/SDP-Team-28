/*
 *	example function library
 *
 *
 *
 */
#include "button-ui-module.h"
// const struct dpad_pin_config dpad_cfg;

int dpad_init(const struct dpad_pin_config &cfg)
{
  // dpad_cfg=cfg
  pinMode(cfg.left, INPUT);
  pinMode(cfg.down, INPUT);
  pinMode(cfg.up, INPUT);
  pinMode(cfg.right, INPUT);
  pinMode(cfg.select, INPUT);
  return 0;
}
int button_matrix_init(void)
{
  digitalWrite(enablePin_columns, LOW);
  digitalWrite(enablePin_rows, LOW);

  for (int x = 0; x < rowCount; x++)
  {
    pinMode(rows[x], INPUT_PULLUP);
    Serial.println(rows[x]);
  }

  pinMode(Button_column8, INPUT_PULLUP);

  pinMode(LED_column8, OUTPUT);
  digitalWrite(LED_column8, LOW);

  pinMode(selectPin1_button, OUTPUT);
  pinMode(selectPin2_button, OUTPUT);
  pinMode(selectPin3_button, OUTPUT);
  pinMode(enablePin_columns, OUTPUT);
  digitalWrite(enablePin_columns, HIGH);

  pinMode(selectPin1_LED, OUTPUT);
  pinMode(selectPin2_LED, OUTPUT);
  pinMode(selectPin3_LED, OUTPUT);

  pinMode(selectPin1_rows, OUTPUT);
  pinMode(selectPin2_rows, OUTPUT);
  pinMode(selectPin3_rows, OUTPUT);
  pinMode(enablePin_rows, OUTPUT);
  digitalWrite(enablePin_rows, HIGH);

  selectRow_LED(7);

  return 0;
}
void dpad_read(const struct dpad_pin_config &cfg)
{
}

int check_ninput(const struct dpad_pin_config &cfg)
{
  {
    int pressedCount = 0;

    if (digitalRead(cfg.left) == LOW)
    {
      pressedCount++;
    }
    if (digitalRead(cfg.down) == LOW)
    {
      pressedCount++;
    }
    if (digitalRead(cfg.up) == LOW)
    {
      pressedCount++;
    }
    if (digitalRead(cfg.right) == LOW)
    {
      pressedCount++;
    }

    return pressedCount == 1;
  }
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

// void palette_assign(struct palette_matrix *palette, const char *sound)
// {
//     // ACTUAL

//     // TEMPORARY
//     int cell_assigned = 0;

//     while ((!cell_assigned))
//     {
//         if (button_pressed(24))
//         {
//             break;
//         }

//         for (int i = 0; i < 3; i++)
//         {
//             if (button_pressed(i + 28))
//             {
//                 Serial.println(i + 28);
//                 (palette->cells[0][i])->sound = sound;
//                 (palette->cells[0][i])->available = 0;
//                 cell_assigned = 1;
//             }
//         }
//     }
// }
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
  for (int colIndex = 0; colIndex < colCount; colIndex++)
  {

    if (colIndex == 8)
    {

      digitalWrite(enablePin_columns, LOW);

      pinMode(Button_column8, OUTPUT);
      digitalWrite(Button_column8, LOW);
      check_rows(colIndex);

      pinMode(Button_column8, INPUT_PULLUP);
      digitalWrite(enablePin_columns, HIGH);
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
  for (int rowIndex = 0; rowIndex < rowCount; rowIndex++)
  {
    // byte rowCol = rows[rowIndex];

    if (digitalRead(rows[rowIndex]) == 0)
    {

      Current_Button_State[0] = rowIndex;
      Current_Button_State[1] = colIndex;
      Pressed = 1;
      Serial.print("Push");
      Serial.print(rowIndex);
      Serial.println(colIndex);
    }
  }
}

void Button_Pressed(int Current_State[], int Previous_State[])
{
  previousMillis = millis();
  LED_On(Current_State[0], Current_State[1]);

  if (loop_counter == 3)
  {

    if (Current_State[1] == 8)
    {

      pinMode(Button_column8, OUTPUT);
      digitalWrite(Button_column8, LOW);

      if (digitalRead(rows[Current_State[0]]) == 1)
      {

        Pressed = 0;
        Button_Released(Current_State, Previous_State);
      }

      pinMode(Button_column8, INPUT_PULLUP);
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
  LED_Off(Previous_State[0], Previous_State[1]);
  Current_State[0] = 9;
  Current_State[1] = 9;
}

void LED_On(int Row, int Column)
{

  if (Column == 8)
  {

    digitalWrite(enablePin_columns, LOW);
    digitalWrite(LED_column8, HIGH);
    selectRow_LED(Row);
  }

  else
  {
    digitalWrite(LED_column8, LOW);
    selectColumn_LED(Column);
    selectRow_LED(Row);
  }
}

void LED_Off(int Row, int Column)
{

  if (Column == 8)
  {

    digitalWrite(LED_column8, LOW);
    digitalWrite(enablePin_columns, HIGH);
  }

  selectRow_LED(7);
}

void selectColumn_button(int Column)
{

  digitalWrite(selectPin1_button, bitRead(Column, 0));
  digitalWrite(selectPin2_button, bitRead(Column, 1));
  digitalWrite(selectPin3_button, bitRead(Column, 2));
}

void selectColumn_LED(int Column)
{

  digitalWrite(selectPin1_LED, bitRead(Column, 0));
  digitalWrite(selectPin2_LED, bitRead(Column, 1));
  digitalWrite(selectPin3_LED, bitRead(Column, 2));
}

void selectRow_LED(int Row)
{

  digitalWrite(selectPin1_rows, bitRead(Row, 0));
  digitalWrite(selectPin2_rows, bitRead(Row, 1));
  digitalWrite(selectPin3_rows, bitRead(Row, 2));
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

  return TEMPO_KNOB_MAX - voltage;

  // Delay for readability (adjust as needed)
}