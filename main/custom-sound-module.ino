/*
 *	example function library
 *
 *
 *
 */
#include "custom-sound-module.h"

void onboard_dac_init(void)
{
  AudioMemory(20);
  rraw_a1.enableInterpolation(true);

  Serial.println("onboard dac initialized");
}

// Define a fixed-size buffer for temp_str
char temp_str[MAX_FILENAME_LENGTH]; // Adjust MAX_FILENAME_LENGTH as needed

int playFile(newdigate::audiosample *cached_sound)
{
  
  int mixer;



  if (rraw_a1.isPlaying() == false)
  {
    Serial.println("Start playing 1");
    rraw_a1.playRaw(cached_sound->sampledata, cached_sound->samplesize / 2, 1);
    

    mixer = 1;
  }
  else if (rraw_a2.isPlaying() == false)
  {
    Serial.println("Start playing 2");
    rraw_a2.playRaw(cached_sound->sampledata, cached_sound->samplesize / 2, 1);
    
    
    mixer = 2;
  }
  else if (rraw_a3.isPlaying() == false)
  {
    Serial.println("Start playing 3");
    rraw_a3.playRaw(cached_sound->sampledata, cached_sound->samplesize / 2, 1);
    
    
    mixer = 3;
  }
  else if (rraw_a4.isPlaying() == false)
  {
    Serial.println("Start playing 4");
    rraw_a4.playRaw(cached_sound->sampledata, cached_sound->samplesize / 2, 1);
    
    
    mixer = 4;
  }
  
  
      
    delay(1); // wait for library to parse WAV info

  

  

  Serial.print(AudioMemoryUsage());
  Serial.print(",");
  Serial.println(AudioMemoryUsageMax());
  
  return mixer;
}
void stopFile(int mixer)
{
}
newdigate::audiosample *cache_sd_sound(const char *filename)
{
  // Calculate the length of the string
  size_t filename_len = strlen(filename);

  // Check if the filename length exceeds the buffer size
  if (filename_len >= MAX_FILENAME_LENGTH - 8)
  {
    Serial.println("Filename is too long for buffer");
  }

  // Copy "/sounds/" prefix into temp_str
  strcpy(temp_str, "/sounds/");

  // Concatenate filename to temp_str
  strcat(temp_str, filename);

  Serial.print("Playing file: ");
  Serial.println(filename);
  return loader.loadSample(temp_str);
}
