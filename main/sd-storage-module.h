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

typedef struct Track
{
  char filename[64];
  int id;
  int bpm;
  int active_measures;
  int measure_beats;
  int measure_steps;
  struct Measure *measure_list;
} Track;

array_with_size *custom_sound_list = new array_with_size;
array_with_size *track_list = new array_with_size;

// Track active_track = {
//     "DEFAULT.json", 0, 50, 1, 4, 6};
Track active_track;

int sd_init(void);
array_with_size *sd_fetch_sounds(void);
void freeArrayOfStrings(char **stringArray, size_t numStrings);
void listfiles(void);
void printDirectory(File dir, int numSpaces);
void printSpaces(int num);
void printTime(const DateTimeFields tm);

void read_track(const char *filename, Track &config);
void save_track(const char *filename, Track &config);
void print_JSON(const char *filename);
array_with_size *sd_fetch_tracks(void);

int sd_delete_track(const char *filename);

#endif // SD_STORAGE_MODULE_H

/*
{
    "track": {
        "filename": "test_filename",
        "id": 0,
        "measure_steps": 0,
        "measure_beats": 0,
        "measures": [
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