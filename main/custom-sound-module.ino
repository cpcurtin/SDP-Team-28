/*
 *	example function library
 *
 *
 * double check what is happening when playing custom sound
 */
#include "custom-sound-module.h"

int dac_init(void)
{
  // AudioMemory(4);
  AudioMemory(8);
  // AudioMemory(12);
  // AudioMemory(16);
  rraw_a1.enableInterpolation(true);
  rraw_a2.enableInterpolation(true);
  rraw_a3.enableInterpolation(true);
  rraw_a4.enableInterpolation(true);
  amp1.gain(dac_vol);
  amp2.gain(dac_vol);

  Serial.println("onboard dac initialized");
  return 0;
}

// Define a fixed-size buffer for temp_str

int playFile(newdigate::audiosample *cached_sound)
{
  Serial.print("MAX MEM: ");
  Serial.println(AudioMemoryUsageMax());
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
  // rraw_a1.stop();
  // rraw_a2.stop();
  // rraw_a3.stop();
  // rraw_a4.stop();

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

#if USING_SAFE_STRINGS == 1 // safe - new
int free_cached_sounds(Track *track)
{

  while (track->cached_sounds.size() != 0)
  {

    Serial.println(track->cached_sounds.front().filename.c_str());

    delete track->cached_sounds.front().sd_cached_sound;

    track->cached_sounds.pop_front();
  }

  return 0;
}
newdigate::audiosample *cache_sd_sound(std::string filename)
{

  std::string full_path = CUSTOM_SOUNDS_DIRECTORY + filename;

  return loader.loadSample(full_path.c_str());
}
#else // unsafe - old
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
int free_cached_sounds(Track *track)
{
  Serial.println("enter func");
  while (track->cached_sounds.size() != 0)
  {
    Serial.println("loop");
    Serial.println(track->cached_sounds.front().filename);
    Serial.println("loop");

    delete track->cached_sounds.front().sd_cached_sound;
    Serial.println("post free");
    track->cached_sounds.pop_front();
    Serial.println("pop");
  }
  Serial.println("end");

  return 0;
}
#endif
