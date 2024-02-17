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
 *    PIN0:   PWM                 CS1   RX1       CRX2  | PIN0: 
 *    PIN1:   PWM                 MISO1 TX1       CTX2  | PIN1: 
 *    PIN2:   PWM     OUT2                              | PIN2: 
 *    PIN3:   PWM     LRCLK2                            | PIN3: 
 *    PIN4:   PWM     BCLK2                             | PIN4: 
 *    PIN5:   PWM     IN2                               | PIN5: 
 *    PIN6:   PWM     OUT1D                             | PIN6: 
 *    PIN7:   PWM     OUT1A             RX2             | PIN7: 
 *    PIN8:   PWM     IN1               TX2             | PIN8: 
 *    PIN9:   PWM     OUT1C                             | PIN9: 
 *    PIN10:  PWM     MQSR        CS                    | PIN10:  
 *    PIN11:  PWM                 MOSI            CTX1  | PIN11:  
 *    PIN12:  PWM     MQSL        MISO                  | PIN12:     
 *    PIN13:  PWM                 SCK                   | PIN13:     
 *    PIN14:  PWM A0  S/PDIF OUT        TX3             | PIN14:       
 *    PIN15:  PWM A1  S/PDIF IN         RX3             | PIN15:         
 *    PIN16:      A2                    RX4 SCL1        | PIN16:         
 *    PIN17:      A3                    TX4 SDA1        | PIN17:           
 *    PIN18:  PWM A4                        SDA         | PIN18:           
 *    PIN19:  PWM A5                        SCL         | PIN19:        
 *    PIN20:      A6  LRCLK1            TX5             | PIN20:  
 *    PIN21:      A7  BCLK1             RX5             | PIN21:  
 *    PIN22:  PWM A8                              CTX1  | PIN22:    
 *    PIN23:  PWM A9  MCLK1                       CRX1  | PIN23:  
 *    PIN24:  PWM A10                       SCL2        | PIN24:  
 *    PIN25:  PWM A11                       SDA2        | PIN25:  
 *    PIN26:      A12             MOSI1                 | PIN26:  
 *    PIN27:      A13             SCK1                  | PIN27:  
 *    PIN28:  PWM                       RX7             | PIN28:  
 *    PIN29:  PWM                       TX7             | PIN29:  
 *    PIN30:                                      CRX3  | PIN30:  
 *    PIN31:                                      CTX3  | PIN31:       
 *    PIN32:          OUT1B                             | PIN32:  
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
 *    PIN42:  PWM     MISO2 DAT1                        | PIN42:  
 *    PIN43:  PWM     MOSI2 DAT0                        | PIN43:  
 *    PIN44:  PWM     CS2   CLK       SDA1              | PIN44:  
 *    PIN45:  PWM     SCK2  CMD       SCL1              | PIN45:  
 *    PIN46:  PWM           DAT3  RX5                   | PIN46:  
 *    PIN47:  PWM           DAT2  TX5                   | PIN47:  
 *    PIN48:      CS0             RX8                   | PIN48:  
 *    PIN49:      D1  SCK2                              | PIN49:  
 *    PIN50:      D2  MOSI2                             | PIN50:  
 *    PIN51:  PWM CS1                 SCL1              | PIN51:  
 *    PIN52:      D0              RX1                   | PIN52:  
 *    PIN53:      CLK             TX1                   | PIN53:
 *    PIN54:  PWM D3  MISO2                             | PIN54:
 *    PINOUT  R   LY  G     LB    B   I                 | PINOUT
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
// #define LCD_RS 2
// #define LCD_EN 3
// #define LCD_DIGITAL_4 4
// #define LCD_DIGITAL_5 5
// #define LCD_DIGITAL_6 6
// #define LCD_DIGITAL_7 8
#define LCD_I2C 0x27 // 18 19

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
Midi Definitions
**************************/
// Pins
#define VS1053 1
#define VS1053_RST 9
// Sound Banks
#define DEFAULT 0x00
#define Melody 0x79
#define Drums1 0x78
#define Drums2 0x7F
// Commands
#define NoteOn 0x90
#define NoteOff 0x80
#define MESSAGE 0xB0
#define BANK 0x00
#define VOLUME 0x07
#define PROGRAM 0xC0
#define OFF1 0x7B
#define OFF2 0x7C
#define OFF3 0x7D
// Define Serial Bus
#define MIDI Serial1
/**************************
Melodic Sounds
**************************/
#define AcoursticGrandPiano 1
#define BrightAcousticPiano 2
#define ElectricGrandPiano 3
#define HonkyTonkPiano 4
#define ElectricPiano1 5
#define ElectricPiano2 6
#define Harpsichord 7
#define Clavi 8
#define Celesta 9
#define Clockenspiel 10
#define MusicBox 11
#define Vibraphone 12
#define Marimba 13
#define Zylophone 14
#define TubularBells 15
#define Dulcimer 16
#define DrawbarOrgan 17
#define PercussiveOrgan 18
#define RockOrgan 19
#define ChurchOrgan 20
#define ReedOrgan 21
#define Accordion 22
#define Harmonica 23
#define TangoAccordian 24
#define AcousticGuitarNylon 25
#define AcoursticGuitarSteel 26
#define ElectricGuitarJazz 27
#define ElectricGuitarClean 28
#define ElectricGuitarMuted 29
#define OverdrivenGuitar 30
#define DistortionGuitar 31
#define GuitarHarmonics 32
#define AcousticBass 33
#define ElectricBassFinger 34
#define ElectricBassPick 35
#define FretlessBass 36
#define SlapBass1 37
#define SlapBass2 38
#define SynthBass1 39
#define SynthBass2 40
#define Violin 41
#define Viola 42
#define Cello 43
#define ContraBass 44
#define TremoloStrings 45
#define PizzicatoStrings 46
#define OrchestralHarp 47
#define Timpani 48
#define StringEnsemble1 49
#define StringEnsemble2 50
#define SythStrings1 51
#define SynthStrings2 52
#define ChoirAahs 53
#define Voice Oohs 54
#define SynthVoice 55
#define OrchestraHit 56
#define Trumpet 57
#define Trombone 58
#define Tuba 59
#define MuteTrumpet 60
#define FrenchHorn 61
#define BrassSelection 62
#define SymthBrass1 63
#define SynthBrass2 64
#define SopranoSax 65
#define AltoSax 66
#define TenorSax 67
#define BaritoneSax 68
#define Oboe 69
#define EnglishHorn 70
#define Bassoon 71
#define Clarinet 72
#define Piccolo 73
#define Flute 74
#define Recorder 75
#define PanFlute 76
#define BlownBottle 77
#define Shakuhachi 78
#define Whistle 79
#define Ocarina 80
#define SquareLead 81
#define SawLead 82
#define CalliopeLead 83
#define ChiffLead 84
#define CharangLead 85
#define VoiceLead 86
#define FifthsLead 87
#define BassLead 88
#define NewAge 89
#define WarmPad 90
#define Polysynth 91
#define Choir 92
#define Bowed 93
#define Metallic 94
#define Halo 95
#define Sweep 96
#define Rain 97
#define SoundTrack 98
#define Crystal 99
#define Atmosphere 100
#define Brightness 101
#define Goblins 102
#define Echoes 103
#define SciFi 104
#define Sitar 105
#define Banjo 106
#define Shamisen 107
#define Koto 108
#define Kalimba 109
#define BagPipe 110
#define Fiddle 111
#define Shanai 112
#define TimkleBell 113
#define Agogo 114
#define PitchedPercussion 115
#define WoodBlock 116
#define TaikoDrum 117
#define MelodicTom 118
#define SynthDrum 119
#define ReverseCymbal 120
#define GuitarFretNoise 121
#define BreathNoise 122
#define Seashore 123
#define BirdTweet 124
#define TelephoneRing 125
#define Helicopter 126
#define Applause 127
#define Gunshot 128

