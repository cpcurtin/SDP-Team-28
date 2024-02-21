#ifndef SD_STORAGE_MODULE_H
#define SD_STORAGE_MODULE_H
#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>

struct array_with_size
{
  const char **array;
  size_t size;
};

struct track {
  char hostname[64];
  int port;
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

void read_track(const char* filename, struct track& config);
void save_track(const char* filename, const struct track& config);
void print_JSON(const char* filename);

#endif // SD_STORAGE_MODULE_H