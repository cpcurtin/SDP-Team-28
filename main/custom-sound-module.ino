/*
 *	example function library
 *
 *
 *
 */
#include "custom-sound-module.h"

void onboard_dac_init(void)
{
  AudioMemory(16);

  Serial.println("onboard dac initialized");
}

// Define a fixed-size buffer for temp_str
char temp_str[MAX_FILENAME_LENGTH]; // Adjust MAX_FILENAME_LENGTH as needed

void playFile(const char *filename)
{
  // Calculate the length of the string
  size_t filename_len = strlen(filename);

  // Check if the filename length exceeds the buffer size
  if (filename_len >= MAX_FILENAME_LENGTH - 8)
  {
    Serial.println("Filename is too long for buffer");
    return;
  }

    Serial.println(playSdWav1.isPlaying());
  Serial.println(playSdWav2.isPlaying());
  // Serial.println(playSdWav3.isPlaying());
  // Serial.println(playSdWav4.isPlaying());

  // Copy "/sounds/" prefix into temp_str
  strcpy(temp_str, "/sounds/");

  // Concatenate filename to temp_str
  strcat(temp_str, filename);

  Serial.print("Playing file: ");
  Serial.println(filename);

  if (playSdWav1.isPlaying() == false)
  {
    Serial.println("Start playing 1");
    playSdWav1.play(temp_str);
    delay(10); // wait for library to parse WAV info
  }
  else if (playSdWav2.isPlaying() == false)
  {
    Serial.println("Start playing 2");
    playSdWav2.play(temp_str);
    delay(10); // wait for library to parse WAV info
  }
  // else if (playSdWav3.isPlaying() == false)
  // {
  //   Serial.println("Start playing 3");
  //   playSdWav3.play(temp_str);
  //   delay(10); // wait for library to parse WAV info
  // }
  // else if (playSdWav4.isPlaying() == false)
  // {
  //   Serial.println("Start playing 4");
  //   playSdWav4.play(temp_str);
  //   delay(10); // wait for library to parse WAV info
  // }
}
