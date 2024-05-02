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
  // amp2.gain(dac_vol);

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
    rraw_a1.playRaw(cached_sound->sampledata, cached_sound->samplesize / 20, 1);

    mixer = 1;
  }
  else if (rraw_a2.isPlaying() == false)
  {
    // Serial.println("Start playing 2");
    rraw_a2.playRaw(cached_sound->sampledata, cached_sound->samplesize / 20, 1);

    mixer = 2;
  }
  else if (rraw_a3.isPlaying() == false)
  {
    // Serial.println("Start playing 3");
    rraw_a3.playRaw(cached_sound->sampledata, cached_sound->samplesize / 20, 1);

    mixer = 3;
  }
  else if (rraw_a4.isPlaying() == false)
  {
    // Serial.println("Start playing 4");
    rraw_a4.playRaw(cached_sound->sampledata, cached_sound->samplesize / 20, 1);

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
  /*
  rraw_a1.stop();
  rraw_a2.stop();
  rraw_a3.stop();
  rraw_a4.stop();
  */
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
    */
}

Sound *cache_sd_sound(std::string filename)
{
  Sound *cached_sound = find_cached_sounds(filename);
  /*  CHECK IF SOUND ALREADY CACHED    */
  if (cached_sound == nullptr)
  {
    Serial.println("SOUND NOT ALREADY IN CACHE");
    std::string full_path = CUSTOM_SOUNDS_DIRECTORY + filename;
    Sound *new_csound = new Sound;
    new_csound->bank = -1;
    new_csound->instrument = -1;
    new_csound->note = -1;
    new_csound->sd_cached_sound = loader.loadSample(full_path.c_str());
    new_csound->filename = filename;
    new_csound->empty = false;

    /*  CHECK IF SOUND CACHED SUCCESSFULLY    */
    if (new_csound->sd_cached_sound == nullptr)
    {
      /*  UNSUCCESSFULLY CACHED   */
      Serial.println("CACHING FAILED");
      delete new_csound;
      return nullptr;
    }
    else
    {
      /*  SUCCESSFULLY CACHED   */
      Serial.println("CACHING SUCCESSFUL");
      cached_sounds.push_back(new_csound); // add to already cached sounds
      return new_csound;
    }
  }
  else
  {
    Serial.println("SOUND ALREADY IN CACHE");
  }
  return cached_sound;
}

Sound *find_cached_sounds(std::string filename)
{
  if (!cached_sounds.empty())
  {
    auto cached_sound_iter = cached_sounds.begin();
    while (cached_sound_iter != cached_sounds.end())
    {
      if ((*cached_sound_iter)->filename == filename)
      {
        Serial.print("check filename:");
        Serial.println(filename.c_str());
        Serial.print("found filename:");
        Serial.println((*cached_sound_iter)->filename.c_str());

        return (*cached_sound_iter);
      }
      cached_sound_iter++;
    }
  }
  return nullptr;
}

void print_cached_sounds(void)
{
  auto cached_sound_iter = cached_sounds.begin();
  int i = 0;
  Serial.print("\n");
  while (cached_sound_iter != cached_sounds.end())
  {
    Sound *cached_sound = *cached_sound_iter;
    Serial.print(i);
    Serial.print(". ");
    Serial.println(cached_sound->filename.c_str());
    i++;

    cached_sound_iter++;
  }
}