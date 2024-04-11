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
#include <TeensyVariablePlayback.h>
#include "flashloader.h"
#include <SerialFlash.h>

#define DAC_GAIN 1.0
struct dac_pin_config
{
    const int din;
    const int ws;
    const int bck;
};

#if USING_MAIN_PCB == 1
// GUItool: begin automatically generated code
AudioPlayArrayResmp rraw_a3;    // xy=321,513
AudioPlayArrayResmp rraw_a4;    // xy=321,513
AudioPlayArrayResmp rraw_a2;    // xy=321,513
AudioPlayArrayResmp rraw_a1;    // xy=321,513
AudioMixer4 mixer1;             // xy=426,173
AudioMixer4 mixer2;             // xy=429,300
AudioAmplifier amp1;            // xy=605,194
AudioAmplifier amp2;            // xy=606,289
AudioOutputPT8211_2 pt8211_2_1; // xy=803,236
AudioConnection patchCord1(rraw_a3, 0, mixer1, 2);
AudioConnection patchCord2(rraw_a3, 1, mixer2, 2);
AudioConnection patchCord3(rraw_a4, 0, mixer1, 3);
AudioConnection patchCord4(rraw_a4, 1, mixer2, 3);
AudioConnection patchCord5(rraw_a2, 0, mixer1, 1);
AudioConnection patchCord6(rraw_a2, 1, mixer2, 1);
AudioConnection patchCord7(rraw_a1, 0, mixer1, 0);
AudioConnection patchCord8(rraw_a1, 1, mixer2, 0);
AudioConnection patchCord9(mixer1, amp1);
AudioConnection patchCord10(mixer2, amp2);
AudioConnection patchCord11(amp1, 0, pt8211_2_1, 0);
AudioConnection patchCord12(amp2, 0, pt8211_2_1, 1);
// GUItool: end automatically generated code

// AudioPlayArrayResmp rraw_a3;    // xy=321,513
// AudioPlayArrayResmp rraw_a4;    // xy=321,513
// AudioPlayArrayResmp rraw_a2;    // xy=321,513
// AudioPlayArrayResmp rraw_a1;    // xy=321,513
// AudioMixer4 mixer1;             // xy=426,163
// AudioMixer4 mixer2;             // xy=429,266
// AudioOutputPT8211_2 pt8211_2_1; // I2s2
// AudioConnection patchCord1(rraw_a3, 0, mixer1, 2);
// AudioConnection patchCord2(rraw_a3, 1, mixer2, 2);
// AudioConnection patchCord3(rraw_a4, 0, mixer1, 3);
// AudioConnection patchCord4(rraw_a4, 1, mixer2, 3);
// AudioConnection patchCord5(rraw_a2, 0, mixer1, 1);
// AudioConnection patchCord6(rraw_a2, 1, mixer2, 1);
// AudioConnection patchCord7(rraw_a1, 0, mixer1, 0);
// AudioConnection patchCord8(rraw_a1, 1, mixer2, 0);
// AudioConnection patchCord9(mixer1, 0, pt8211_2_1, 0);
// AudioConnection patchCord10(mixer2, 0, pt8211_2_1, 1);
#else
// GUItool: begin automatically generated code
AudioPlayArrayResmp rraw_a3;  // xy=321,513
AudioPlayArrayResmp rraw_a4;  // xy=321,513
AudioPlayArrayResmp rraw_a2;  // xy=321,513
AudioPlayArrayResmp rraw_a1;  // xy=321,513
AudioMixer4 mixer1;           // xy=426,173
AudioMixer4 mixer2;           // xy=429,300
AudioAmplifier amp1;          // xy=605,194
AudioAmplifier amp2;          // xy=606,289
AudioOutputPT8211_2 pt8211_1; // xy=803,236
AudioConnection patchCord1(rraw_a3, 0, mixer1, 2);
AudioConnection patchCord2(rraw_a3, 1, mixer2, 2);
AudioConnection patchCord3(rraw_a4, 0, mixer1, 3);
AudioConnection patchCord4(rraw_a4, 1, mixer2, 3);
AudioConnection patchCord5(rraw_a2, 0, mixer1, 1);
AudioConnection patchCord6(rraw_a2, 1, mixer2, 1);
AudioConnection patchCord7(rraw_a1, 0, mixer1, 0);
AudioConnection patchCord8(rraw_a1, 1, mixer2, 0);
AudioConnection patchCord9(mixer1, amp1);
AudioConnection patchCord10(mixer2, amp2);
AudioConnection patchCord11(amp1, 0, pt8211_1, 0);
AudioConnection patchCord12(amp2, 0, pt8211_1, 1);
#endif

#define MAX_FILENAME_LENGTH 50
#define MAX_SOUNDS 16
newdigate::audiosample *cached_samples[MAX_SOUNDS];
newdigate::audiosample *temp_sample;
// newdigate::audiosample *player1;
// newdigate::audiosample *player2;
// newdigate::audiosample *player3;
// newdigate::audiosample *player4;
newdigate::flashloader loader;

// functions, extern variables, structs go here
int dac_init(void);
int playFile(newdigate::audiosample *cached_sound);
newdigate::audiosample *cache_sd_sound(const char *filename);
void stopFile(int mixer);

#endif // CUSTOM_SOUND_MODULE_H