/**************************
Percussive Sounds
**************************/
#define HighQ 27
#define Slap 28
#define ScratchPush 29
#define ScratchPull 30
#define Sticks 31
#define SquareClick 32
#define MetClick 33
#define MetBell 34
#define AcousticBassDrum 35
#define BassDrum 36
#define SideStick 37
#define AcousticSnare 38
#define Clap 39
#define ElectricSnare 40
#define LowFloorTom 41
#define ClosedHiHat 42
#define HighFloorTom 43
#define PedalHiHat 44
#define LowTom 45
#define OpenHiHat 46
#define LowMidTom 47
#define HighMidTom 48
#define Crash1 49
#define HighTom 50
#define Ride1 51
#define China 52
#define RideBell 53
#define Tambourine 54
#define Splash 55
#define Cowbell 56
#define Crash2
#define Vibraslap 58
#define Ride2 59
#define HighBongo 60
#define LowBogo 61
#define MuteHighConga 62
#define HighConga 63
#define LowConga 64
#define HighTimbale 65
#define LowTimbale 66
#define HighAgogo 67
#define LowAgogo 68
#define Cabasa 69
#define Maracas 70
#define ShortWhistle 71
#define LongWhistle 72
#define ShortGuiro 73
#define LongGuiro 74
#define Claves 75
#define HighWoodBlock 76
#define LowWoodBlock 77
#define Mute Cuica 78
#define Cuica 79
#define MuteTriangle 80
#define Triangle 81
#define Shaker 82
#define JingleBell 83
#define BellTree 84
#define Castanets 85
#define MuteSurdo 86
#define Surdo 87

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

struct lcd_pin_config
{
    const int i2c;
    const int rows;
    const int columns;
};

struct dac_pin_config
{
    const int din;
    const int ws;
    const int bck;
};

struct dpad_pin_config
{
    const int up;
    const int down;
    const int left;
    const int right;
    const int select;
};
struct lcd_nav
{
    char *name;
    char **ptr_str_array;
    struct lcd_nav *parent;
    struct lcd_nav **child;
    size_t size;
    char **lcd_state;
    int index;
    int depth;
};

struct nav_config
{
    struct array_with_size *sounds_custom;
    struct array_with_size *sounds_midi;
    struct array_with_size *effects;
    struct array_with_size *tracks;
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

#endif // MAIN_H