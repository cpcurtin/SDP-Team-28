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

        if (digitalRead(cfg.left) == HIGH)
        {
            pressedCount++;
        }
        if (digitalRead(cfg.down) == HIGH)
        {
            pressedCount++;
        }
        if (digitalRead(cfg.up) == HIGH)
        {
            pressedCount++;
        }
        if (digitalRead(cfg.right) == HIGH)
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
              Serial.println(i+28);
                (palette->cells[0][i])->sound = sound;
                (palette->cells[0][i])->available = 0;
                cell_assigned = 1;
            }
        }
    }
}
void measure_matrix_led_init(struct button_maxtrix_pin_config *button_cfg,struct button_maxtrix_pin_config *led_cfg){
  
}