CODE STRUCTURE:

.ino files include predefined arduino functions etc.
.ino files are c++

- main.ino // main program file, contains setup and forever loop
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

module.ino: // example c file
/\*
_ hardware component function library outline
_ \* \*
\*/
#include "sd-storage-module.h"
#include <SD.h>
// set Teensy4.1 onbaord SD card chipset
const int chipSelect = BUILTIN_SDCARD;

    void sdinit()
    {
    ...
    }

################################################################################
################################ LCD NAVIGATION ################################
################################################################################

########################## THEORETICAL IMPLEMENTATION ##########################

how to implement navigation:
navigation states are constant
scroll arrays are constant

binary tree for navigation states

> leaf node points to scroll array?

############################### FRAMEWORK SYNTAX ###############################

# comment

>      - selectable option
>
> - child selectable option
>   scroll - display loopable array of leaf selections

############################## EXPECTED BEHAVIOR ###############################

initialized:

-welcome state

3 second delay

-base nav state

> Sounds
> custom sounds
> scroll custom sounds
> select for sound palette
> midi sounds
> scroll midi sounds
> select for sound palette
> Effects

    #all digital??
    >scroll digital effects
      >select for effect palette

###################### Note For Running On Battery Power #######################

If the system is going to be run on battery power, the serial_init() line in main.ino has to be removed!!!!!!!!!

If you do not remove this line, the system will not function on battery power!!!!!!!!!!!!!
