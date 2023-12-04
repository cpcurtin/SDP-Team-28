/*
 *	example function library
 *
 *
 *
 */
#include "custom-sound-module.h"
// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=391,229
AudioPlaySdWav           playSdWav2;     //xy=391,292
AudioMixer4              mixer1;         //xy=584,204
AudioMixer4              mixer2;         //xy=584,340
AudioOutputPT8211        pt8211_1;       //xy=750,264
AudioConnection          patchCord1(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord2(playSdWav1, 1, mixer2, 0);
AudioConnection          patchCord3(playSdWav2, 0, mixer1, 1);
AudioConnection          patchCord4(playSdWav2, 1, mixer2, 1);
AudioConnection          patchCord5(mixer1, 0, pt8211_1, 0);
AudioConnection          patchCord6(mixer2, 0, pt8211_1, 1);

void onboard_dac_init(void)
{
  AudioMemory(40);

  Serial.println("onboard dac initialized");
  
}

void playFile(const char *filename)
{
  
  char *temp_str = (char *)malloc(strlen(filename) + 10);
  char *temp_str2 = (char *)malloc(strlen(filename));
  strcpy(temp_str, "/sounds/");
  strcpy(temp_str2, filename);
  strcat(temp_str, temp_str2);
  
  Serial.print("Playing file: ");
  Serial.println(filename);


  // Start playing the file.  This sketch continues to
  // run while the file plays.

  if(!playSdWav1.isPlaying()){
  playSdWav1.play(strdup(temp_str));
  }
  else{
    playSdWav2.play(strdup(temp_str));
  }
  

  // A brief delay for the library read WAV info
  // delay(25);

  // Simply wait for the file to finish playing.
  // while (playSdWav1.isPlaying()) {
  //   // uncomment these lines if you audio shield
  //   // has the optional volume pot soldered
  //   //float vol = analogRead(15);
  //   //vol = vol / 1024;
  //   // sgtl5000_1.volume(vol);
  // }
   free(temp_str);
  free(temp_str2);
}