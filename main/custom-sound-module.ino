/*
 *	example function library
 *
 *
 *
 */
#include "custom-sound-module.h"

int dac_init(void)
{
  AudioMemory(10);
  rraw_a1.enableInterpolation(true);
  amp1.gain(dac_vol);
  amp2.gain(dac_vol);

  Serial.println("onboard dac initialized");
  return 0;
}

// Define a fixed-size buffer for temp_str
char temp_str[MAX_FILENAME_LENGTH]; // Adjust MAX_FILENAME_LENGTH as needed

int playFile(newdigate::audiosample *cached_sound)
{

  int mixer = -1;

  if (rraw_a1.isPlaying() == false)
  {
    // Serial.println("Start playing 1");
    rraw_a1.playRaw(cached_sound->sampledata, cached_sound->samplesize / 2, 1);

    mixer = 1;
  }
  else if (rraw_a2.isPlaying() == false)
  {
    // Serial.println("Start playing 2");
    rraw_a2.playRaw(cached_sound->sampledata, cached_sound->samplesize / 2, 1);

    mixer = 2;
  }
  else if (rraw_a3.isPlaying() == false)
  {
    // Serial.println("Start playing 3");
    rraw_a3.playRaw(cached_sound->sampledata, cached_sound->samplesize / 2, 1);

    mixer = 3;
  }
  else if (rraw_a4.isPlaying() == false)
  {
    // Serial.println("Start playing 4");
    rraw_a4.playRaw(cached_sound->sampledata, cached_sound->samplesize / 2, 1);

    mixer = 4;
  }

  delay(1); // wait for library to parse WAV info

  // // Serial.print(AudioMemoryUsage());
  // // Serial.print(",");
  // Serial.println(AudioMemoryUsageMax());

  return mixer;
}
void stopFile(int mixer)
{
  if (mixer == 0)
  {
    rraw_a1.stop();
    rraw_a2.stop();
    rraw_a3.stop();
    rraw_a4.stop();
    /*
    if (rraw_a1.isPlaying())
    {
      rraw_a1.stop();
    }
    if (rraw_a2.isPlaying())
    {
      rraw_a2.stop();
    }
    if (rraw_a3.isPlaying())
    {
      rraw_a3.stop();
    }
    if (rraw_a4.isPlaying())
    {
      rraw_a4.stop();
    }
  }
  if (mixer == 1)
  {
    // Serial.println("Stopping 1");
    rraw_a1.stop();
  }
  else if (mixer == 2)
  {
    // Serial.println("Stopping 2");
    rraw_a2.stop();
  }
  else if (mixer == 3)
  {
    // Serial.println("Stopping 3");
    rraw_a3.stop();
  }
  else if (mixer == 4)
  {
    // Serial.println("Stopping 4");
    rraw_a4.stop();
  }
  */
  }
  
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

  // Copy CUSTOM_SOUNDS_DIRECTORY prefix into temp_str
  strcpy(temp_str, CUSTOM_SOUNDS_DIRECTORY);

  // Concatenate filename to temp_str
  strcat(temp_str, filename);

  // Serial.print("Playing file: ");
  Serial.println(filename);
  return loader.loadSample(temp_str);
}
