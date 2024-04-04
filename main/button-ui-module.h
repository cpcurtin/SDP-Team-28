/*
 *
 *       example function library
 *
 *
 */
#ifndef BUTTON_UI_MODULE_H
#define BUTTON_UI_MODULE_H

#define TEMPO_KNOB_MIN 20
#define TEMPO_KNOB_MAX 150
#define SMOOTHING_SAMPLES 100
#define DPAD_LEFT 1
#define DPAD_RIGHT 2
#define DPAD_DOWN 3
#define DPAD_UP 4

#define MATRIX_BUTTON_DEBOUNCE 50

#define BUTTON_SELECT_PIN_1 12 // select bit A
#define BUTTON_SELECT_PIN_2 11 // select bit B
#define BUTTON_SELECT_PIN_3 10 // select bit C
#define ENABLE_PIN_COLUMNS 24  // enable bit

#define SELECT_ROW_PIN_1 32 // select bit A
#define SELECT_ROW_PIN_2 31 // select bit B
#define SELECT_ROW_PIN_3 30 // select bit C
#define ENABLE_PIN_ROWS 29

#define BUTTON_COLUMN_8 8

#define BUTTON_HELD 1
#define BUTTON_NOT_HELD 0
#define EFFECT_PALETTE_ROW 3
#define PALETTE_SOUND 0
#define PALETTE_EFFECT 1

#define BUTTON_MEASURE 0
#define BUTTON_PALETTE 1
#define BUTTON_SOUND 2
#define BUTTON_EFFECT 3

struct button_maxtrix_pin_config
{
    size_t width;   // The length of the array
    size_t length;  // The length of the array
    int rows[5];    // Flexible array member
    int columns[7]; // Flexible array member
};

typedef struct Button
{
    int row;
    int column;
    bool valid;
    bool waiting;
    unsigned long debounce_interval;
    unsigned long current_interval;

} Button;

byte rows[] = {33, 34, 35, 36};

int Previous_Button_State[] = {9, 9};
int Current_Button_State[] = {9, 9};
int Last_Pushed_State[] = {9, 9}; // row column
int Pressed = 0;

unsigned long previousMillis = 0;
unsigned long interval = 50;
int loop_counter = 0;

int Current_Row = 0;
int Current_Column = 0;

int dpad_button_pressed = 0;
int dpad_press_return = 0;
int dpad_pressed = 0;
unsigned long dpad_last_pressed;

Button matrix_button = {
    9,
    9,
    false,
    false,
    MATRIX_BUTTON_DEBOUNCE,
    0}; // matrix button info

int dpad_init(void);

int dpad_read(void);
int button_pressed(int pin);

int read_tempo(void);

// X
int button_matrix_init(void);
void readMatrix();
void check_rows(int colIndex);
void Button_Pressed(int Current_State[], int Previous_State[]);
void Button_Released(int Current_State[], int Previous_State[]);

void selectColumn_button(int Column);

// TESTING ZONE
// TESTING ZONE
// TESTING ZONE
bool matrix_pressed(int type, int held);

#endif // EXAMPLE_MODULE_H