/*
 *
 *       example function library
 *
 *
 */
#ifndef LED_MODULE_H
#define LED_MODULE_H

// functions, extern variables, structs go here

const int LED_column8 = 28;
const int selectPin1_LED = 27; // select bit A
const int selectPin2_LED = 26; // select bit B
const int selectPin3_LED = 25; // select bit C
const int LED_rowcount = 4;
const int LED_colcount = 9;
int MeMat_LEDindex[][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}};

int Palette_LEDMatrix[][2] = {{0, 6}, {0, 7}, {0, 8}, {1, 6}, {1, 7}, {1, 8}, {2, 6}, {2, 7}, {2, 8}, {3, 6}, {3, 7}, {3, 8}};

void led_init(void);
void LED_On(int Row, int Column);
void LED_Off(int Row, int Column);
void selectColumn_LED(int Column);
void selectRow_LED(int Row);
#endif // EXAMPLE_MODULE_H