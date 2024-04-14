#ifndef SD_STORAGE_MODULE_H
#define SD_STORAGE_MODULE_H
#ifndef USING_SAFE_STRINGS
#define USING_SAFE_STRINGS 1
#endif

#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <vector>
#include <deque>

#define CUSTOM_SOUNDS_DIRECTORY "/sounds/"
#define TRACKS_DIRECTORY "/tracks/"
#define FILENAME_MAX_SIZE 64

#if USING_SAFE_STRINGS == 1 // safe - new

std::vector<const char *> track_list;

typedef struct array_with_size
{
    std::vector<const char *> array;
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
    struct Measure *measure_list;
    std::deque<struct Sound> cached_sounds;
} Track;

std::vector<const char *> sd_fetch_sounds(void);
std::vector<const char *> sd_fetch_tracks(void);
bool find_sd_sound(std::string filename);
int sd_delete_track(std::string filename);

#else // unsafe - old

typedef struct array_with_size
{
    const char **array;
    size_t size;
} array_with_size;

typedef struct Track
{
    char filename[64];
    int id;
    int bpm;
    int active_measures;
    int measure_beats;
    int measure_steps;
    int current_measure_id;
    struct Measure *current_measure;
    struct Measure *measure_list;
    std::deque<struct Sound> cached_sounds;
} Track;

array_with_size *custom_sound_list = new array_with_size;
array_with_size *track_list = new array_with_size;
array_with_size *sd_fetch_sounds(void);
array_with_size *sd_fetch_tracks(void);
bool find_sd_sound(const char *filename);
int sd_delete_track(const char *filename);

#endif

Track *current_track;

int sd_init(void);
int track_init(void);

void freeArrayOfStrings(char **stringArray, size_t numStrings);
void listfiles(void);
void printDirectory(File dir, int numSpaces);
void printSpaces(int num);
void printTime(const DateTimeFields tm);

void read_track(const char *filename, Track *config);
void save_track(const char *filename, Track *config);
void print_JSON(const char *filename);

int free_track(Track *track);

#endif // SD_STORAGE_MODULE_H

/*
{
    "track": {
        "filename": "test_filename",
        "id": 0,
        "measure_steps": 0,
        "measure_beats": 0,
        "measure_list": [
            {
                "id": 0,
                "active_beats": 4,
                "effect_mode": false,
                "beat_list": [
                    {
                        "id": 0,
                        "active_steps": 6,
                        "step_list": [
                            {
                                "id": 0,
                                "active_sounds": 6,
                                "sound_list": [
                                    {
                                        "bank": -1,
                                        "instrument": -1,
                                        "note": -1,
                                        "filename": "test",
                                        "empty": true
                                    },
                                    {
                                        "bank": -1,
                                        "instrument": -1,
                                        "note": -1,
                                        "filename": "test",
                                        "empty": true
                                    },
                                    {
                                        "bank": -1,
                                        "instrument": -1,
                                        "note": -1,
                                        "filename": "test",
                                        "empty": true
                                    },
                                    {
                                        "bank": -1,
                                        "instrument": -1,
                                        "note": -1,
                                        "filename": "test",
                                        "empty": true
                                    }
                                ]
                            }
                        ]
                    }
                ]
            }
        ]
    }
}

*/