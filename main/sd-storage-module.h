#ifndef SD_STORAGE_MODULE_H
#define SD_STORAGE_MODULE_H
#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>

#define CUSTOM_SOUNDS_DIRECTORY "/sounds/"
#define TRACKS_DIRECTORY "/tracks/"

typedef struct array_with_size
{
  const char **array;
  size_t size;
} array_with_size;

typedef struct track
{
  char filename[64];
  int id;
  int bpm;
  int measure_steps;
} track;

array_with_size *custom_sound_list = new array_with_size;
array_with_size *track_list = new array_with_size;

track active_track = {
    "DEFAULT.json", // filename
    1,              // id
    120,            // bpm
    6              // measure_steps
};
track default_track=active_track;
int sd_init(void);
array_with_size *sd_fetch_sounds(void);
void freeArrayOfStrings(char **stringArray, size_t numStrings);
void listfiles(void);
void printDirectory(File dir, int numSpaces);
void printSpaces(int num);
void printTime(const DateTimeFields tm);

void read_track(const char *filename, track &config);
void save_track(const char *filename, track &config);
void print_JSON(const char *filename);
array_with_size *sd_fetch_tracks(void);

int sd_delete_track(const char *filename);

#endif // SD_STORAGE_MODULE_H
