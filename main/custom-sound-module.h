/*
 *
 *       example function library
 *
 *
 */
#ifndef CUSTOM_SOUND_MODULE_H
#define CUSTOM_SOUND_MODULE_H

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>



// functions, extern variables, structs go here
void onboard_dac_init(void);
void playFile(const char *filename);

#endif // CUSTOM_SOUND_MODULE_H