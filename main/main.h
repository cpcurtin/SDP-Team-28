#ifndef MAIN_H
#define MAIN_H
/*
 *       MODULOOP MAIN CONFIGURATIONS
 *
 *    KEY:
 *    R:  PWM
 *    O:  ANALOG
 *    LY: PROP
 *    Y:  AUDIO
 *    G:  SPI
 *    LB: FLEXIO
 *    B:  SERIAL
 *    I:  I2C
 *    P:  CAN
 **************************  HEADERS  **************************
 *    PINOUT  R   O   Y           G     B   I     P     | PINOUT
 *    PIN0:   PWM                 CS1   RX1       CRX2  | PIN0: empty(For MIDI) - RX
 *    PIN1:   PWM                 MISO1 TX1       CTX2  | PIN1: MIDI - TX
 *    PIN2:   PWM     OUT2                              | PIN2: DPAD
 *    PIN3:   PWM     LRCLK2                            | PIN3: DPAD
 *    PIN4:   PWM     BCLK2                             | PIN4: DPAD
 *    PIN5:   PWM     IN2                               | PIN5: DPAD
 *    PIN6:   PWM     OUT1D                             | PIN6:
 *    PIN7:   PWM     OUT1A             RX2             | PIN7: DAC - DIN
 *    PIN8:   PWM     IN1               TX2             | PIN8: DEC3
 *    PIN9:   PWM     OUT1C                             | PIN9: MIDI
 *    PIN10:  PWM     MQSR        CS                    | PIN10: DEC3
 *    PIN11:  PWM                 MOSI            CTX1  | PIN11: DEC3
 *    PIN12:  PWM     MQSL        MISO                  | PIN12: DEC3
 *    PIN13:  PWM                 SCK                   | PIN 13:
 *    PIN14:  PWM A0  S/PDIF OUT        TX3             | PIN14:
 *    PIN15:  PWM A1  S/PDIF IN         RX3             | PIN15:
 *    PIN16:      A2                    RX4 SCL1        | PIN16:
 *    PIN17:      A3                    TX4 SDA1        | PIN17:
 *    PIN18:  PWM A4                        SDA         | PIN18: LCD - SDA
 *    PIN19:  PWM A5                        SCL         | PIN19: LCD - SCL
 *    PIN20:      A6  LRCLK1            TX5             | PIN20: DAC - WS
 *    PIN21:      A7  BCLK1             RX5             | PIN21: DAC - BCK
 *    PIN22:  PWM A8                              CTX1  | PIN22:
 *    PIN23:  PWM A9  MCLK1                       CRX1  | PIN23:
 *    PIN24:  PWM A10                       SCL2        | PIN24: DEC1
 *    PIN25:  PWM A11                       SDA2        | PIN25: DEC1
 *    PIN26:      A12             MOSI1                 | PIN26: DEC1
 *    PIN27:      A13             SCK1                  | PIN27: DEC2
 *    PIN28:  PWM                       RX7             | PIN28: DEC2
 *    PIN29:  PWM                       TX7             | PIN29: DEC2
 *    PIN30:                                      CRX3  | PIN30: DEC1/2
 *    PIN31:                                      CTX3  | PIN31: Button9
 *    PIN32:          OUT1B                             | PIN32: LED9
 *    PIN33:  PWM     MCLK2                             | PIN33:
 *    PIN34:                            RX8             | PIN34:
 *    PIN35:                            TX8             | PIN35:
 *    PIN36:  PWM                 CS                    | PIN36:
 *    PIN37:  PWM                 CS                    | PIN37:
 *    PIN38:      A14 IN1         CS1                   | PIN38:
 *    PIN39:      A15 OUT1A       MISO1                 | PIN39:
 *    PIN40:      A16                                   | PIN40:
 *    PIN41:      A17                                   | PIN41:
 *    PINOUT  R   O   Y           G     B   I     P     | PINOUT
 *    *********************  BOTTOM PADS  **********************
 *    PINOUT  R   LY  G     LB    B   I                 | PINOUT
 *    PIN42:  PWM     MISO2 DAT1                        | PIN42:    SD READER - DAT1
 *    PIN43:  PWM     MOSI2 DAT0                        | PIN43:    SD READER - DAT0
 *    PIN44:  PWM     CS2   CLK       SDA1              | PIN44:    SD READER - CLK
 *    PIN45:  PWM     SCK2  CMD       SCL1              | PIN45:    SD READER - CMD
 *    PIN46:  PWM           DAT3  RX5                   | PIN46:    SD READER - DAT3
 *    PIN47:  PWM           DAT2  TX5                   | PIN47:    SD READER - DAT2
 *    PIN48:      CS0             RX8                   | PIN48:    FLASH MEM - CS0
 *    PIN49:      D1  SCK2                              | PIN49:    FLASH MEM - D1
 *    PIN50:      D2  MOSI2                             | PIN50:    FLASH MEM - D2
 *    PIN51:  PWM CS1                 SCL1              | PIN51:
 *    PIN52:      D0              RX1                   | PIN52:    FLASH MEM - D0
 *    PIN53:      CLK             TX1                   | PIN53:    FLASH MEM - CLK
 *    PIN54:  PWM D3  MISO2                             | PIN54:    FLASH MEM - D3
 *    PINOUT  R   LY  G     LB    B   I                 | PINOUT
 */

