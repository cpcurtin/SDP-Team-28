/*
 *
 *       example function library
 *
 *
 */
#ifndef BUTTON_UI_MODULE_H
#define BUTTON_UI_MODULE_H

#define TEMPO_KNOB 23
#define TEMPO_KNOB_MIN 20
#define TEMPO_KNOB_MAX 150
#define SMOOTHING_SAMPLES 100

struct dpad_pin_config
{
    const int up;
    const int down;
    const int left;
    const int right;
    const int select;
};
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

const int selectPin1_button = 10; //select bit A
const int selectPin2_button = 11; //select bit B
const int selectPin3_button = 12; //select bit C
const int enablePin_columns = 24; //enable bit

const int selectPin1_LED = 25; //select bit A
const int selectPin2_LED = 26; //select bit B
const int selectPin3_LED = 27; //select bit C

const int selectPin1_rows = 32; //select bit A
const int selectPin2_rows = 31; //select bit B
const int selectPin3_rows = 30; //select bit C
const int enablePin_rows = 29;

const int Button_column8 = 8;
const int LED_column8 = 28;

byte rows[] = {33,34,35,36};

const int rowCount = 4;
const int colCount = 9;
const int LED_rowcount = 4;
const int LED_colcount = 9;


int Previous_Button_State[] = {9,9};
int Current_Button_State[] = {9,9}; 
int Last_Pushed_State[] = {9,9};
int Pressed = 0;

unsigned long previousMillis = 0;
unsigned long interval = 50;
int loop_counter = 0;



int dpad_init(const struct dpad_pin_config &cfg);
void dpad_read(const struct dpad_pin_config &cfg);

int check_ninput(const struct dpad_pin_config &cfg);

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