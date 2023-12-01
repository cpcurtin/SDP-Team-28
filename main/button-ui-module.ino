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
