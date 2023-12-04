/*
 *	example function library
 *
 *
 *
 */
#include "button-ui-module.h"
// const struct dpad_pin_config dpad_cfg;

void dpad_init(const struct dpad_pin_config &cfg)
{
    // dpad_cfg=cfg
    pinMode(cfg.left, INPUT);
    pinMode(cfg.down, INPUT);
    pinMode(cfg.up, INPUT);
    pinMode(cfg.right, INPUT);
    pinMode(cfg.select, INPUT);
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

void palette_assign(struct palette_matrix *palette, char *sound)
{
    // ACTUAL

    // TEMPORARY
    int cell_assigned = 0;

    while ((!cell_assigned))
    {
        if (button_pressed(24))
        {
            break;
        }

        for (int i = 0; i < 3; i++)
        {
            if (button_pressed(i + 28))
            {
                Serial.println(i + 28);
                (palette->cells[0][i])->sound = sound;
                (palette->cells[0][i])->available = 0;
                cell_assigned = 1;
            }
        }
    }
}
void measure_matrix_init(struct button_maxtrix_pin_config *button_cfg, struct button_maxtrix_pin_config *led_cfg)
{
    //  size_t width;  // The length of the array
    // size_t length; // The length of the array
    // int *rows;     // Flexible array member
    // int *columns;  // Flexible array member
    // init buttons
    for (size_t i = 0; i < button_cfg->length; i++)
    {
        pinMode(button_cfg->rows[i], INPUT_PULLUP);
    }
    for (size_t i = 0; i < button_cfg->width; i++)
    {
        pinMode(button_cfg->columns[i], INPUT_PULLUP);
    }
    

    // init LED
     for (size_t i = 0; i < led_cfg->length; i++)
    {
        pinMode(led_cfg->rows[i], OUTPUT);
        digitalWrite(led_cfg->rows[i], HIGH);
    }
    for (size_t i = 0; i < led_cfg->width; i++)
    {
        pinMode(led_cfg->columns[i], OUTPUT);
        digitalWrite(led_cfg->columns[i], LOW);
    }
   
}

void readMatrix(struct button_maxtrix_pin_config *button_cfg, struct button_maxtrix_pin_config *led_cfg)
{
    for (size_t column = 0; column<button_cfg->width;column++)
    {
        // col: set to output to low
        // byte curCol = cols[colIndex];
        // pinMode(curCol, OUTPUT);
        // digitalWrite(curCol, LOW);
        pinMode(button_cfg->columns[column],OUTPUT);
      digitalWrite(button_cfg->columns[column], LOW);
        


        // row: interate through the rows
        for (size_t row= 0; row<button_cfg->length;row++)
        {
            // byte rowCol = rows[rowIndex];
            if (digitalRead(button_cfg->rows[row]) == 0)
            {
                Serial.println("Push");
                Serial.print(row);
                Serial.print(column);
                digitalWrite(led_cfg->columns[column], HIGH);
                digitalWrite(led_cfg->rows[row], LOW);
            }

            else
            {
                digitalWrite(led_cfg->columns[column], LOW);
                digitalWrite(led_cfg->rows[row], HIGH);
            }
        }

        // disable the column
        // delay(1000);
        pinMode(button_cfg->columns[column],INPUT_PULLUP);
    }
}
// void set_matrix_led(struct button_maxtrix_pin_config *led_cfg,struct matrix_coordinate coordinate, int state){

//   if (state){
//     digitalWrite(led_cfg->columns[coordinate.column], LOW);
//                 digitalWrite(led_cfg->rows[coordinate.row], LOW);
//   }
//   else{
//     digitalWrite(led_cfg->columns[coordinate.column], LOW);
//                 digitalWrite(led_cfg->rows[coordinate.row], HIGH);
//   }



// }

// void readMatrix() {
// 	// iterate the columns
// 	for (int colIndex=0; colIndex < colCount; colIndex++) {
// 		// col: set to output to low
// 		byte curCol = cols[colIndex];
// 		pinMode(curCol, OUTPUT);
// 		digitalWrite(curCol, LOW);

// 		// row: interate through the rows
// 		for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
// 			byte rowCol = rows[rowIndex];
// 			//pinMode(rowCol, INPUT_PULLUP);
// 			//keys[colIndex][rowIndex] = digitalRead(rowCol);
// 		//	pinMode(rowCol, INPUT);
//       if (digitalRead(rows[rowIndex])==0){
//         Serial.println("Push");
//         Serial.print(rowIndex);
//         Serial.print(colIndex);
//         digitalWrite(LED_cols[colIndex], LOW);
//         digitalWrite(LED_rows[rowIndex], LOW);

//       }

//       else{
//         digitalWrite(LED_cols[colIndex], LOW);
//         digitalWrite(LED_rows[rowIndex], LOW);

//       }

// 		}

// 		// disable the column
//     //delay(1000);
// 		pinMode(curCol, INPUT_PULLUP);

// 	}
// }