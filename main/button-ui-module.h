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

struct button_maxtrix_pin_config
{
    size_t width;   // The length of the array
    size_t length;  // The length of the array
    int rows[5];    // Flexible array member
    int columns[7]; // Flexible array member
};

struct palette_cell
{
    char *sound;
    int available;
};

struct palette_matrix
{
    struct palette_cell ***cells;
    int rows;
    int columns;
};

const int selectPin1_button = 10; // select bit A
const int selectPin2_button = 11; // select bit B
const int selectPin3_button = 12; // select bit C
const int enablePin_columns = 24; // enable bit

const int selectPin1_LED = 27; // select bit A
const int selectPin2_LED = 26; // select bit B
const int selectPin3_LED = 25; // select bit C

const int selectPin1_rows = 32; // select bit A
const int selectPin2_rows = 31; // select bit B
const int selectPin3_rows = 30; // select bit C
const int enablePin_rows = 29;

const int Button_column8 = 8;
const int LED_column8 = 28;

byte rows[] = {33, 34, 35, 36};

const int rowCount = 4;
const int colCount = 9;
const int LED_rowcount = 4;
const int LED_colcount = 9;

int Previous_Button_State[] = {9, 9};
int Current_Button_State[] = {9, 9};
int Last_Pushed_State[] = {9, 9};
int Pressed = 0;

unsigned long previousMillis = 0;
unsigned long interval = 50;
int loop_counter = 0;

int MeMat_LEDindex[][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}};

int Palette_LEDMatrix[][2] = {{0, 6}, {0, 7}, {0, 8}, {1, 6}, {1, 7}, {1, 8}, {2, 6}, {2, 7}, {2, 8}, {3, 6}, {3, 7}, {3, 8}};

int Current_Row = 0;
int Current_Column = 0;

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
void LED_On(int Row, int Column);
void LED_Off(int Row, int Column);
void selectColumn_button(int Column);
void selectColumn_LED(int Column);
void selectRow_LED(int Row);

// void set_matrix_led(struct matrix_coordinate, int state);

#endif // EXAMPLE_MODULE_H