/*
HARDWARE PIN ALLOCATION REQUIREMENTS

MIDI board for preset sounds:
NAME:   TYPE:   ASSIGNMENT:
VDD:    ?V
GND:    GND     GND
RX:     Serial  PIN0
TX:     Serial  PIN1
GPIO1:   GPIO   PIN9

DPAD buttoms for GUI:
NAME:   TYPE:   ASSIGNMENT:
VDD:    5V
GND:    GND
GPIO1:   GPIO     Pin2
GPIO2:   GPIO     Pin3
GPIO3:   GPIO     Pin4
GPIO4:   GPIO     Pin5


Measure matrix decoders for buttons and LEDs
NAME:    TYPE:   ASSIGNMENT:
VDD:     5V
GND:     GND
GPIO1:   GPIO       Pin24
GPIO2:   GPIO       Pin25
GPIO3:   GPIO       Pin26
GPIO4:   GPIO       Pin27
GPIO5:   GPIO       Pin28
GPIO6:   GPIO       Pin29
GPIO7:   GPIO       Pin30
GPIO8:   GPIO       Pin31
GPIO9:   GPIO       Pin32
GPIO10:  GPIO       Pin8
GPIO11:  GPIO       Pin10
GPIO12:  GPIO       Pin11
GPIO13:  GPIO       Pin12

LCD for GUI:
NAME:   TYPE:   ASSIGNMENT:
VDD:    5V      5V
GND:    GND     GND
SDA:    I2C     PIN18
SCL:    I2C     PIN19

Serial Flash Memory for custom sounds:
** CANNOT BE CHANGED **
NAME:       TYPE:   ASSIGNMENT:
VDD:        3.3     3.3V
VSS:        GND     GND
CE#:        CS0     PIN48
SCLK:       CLK     PIN53
SI/SIO[0]:  D0      PIN52
SO/SIO[1]:          PIN49
SIO[2]:     D2      PIN50
SIO[3]:     D3      PIN54

PT8211 DAC for custom sounds:
** UNCERTAIN CAN BE CHANGED **
NAME:   TYPE:   ASSIGNMENT:
VDD:    3.3V      3.3V
GND:    GND       GND
BCK:    I2s       PIN21
WS:     I2s       PIN20
DIN:    I2s       PIN7

Onboard SD card reader for custom sounds and tracks:
** CANNOT BE CHANGED **
NAME:   TYPE:   ASSIGNMENT:
VDD:    3.3V    3.3V
GND:    GND     GND
CMD:    FLEXIO  PIN45
CLK:    FLEXIO  PIN44
DAT0:   FLEXIO  PIN43
DAT1:   FLEXIO  PIN42
DAT2:   FLEXIO  PIN47
DAT3:   FLEXIO  PIN46


*/

