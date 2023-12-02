CODE STRUCTURE:

- main.c // main program file, contains setup and forever loop
  // imports module header files to access functions
  - <hardware>-module.c // functions written to preform hardware functions
  - <hardware>-module.h // header files for hardware function modules

module.h: // example header file

    #ifndef <HARDWARE>\_MODULE_H
    #define <HARDWARE>\_MODULE_H

    // define global extern variables here
    // insert function definintions for associated .c file
    // example:
    {
    #include <SD.h>

    extern const int chipSelect;

    void sdinit();
    }

    #endif // <HARDWARE>\_MODULE_H

module.c: // example c file
   /*
    *       hardware component function library outline
    *
    *
    *
    */
    #include "sd-storage-module.h"
    #include <SD.h>
    // set Teensy4.1 onbaord SD card chipset
    const int chipSelect = BUILTIN_SDCARD;

    void sdinit()
    {
    ...
    }



how to implement navigation:
navigation states are constant
scroll arrays are constant

binary tree for navigation states
  >leaf node points to scroll array?


###### LCD NAVIGATION ###########
lcd navigation:
# < comment
>option
  >child option

scroll is looping values through string array
#################################
initialized: 

-welcome state

3 second delay

-base nav state
  >Sounds
    >custom sounds
      >scroll custom sounds
        >select for sound palette
    >midi sounds
      >scroll midi sounds
        >select for sound palette
  >Effects
    #all digital??
    >scroll digital effects
      >select for effect palette

