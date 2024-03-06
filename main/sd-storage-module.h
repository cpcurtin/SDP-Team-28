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

struct track
{

  int id;
  int bpm;
  int measure_steps;
};
// Number of elements in the array
const int arraySize = 5;
struct array_with_size *track_list = new struct array_with_size;

// File name on the SD card
// const char *fileName = "test.txt";
const char *fileNamejson = "test.json";

void sd_init(void);
struct array_with_size *parsefiles(void);
void freeArrayOfStrings(char **stringArray, size_t numStrings);
void listfiles(void);
void printDirectory(File dir, int numSpaces);
void printSpaces(int num);
void printTime(const DateTimeFields tm);

void read_track(const char *filename, struct track &config);
void save_track(const char *filename, struct track &config);
void print_JSON(const char *filename);
void parse_tracks(void);

#endif // SD_STORAGE_MODULE_H