/**************************
MODULE LINKING
**************************/

#include "lcd-module.h"
#include "sd-storage-module.h"
#include "button-ui-module.h"
#include "custom-sound-module.h"
#include "test-module.h"
#include "midi-sound-module.h"
#include <Metro.h>

/**************************
DAC PIN ASSIGNMENTS
**************************/
#define DAC_DIN 7
#define DAC_WS 20
#define DAC_BCK 21

/**************************
LCD PIN ASSIGNMENTS
**************************/
#define LCD_I2C 0x27 // pin18 pin19

/**************************
DPAD BUTTONS PIN ASSIGNMENTS
**************************/
#define BUTTON_DPAD_UP 26
#define BUTTON_DPAD_DOWN 25
#define BUTTON_DPAD_LEFT 24
#define BUTTON_DPAD_RIGHT 27

/**************************
MATRIX BUTTON PIN ASSIGNMENTS
**************************/
#define BUTTON_MATRIX_ROW_1 999
#define BUTTON_MATRIX_ROW_2 999
#define BUTTON_MATRIX_ROW_3 999
#define BUTTON_MATRIX_ROW_4 999

#define BUTTON_MATRIX_COLUMN_1 28
#define BUTTON_MATRIX_COLUMN_2 29
#define BUTTON_MATRIX_COLUMN_3 30
#define BUTTON_MATRIX_COLUMN_4 999
#define BUTTON_MATRIX_COLUMN_5 999
#define BUTTON_MATRIX_COLUMN_6 999
#define BUTTON_MATRIX_COLUMN_7 999
#define BUTTON_MATRIX_COLUMN_8 999
#define BUTTON_MATRIX_COLUMN_9 999

/**************************
MEASURE MATRIX BUTTON PIN ASSIGNMENTS
**************************/
#define BUTTON_MEASURE_MATRIX_ROW_1 32
#define BUTTON_MEASURE_MATRIX_ROW_2 31
#define BUTTON_MEASURE_MATRIX_ROW_3 34
#define BUTTON_MEASURE_MATRIX_ROW_4 33

#define BUTTON_MEASURE_MATRIX_COLUMN_1 23
#define BUTTON_MEASURE_MATRIX_COLUMN_2 22
#define BUTTON_MEASURE_MATRIX_COLUMN_3 19
#define BUTTON_MEASURE_MATRIX_COLUMN_4 18
#define BUTTON_MEASURE_MATRIX_COLUMN_5 17
#define BUTTON_MEASURE_MATRIX_COLUMN_6 16

/**************************
MEASURE MATRIX LED PIN ASSIGNMENTS
**************************/
#define LED_MEASURE_MATRIX_ROW_1 38
#define LED_MEASURE_MATRIX_ROW_2 37
#define LED_MEASURE_MATRIX_ROW_3 36
#define LED_MEASURE_MATRIX_ROW_4 35

#define LED_MEASURE_MATRIX_COLUMN_1 15
#define LED_MEASURE_MATRIX_COLUMN_2 14
#define LED_MEASURE_MATRIX_COLUMN_3 13
#define LED_MEASURE_MATRIX_COLUMN_4 41
#define LED_MEASURE_MATRIX_COLUMN_5 40
#define LED_MEASURE_MATRIX_COLUMN_6 39

/**************************
HARDWARE CONFIGURATIONS
**************************/

// LCD
#define LCD_ROWS 4
#define LCD_COLUMNS 20

// MATRIX
#define MATRIX_ROWS 4
#define MATRIX_COLUMNS 9
#define MEASURE_MATRIX_ROWS 4
#define MEASURE_MATRIX_COLUMNS 6

// PALETTE
#define PALETTE_MATRIX_ROWS 4
#define PALETTE_MATRIX_COLUMNS 3

/**************************
PROGRAM VARIABLES
**************************/
#define NAV_FORWARD 1
#define NAV_BACKWARD -1
#define NAV_UP -1
#define NAV_DOWN 1

/**************************
PROGRAM STRUCTS
**************************/

#endif // MAIN_H