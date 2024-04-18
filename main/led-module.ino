/*
 *	example function library
 *
 *
 *
 */
#include "led-module.h"

void led_init(void)
{
}

void LED_On(int Row, int Column)
{

    if (Column == 8)
    {

        digitalWrite(ENABLE_PIN_LED, LOW);
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
        digitalWrite(ENABLE_PIN_LED, HIGH);
    }

    selectRow_LED(7);
}

void selectColumn_LED(int Column)
{

    digitalWrite(selectPin1_LED, bitRead(Column, 0));
    digitalWrite(selectPin2_LED, bitRead(Column, 1));
    digitalWrite(selectPin3_LED, bitRead(Column, 2));
}

void selectRow_LED(int Row)
{

    digitalWrite(SELECT_ROW_PIN_1, bitRead(Row, 0));
    digitalWrite(SELECT_ROW_PIN_2, bitRead(Row, 1));
    digitalWrite(SELECT_ROW_PIN_3, bitRead(Row, 2));
}

void LED_routine(int mode)
{
}