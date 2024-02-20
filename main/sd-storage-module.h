#ifndef SD_STORAGE_MODULE_H
#define SD_STORAGE_MODULE_H
#include <Arduino.h>
#include <SD.h>

struct array_with_size
{
  const char **array;
  size_t size;
};

struct track
{
  char *name;
  int bpm;
};
// Number of elements in the array
const int arraySize = 5;

// File name on the SD card
const char *fileName = "data.txt";

void sd_init(void);
struct array_with_size *parsefiles(void);
void freeArrayOfStrings(char **stringArray, size_t numStrings);
void listfiles(void);
void printDirectory(File dir, int numSpaces);
void printSpaces(int num);
void printTime(const DateTimeFields tm);

void saveTracks(struct track singleTrack);
void read_STRUCT(void);
// struct tracks* loadTracks(int index);

#endif // SD_STORAGE_MODULE_H