#ifndef SD_STORAGE_MODULE_H
#define SD_STORAGE_MODULE_H

#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <vector>
#include <deque>

#define CUSTOM_SOUNDS_DIRECTORY "/sounds/"
#define TRACKS_DIRECTORY "/tracks/"

typedef struct array_with_size
{
    std::vector<std::string> array;
} array_with_size;

typedef struct Track
{
    std::string filename;
    int id;
    int bpm;
    int active_measures;
    int measure_beats;
    int measure_steps;
    int current_measure_id;
    struct Measure *current_measure;
    std::vector<struct Measure *> measure_list;
} Track;

std::deque<struct Sound *> cached_sounds;

std::vector<std::string> sd_fetch_sounds(void);
std::vector<std::string> sd_fetch_tracks(void);
std::vector<std::string> sd_fetch_measures(void);

bool find_sd_sound(std::string filename);
int sd_delete_track(std::string filename);
void read_track(std::string filename, Track *config);
void save_track(std::string filename, Track *config);
Measure *read_measure(std::string filename, int measure_id);
void read_palette(std::string filename, std::vector<Palette_Slot> &palette);
Track *current_track;

int sd_init(void);
int track_init(void);

void freeArrayOfStrings(char **stringArray, size_t numStrings);
void listfiles(void);
void printDirectory(File dir, int numSpaces);
void printSpaces(int num);
void printTime(const DateTimeFields tm);

void print_JSON(const char *filename);

int free_track(Track *track);

#endif // SD_STORAGE_MODULE_H
