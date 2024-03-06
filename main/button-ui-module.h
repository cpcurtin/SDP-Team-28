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

int dpad_init(const struct dpad_pin_config &cfg);
void dpad_read(const struct dpad_pin_config &cfg);

int check_ninput(const struct dpad_pin_config &cfg);

int button_pressed(int pin);

// void palette_init(const struct button_maxtrix_pin_config *cfg);
// void measure_matrix_init(const struct button_maxtrix_pin_config *cfg);
void button_matrix_init(struct button_maxtrix_pin_config *cfg);
// void measure_matrix_init(struct)
// void palette_assign(struct palette_matrix *palette, const char *sound);
void measure_matrix_init(struct button_maxtrix_pin_config *button_cfg, struct button_maxtrix_pin_config *led_cfg);
void readMatrix(struct button_maxtrix_pin_config *button_cfg, struct button_maxtrix_pin_config *led_cfg);
int read_tempo(void);

// void set_matrix_led(struct matrix_coordinate, int state);

#endif // EXAMPLE_MODULE_H