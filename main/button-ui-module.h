/*
 *
 *       example function library
 *
 *
 */
#ifndef BUTTON_UI__MODULE_H
#define BUTTON_UI_MODULE_H

// functions, extern variables, structs go here
void dpad_init(const struct dpad_pin_config &cfg);
void dpad_read(const struct dpad_pin_config &cfg);

int check_ninput(const struct dpad_pin_config &cfg);

int button_pressed(int pin);

// void palette_init(const struct button_maxtrix_pin_config *cfg);
// void measure_matrix_init(const struct button_maxtrix_pin_config *cfg);
void button_matrix_init(struct button_maxtrix_pin_config *cfg);
void palette_assign(struct palette_matrix *palette, char *sound);

#endif // EXAMPLE_MODULE_H