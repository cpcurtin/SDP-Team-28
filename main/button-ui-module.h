/*
 *
 *       example function library
 *
 *
 */
#ifndef EXAMPLE_MODULE_H
#define EXAMPLE_MODULE_H


// functions, extern variables, structs go here
void dpad_init(const struct dpad_pin_config &cfg);
void dpad_read(const struct dpad_pin_config &cfg);

int check_ninput(const struct dpad_pin_config &cfg);

int button_pressed(int pin);


#endif // EXAMPLE_MODULE_H