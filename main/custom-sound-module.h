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
// #include <SD.h>
#include <SerialFlash.h>

struct dac_pin_config
{
    const int din;
    const int ws;
    const int bck;
};

// GUItool: begin automatically generated code
AudioPlaySdWav playSdWav3;  // xy=181,257
AudioPlaySdWav playSdWav4;  // xy=188,328
AudioPlaySdWav playSdWav2;  // xy=190,200
AudioPlaySdWav playSdWav1;  // xy=210,131
AudioMixer4 mixer1;         // xy=426,163
AudioMixer4 mixer2;         // xy=429,266
AudioOutputPT8211_2      pt8211_2_1;     //xy=443,203
AudioConnection patchCord1(playSdWav3, 0, mixer1, 2);
AudioConnection patchCord2(playSdWav3, 1, mixer2, 2);
AudioConnection patchCord3(playSdWav4, 0, mixer1, 3);
AudioConnection patchCord4(playSdWav4, 1, mixer2, 3);
AudioConnection patchCord5(playSdWav2, 0, mixer1, 1);
AudioConnection patchCord6(playSdWav2, 1, mixer2, 1);
AudioConnection patchCord7(playSdWav1, 0, mixer1, 0);
AudioConnection patchCord8(playSdWav1, 1, mixer2, 0);
AudioConnection patchCord9(mixer1, 0, pt8211_2_1, 0);
AudioConnection patchCord10(mixer2, 0, pt8211_2_1, 1);

#define MAX_FILENAME_LENGTH 50

// functions, extern variables, structs go here
void onboard_dac_init(void);
void playFile(const char *filename);

#endif // CUSTOM_SOUND_MODULE_H