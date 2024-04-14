/*
 *       SD card function library
 *
 *
 *
 */
#include <Arduino.h>
// #include <SD.h>
#include "sd-storage-module.h"

int sd_init(void)
{

  if (!SD.begin(BUILTIN_SDCARD))
  {
    Serial.println("initialization failed!");
    return 1;
  }
  return 0;
}

#if USING_SAFE_STRINGS == 1 // safe - new

int track_init(void)
{

  // Allocate memory for the new Track
  Track *new_track = new (std::nothrow) Track;
  if (!new_track)
  {
    Serial.println("Error: Memory allocation failed for Track.");
    return -1;
  }

  // Initialize the Track members
  new_track->filename = "DEFAULT.json";
  new_track->id = 0;
  new_track->bpm = 120;
  new_track->active_measures = 1;
  new_track->measure_beats = 4;
  new_track->measure_steps = 6;
  new_track->current_measure_id = 0;

  // Allocate memory for the measure_list
  new_track->measure_list = new (std::nothrow) Measure[new_track->active_measures];
  if (!new_track->measure_list)
  {
    Serial.println("Error: Memory allocation failed for Measure list.");
    delete new_track; // Free allocated memory
    return -1;
  }

  // Create measures and add them to the measure_list
  for (int i = 0; i < new_track->active_measures; i++)
  {

    new_track->measure_list[i] = *measure_create(i);
  }

  current_measure = &(new_track->measure_list[new_track->current_measure_id]);
  current_track = new_track;
  current_track->current_measure = current_measure;

  return 0; // Indicate success
}

std::vector<std::string> sd_fetch_sounds(void)
{
  std::vector<std::string> filenames;

  File root = SD.open(CUSTOM_SOUNDS_DIRECTORY);
  if (!root)
  {
    // Error handling if the directory couldn't be opened
    return filenames;
  }

  // Iterate through the files in the directory
  while (true)
  {
    File entry = root.openNextFile();
    if (!entry)
    {
      break; // No more files
    }

    // Skip system files
    if (strncmp(entry.name(), "._", 2) == 0)
    {
      continue;
    }

    // Store the filename in the vector
    filenames.push_back(strdup(entry.name()));
    entry.close();
  }

  // Close the "sounds" directory
  root.close();

  return filenames;
}

std::vector<std::string> sd_fetch_tracks(void)
{
  std::vector<std::string> filenames;

  File root = SD.open(TRACKS_DIRECTORY);
  if (!root)
  {
    // Error handling if the directory couldn't be opened
    return filenames;
  }

  // Iterate through the files in the directory
  while (true)
  {
    File entry = root.openNextFile();
    if (!entry)
    {
      break; // No more files
    }

    // Skip system files
    if (strncmp(entry.name(), "._", 2) == 0)
    {
      continue;
    }

    // Store the filename in the vector
    filenames.push_back(strdup(entry.name()));
    entry.close();
  }

  // Close the "tracks" directory
  root.close();

  return filenames;
}

void read_track(std::string filename, Track *config)
{
  bool file_found = false;
  bool already_cached = false;

  std::string full_path = TRACKS_DIRECTORY + filename;
  File file = SD.open(full_path.c_str());

  // Allocate a temporary JsonDocument
  JsonDocument doc;

  // Deserialize the JSON document

  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  else
  {
    Serial.println("DESERIALIZE PASSED TRACK");
  }
  int btp = 0;
  int stp = 0;
  int sp = 0;
  Track *new_track = new Track;
  Measure *new_measure;
  Beat *new_beat;
  Step *new_step;
  Sound *new_sound;
  JsonObject track = doc["track"];
  Serial.println("TRACK MADE");
  // strlcpy(new_track->filename, // <- destination
  //         track["filename"],   // <- source
  //         sizeof(new_track->filename));
  new_track->filename = std::string(track["filename"]);

  Serial.println("TRACK FILENAME");

  new_track->id = track["id"];                           // 4
  new_track->bpm = 50;                                   // 4
  new_track->active_measures = track["active_measures"]; // 1
  new_track->measure_beats = track["measure_beats"];     // 4
  new_track->measure_steps = track["measure_steps"];     // 6
  new_track->current_measure_id = 0;
  Serial.print("id: ");
  Serial.print(new_track->id);
  Serial.print("\ta: ");
  Serial.print(new_track->active_measures);
  Serial.print("\tf: ");
  Serial.println(new_track->filename.c_str());

  new_track->measure_list = new Measure[new_track->active_measures];
  Serial.println("TRACK ARRAY PASSED");
  for (int m = 0; m < new_track->active_measures; m++)
  {
    new_track->measure_list[m] = *measure_create(m);
    new_measure = &(new_track->measure_list[m]);
    JsonObject measure = track["measure_list"][m];

    Serial.println("MEASURE PASSED");
    new_measure->id = measure["id"];                     // 0
    new_measure->active_beats = measure["active_beats"]; // 4
    new_measure->step = 0;
    new_measure->step = 0;
    new_measure->effect_mode = false;
    new_measure->current_step = new_measure->beat_list[0].step_list[0];
    new_measure->prior_step = new_measure->beat_list[3].step_list[5];
    Serial.print("measure id: ");
    Serial.println(new_measure->id);
    Serial.print("measure active_beats: ");
    Serial.println(new_measure->active_beats);
    Serial.println("MEASURE ARRAY PASSED");

    for (int b = 0; b < MAX_BEATS; b++)
    {
      Serial.println("BEAT PASSED");
      JsonObject beat = measure["beat_list"][b];

      new_beat = &(new_measure->beat_list[b]);

      new_beat->id = beat["id"];                     // 0
      new_beat->active_steps = beat["active_steps"]; // 4

      Serial.print("beat id: ");
      Serial.println(new_beat->id);
      Serial.print("beat active_steps: ");
      Serial.println(new_beat->active_steps);
      Serial.println("BEAT ARRAY PASSED");
      for (int s = 0; s < MAX_STEPS; s++)
      {
        Serial.println("STEP PASSED");
        JsonObject step = beat["step_list"][s];
        new_step = &(new_beat->step_list[s]);

        new_step->id = step["id"];                       // 0
        new_step->active_sounds = step["active_sounds"]; // 0

        Serial.print("step id: ");
        Serial.println(new_step->id);
        Serial.print("step active_sounds: ");
        Serial.println(new_step->active_sounds);
        Serial.println("STEP ARRAY PASSED");
        for (int i = 0; i < MAX_STEP_SOUNDS; i++)
        {
          Serial.println("SOUND PASSED");

          JsonObject sound = step["sound_list"][i];

          new_sound = &(new_step->sound_list[i]);

          Sound temp_sound;
          new_sound->bank = sound["bank"];             // 0
          new_sound->instrument = sound["instrument"]; // 0
          new_sound->note = sound["note"];             // 0
          new_sound->filename = std::string(sound["filename"]);
          new_sound->sd_cached_sound = nullptr;
          new_sound->empty = sound["empty"];

          Serial.print("B: ");
          Serial.print(new_sound->bank);
          Serial.print("\tI: ");
          Serial.print(new_sound->instrument);
          Serial.print("\tN: ");
          Serial.print(new_sound->note);
          Serial.print("\tf: ");
          if (!new_sound->filename.empty())
          {
            Serial.print(new_sound->filename.c_str());
          }
          else
          {
            Serial.print("x");
          }
          Serial.print("\tE: ");
          Serial.println(new_sound->empty);
          if (new_sound->filename.empty() != false)
          {
            Serial.println("filename != null");
            btp = b;
            stp = s;
            sp = i;
            already_cached = false;
            auto cache_file_iter = new_track->cached_sounds.begin();
            while (cache_file_iter != new_track->cached_sounds.end())
            {
              Sound cache_file = *cache_file_iter;
              if (new_sound->filename == cache_file.filename)
              {
                already_cached = true;
                Serial.println("filename found in cache");
                new_sound->sd_cached_sound = cache_file.sd_cached_sound;
              }
              cache_file_iter++;
            }

            if (already_cached == false)
            {
              if (find_sd_sound(new_sound->filename))
              {
                new_sound->sd_cached_sound = cache_sd_sound(new_sound->filename.c_str());
                new_track->cached_sounds.push_back(*new_sound);
              }
              else
              {
                new_sound->empty = true;
              }
            }
          }
        }
      }
    }
  }

  file.close();
  Serial.println("here1");
  current_track = new_track;
  Serial.println("here2");
  current_measure = &(current_track->measure_list[current_track->current_measure_id]);
  Serial.println("here3");
  Serial.println("test new track stuff");
  Serial.print("f: ");
  Serial.print(current_track->filename.c_str());
  Serial.print("\tcurrent: ");
  Serial.print(current_track->current_measure_id);
  Serial.print("\tbpm: ");
  Serial.println(current_track->bpm);
  Serial.println("current_measure stuff:");
  Serial.print("id: ");
  Serial.print(current_measure->id);
  Serial.print("\tact: ");
  Serial.println(current_measure->active_beats);

  Serial.print("previous filename: ");
  Serial.println(new_track->measure_list[0].beat_list[btp].step_list[stp].sound_list[sp].filename.c_str());
  Serial.print("new filename: ");
  Serial.println(current_track->measure_list[0].beat_list[btp].step_list[stp].sound_list[sp].filename.c_str());
}

// Saves the configuration to a file
void save_track(std::string filename, Track *config)
{
  std::string full_path = TRACKS_DIRECTORY + filename;

  SD.remove(full_path.c_str());
  File file = SD.open(full_path.c_str(), FILE_WRITE);
  if (!file)
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  JsonDocument doc;
  JsonObject track = doc["track"].to<JsonObject>();
  track["filename"] = config->filename;
  track["id"] = config->id;
  track["active_measures"] = config->active_measures;
  track["measure_beats"] = config->measure_beats;
  track["measure_steps"] = config->measure_steps;
  JsonArray measure_list = track["measure_list"].to<JsonArray>();

  for (int m = 0; m < config->active_measures; m++)
  {
    JsonObject measure = measure_list.add<JsonObject>();
    measure["id"] = config->measure_list[m].id;
    measure["active_beats"] = config->measure_list[m].active_beats;

    JsonArray beat_list = measure["beat_list"].to<JsonArray>();
    for (int b = 0; b < MAX_BEATS; b++)
    {
      JsonObject beat = beat_list.add<JsonObject>();
      beat["id"] = config->measure_list[m].beat_list[b].id;
      beat["active_steps"] = config->measure_list[m].beat_list[b].active_steps;

      JsonArray step_list = beat["step_list"].to<JsonArray>();
      for (int s = 0; s < MAX_STEPS; s++)
      {
        JsonObject step = step_list.add<JsonObject>();
        step["id"] = config->measure_list[m].beat_list[b].step_list[s].id;
        step["active_sounds"] = config->measure_list[m].beat_list[b].step_list[s].active_sounds;

        JsonArray sound_list = step["sound_list"].to<JsonArray>();
        for (int i = 0; i < MAX_STEP_SOUNDS; i++)
        {
          JsonObject sound = sound_list.add<JsonObject>();
          sound["bank"] = config->measure_list[m].beat_list[b].step_list[s].sound_list[i].bank;
          sound["instrument"] = config->measure_list[m].beat_list[b].step_list[s].sound_list[i].instrument;
          sound["note"] = config->measure_list[m].beat_list[b].step_list[s].sound_list[i].note;
          sound["filename"] = config->measure_list[m].beat_list[b].step_list[s].sound_list[i].filename;
          sound["empty"] = config->measure_list[m].beat_list[b].step_list[s].sound_list[i].empty;
        }
      }
    }
  }
  // Serialize JSON to file
  if (serializeJsonPretty(doc, file) == 0)
  // if (serializeJson(doc, file) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
}

int sd_delete_track(const std::string filename)
{
  std::string full_path = TRACKS_DIRECTORY + filename;

  if (SD.remove(full_path.c_str()))
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

bool find_sd_sound(std::string filename)
{
  Serial.println("filename not cached");

  File root = SD.open(CUSTOM_SOUNDS_DIRECTORY);
  Serial.print("checking for: ");
  Serial.print(filename.c_str());
  Serial.print("\tsize: ");
  Serial.println(sizeof(filename));

  while (true)
  {
    File entry = root.openNextFile();
    if (!entry)
    {
      break;
    }
    if (!strncmp(entry.name(), "._", 2))
    {
      continue;
    }
    Serial.print("file: ");
    Serial.print(entry.name());
    Serial.print("\tsize: ");
    Serial.print(sizeof(entry.name()));

    if (strcmp(entry.name(), filename.c_str()) == 0)
    {
      Serial.println("\tEQUAL");
      return true;
    }
    else
    {
      Serial.println("\tNOT EQUAL");
    }
    entry.close();
  }

  Serial.println("file not found on sd");
  root.close();

  return false;
}

#else // unsafe - old
int track_init(void)
{
  Track *new_track = new Track;
  strcpy(new_track->filename, "DEFAULT.json");
  new_track->id = 0;
  new_track->bpm = 120;
  new_track->active_measures = 1;
  new_track->measure_beats = 4;
  new_track->measure_steps = 6;
  new_track->current_measure_id = 0;
  new_track->measure_list = new Measure[new_track->active_measures];

  // Initialize the measure_list
  // Example initialization for the id of each measure
  for (int i = 0; i < new_track->active_measures; i++)
  {
    new_track->measure_list[i] = *measure_create(i);
  }
  // new_track->cached_sounds = new std::deque<struct Sound>;
  current_measure = &(new_track->measure_list[new_track->current_measure_id]);
  current_track = new_track;
  return 0;
}

array_with_size *sd_fetch_sounds(void)
{
  File root = SD.open("/sounds");

  // Initialize a dynamic array to store filenames
  const char **filenames = NULL;
  size_t numFiles = 0;

  // Iterate through the files in the directory
  while (true)
  {
    File entry = root.openNextFile();
    if (!entry)
    {
      break; // No more files
    }

    // Allocate memory for the new filename
    if (!strncmp(entry.name(), "._", 2))
    {
      continue;
    }
    char *filename = strdup(entry.name());

    // Resize the filenames array
    filenames = (const char **)realloc(filenames, (numFiles + 1) * sizeof(char *));

    // Store the filename in the array
    filenames[numFiles] = filename;
    ++numFiles;

    entry.close();
  }

  // Close the "sounds" directory
  root.close();

  // Resize the filenames array to its final size
  filenames = (const char **)realloc(filenames, (numFiles + 1) * sizeof(char *));

  // Set the last element of the array to NULL to indicate the end
  filenames[numFiles] = NULL;

  custom_sound_list->array = filenames;
  custom_sound_list->size = numFiles;
  return custom_sound_list;
}

array_with_size *sd_fetch_tracks(void)
{
  File root = SD.open("/tracks");

  // Initialize a dynamic array to store filenames
  const char **filenames = NULL;
  size_t numFiles = 0;

  // Iterate through the files in the directory
  while (true)
  {
    File entry = root.openNextFile();
    if (!entry)
    {
      break; // No more files
    }

    // Allocate memory for the new filename
    if (!strncmp(entry.name(), "._", 2))
    {
      continue;
    }
    char *filename = strdup(entry.name());

    // Resize the filenames array
    filenames = (const char **)realloc(filenames, (numFiles + 1) * sizeof(char *));

    // Store the filename in the array
    filenames[numFiles] = filename;
    ++numFiles;

    entry.close();
  }

  // Close the "sounds" directory
  root.close();

  // Resize the filenames array to its final size
  filenames = (const char **)realloc(filenames, (numFiles + 1) * sizeof(char *));

  // Set the last element of the array to NULL to indicate the end
  filenames[numFiles] = NULL;

  track_list->array = filenames;
  track_list->size = numFiles;
  return track_list;
}

void read_track(const char *filename, Track *config)
{
  // vector<pair<const char *, newdigate::audiosample *>> cached_files;
  // pair<const char *, newdigate::audiosample *> cached_file;

  bool file_found = false;
  bool already_cached = false;
  // Calculate the length of the string
  size_t filename_len = strlen(filename);

  // Check if the filename length exceeds the buffer size
  if (filename_len >= MAX_FILENAME_LENGTH - 8)
  {
    Serial.println("Filename is too long for buffer");
    return;
  }

  // Copy CUSTOM_SOUNDS_DIRECTORY prefix into temp_str
  strcpy(temp_str, TRACKS_DIRECTORY);
  // Concatenate filename to temp_str
  strcat(temp_str, filename);
  // Open file for reading

  File file = SD.open(temp_str);

  // Allocate a temporary JsonDocument
  JsonDocument doc;

  // Deserialize the JSON document

  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  else
  {
    Serial.println("DESERIALIZE PASSED TRACK");
  }
  int btp = 0;
  int stp = 0;
  int sp = 0;
  Track *new_track = new Track;
  Measure *new_measure;
  Beat *new_beat;
  Step *new_step;
  Sound *new_sound;
  JsonObject track = doc["track"];
  Serial.println("TRACK MADE");
  strlcpy(new_track->filename, // <- destination
          track["filename"],   // <- source
          sizeof(new_track->filename));
  Serial.println("TRACK FILENAME");

  new_track->id = track["id"];                           // 4
  new_track->bpm = 50;                                   // 4
  new_track->active_measures = track["active_measures"]; // 1
  new_track->measure_beats = track["measure_beats"];     // 4
  new_track->measure_steps = track["measure_steps"];     // 6
  new_track->current_measure_id = 0;
  Serial.print("id: ");
  Serial.print(new_track->id);
  Serial.print("\ta: ");
  Serial.print(new_track->active_measures);
  Serial.print("\tf: ");
  Serial.println(new_track->filename);

  new_track->measure_list = new Measure[new_track->active_measures];
  Serial.println("TRACK ARRAY PASSED");
  for (int m = 0; m < new_track->active_measures; m++)
  {
    new_track->measure_list[m] = *measure_create(m);
    new_measure = &(new_track->measure_list[m]);
    JsonObject measure = track["measure_list"][m];

    Serial.println("MEASURE PASSED");
    new_measure->id = measure["id"];                     // 0
    new_measure->active_beats = measure["active_beats"]; // 4
    new_measure->step = 0;
    new_measure->step = 0;
    new_measure->effect_mode = false;
    new_measure->current_step = new_measure->beat_list[0].step_list[0];
    new_measure->prior_step = new_measure->beat_list[3].step_list[5];
    Serial.print("measure id: ");
    Serial.println(new_measure->id);
    Serial.print("measure active_beats: ");
    Serial.println(new_measure->active_beats);
    Serial.println("MEASURE ARRAY PASSED");

    for (int b = 0; b < MAX_BEATS; b++)
    {
      Serial.println("BEAT PASSED");
      JsonObject beat = measure["beat_list"][b];

      new_beat = &(new_measure->beat_list[b]);

      new_beat->id = beat["id"];                     // 0
      new_beat->active_steps = beat["active_steps"]; // 4

      Serial.print("beat id: ");
      Serial.println(new_beat->id);
      Serial.print("beat active_steps: ");
      Serial.println(new_beat->active_steps);
      Serial.println("BEAT ARRAY PASSED");
      for (int s = 0; s < MAX_STEPS; s++)
      {
        Serial.println("STEP PASSED");
        JsonObject step = beat["step_list"][s];
        new_step = &(new_beat->step_list[s]);

        new_step->id = step["id"];                       // 0
        new_step->active_sounds = step["active_sounds"]; // 0

        Serial.print("step id: ");
        Serial.println(new_step->id);
        Serial.print("step active_sounds: ");
        Serial.println(new_step->active_sounds);
        Serial.println("STEP ARRAY PASSED");
        for (int i = 0; i < MAX_STEP_SOUNDS; i++)
        {
          Serial.println("SOUND PASSED");

          JsonObject sound = step["sound_list"][i];

          new_sound = &(new_step->sound_list[i]);

          Sound temp_sound;
          new_sound->bank = sound["bank"];             // 0
          new_sound->instrument = sound["instrument"]; // 0
          new_sound->note = sound["note"];             // 0
          // new_sound->filename = sound["filename"];     //
          new_sound->filename = sound["filename"].as<const char *>();
          // strncpy(new_sound->filename, // <- destination
          //         sound["filename"],   // <- source
          //         FILENAME_MAX_SIZE - 1);
          new_sound->sd_cached_sound = nullptr;
          new_sound->empty = sound["empty"];

          Serial.print("B: ");
          Serial.print(new_sound->bank);
          Serial.print("\tI: ");
          Serial.print(new_sound->instrument);
          Serial.print("\tN: ");
          Serial.print(new_sound->note);
          Serial.print("\tf: ");
          if (new_sound->filename != nullptr)
          {
            Serial.print(new_sound->filename);
          }
          else
          {
            Serial.print("x");
          }
          Serial.print("\tE: ");
          Serial.println(new_sound->empty);
          if (new_sound->filename != nullptr)
          {
            Serial.println("filename != null");
            btp = b;
            stp = s;
            sp = i;
            already_cached = false;
            auto cache_file_iter = new_track->cached_sounds.begin();
            while (cache_file_iter != new_track->cached_sounds.end())
            {
              Sound cache_file = *cache_file_iter;
              if (strcmp(new_sound->filename, cache_file.filename) == 0)
              {
                already_cached = true;
                Serial.println("filename found in cache");
                new_sound->sd_cached_sound = cache_file.sd_cached_sound;
              }
              cache_file_iter++;
            }

            if (already_cached == false)
            {
              if (find_sd_sound(new_sound->filename))
              {
                new_sound->sd_cached_sound = cache_sd_sound(new_sound->filename);
                new_track->cached_sounds.push_back(*new_sound);
              }
              else
              {
                new_sound->empty = true;
              }
            }
          }
        }
      }
    }
  }

  file.close();
  Serial.println("here1");
  current_track = new_track;
  Serial.println("here2");
  current_measure = &(current_track->measure_list[current_track->current_measure_id]);
  Serial.println("here3");
  Serial.println("test new track stuff");
  Serial.print("f: ");
  Serial.print(current_track->filename);
  Serial.print("\tcurrent: ");
  Serial.print(current_track->current_measure_id);
  Serial.print("\tbpm: ");
  Serial.println(current_track->bpm);
  Serial.println("current_measure stuff:");
  Serial.print("id: ");
  Serial.print(current_measure->id);
  Serial.print("\tact: ");
  Serial.println(current_measure->active_beats);

  Serial.print("previous filename: ");
  Serial.println(new_track->measure_list[0].beat_list[btp].step_list[stp].sound_list[sp].filename);
  Serial.print("new filename: ");
  Serial.println(current_track->measure_list[0].beat_list[btp].step_list[stp].sound_list[sp].filename);
}

// Saves the configuration to a file
void save_track(const char *filename, Track *config)
{

  // Calculate the length of the string
  size_t filename_len = strlen(filename);

  // Check if the filename length exceeds the buffer size
  if (filename_len >= MAX_FILENAME_LENGTH - 8)
  {
    Serial.println("Filename is too long for buffer");
    return;
  }
  // Copy CUSTOM_SOUNDS_DIRECTORY prefix into temp_str
  strcpy(temp_str, TRACKS_DIRECTORY);
  // Concatenate filename to temp_str
  strcat(temp_str, filename);
  // Delete existing file, otherwise the configuration is appended to the file
  SD.remove(temp_str);

  // Open file for writing
  File file = SD.open(temp_str, FILE_WRITE);
  if (!file)
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  JsonDocument doc;
  JsonObject track = doc["track"].to<JsonObject>();
  track["filename"] = config->filename;
  track["id"] = config->id;
  track["active_measures"] = config->active_measures;
  track["measure_beats"] = config->measure_beats;
  track["measure_steps"] = config->measure_steps;
  JsonArray measure_list = track["measure_list"].to<JsonArray>();

  for (int m = 0; m < config->active_measures; m++)
  {
    JsonObject measure = measure_list.add<JsonObject>();
    measure["id"] = config->measure_list[m].id;
    measure["active_beats"] = config->measure_list[m].active_beats;

    JsonArray beat_list = measure["beat_list"].to<JsonArray>();
    for (int b = 0; b < MAX_BEATS; b++)
    {
      JsonObject beat = beat_list.add<JsonObject>();
      beat["id"] = config->measure_list[m].beat_list[b].id;
      beat["active_steps"] = config->measure_list[m].beat_list[b].active_steps;

      JsonArray step_list = beat["step_list"].to<JsonArray>();
      for (int s = 0; s < MAX_STEPS; s++)
      {
        JsonObject step = step_list.add<JsonObject>();
        step["id"] = config->measure_list[m].beat_list[b].step_list[s].id;
        step["active_sounds"] = config->measure_list[m].beat_list[b].step_list[s].active_sounds;

        JsonArray sound_list = step["sound_list"].to<JsonArray>();
        for (int i = 0; i < MAX_STEP_SOUNDS; i++)
        {
          JsonObject sound = sound_list.add<JsonObject>();
          sound["bank"] = config->measure_list[m].beat_list[b].step_list[s].sound_list[i].bank;
          sound["instrument"] = config->measure_list[m].beat_list[b].step_list[s].sound_list[i].instrument;
          sound["note"] = config->measure_list[m].beat_list[b].step_list[s].sound_list[i].note;
          sound["filename"] = config->measure_list[m].beat_list[b].step_list[s].sound_list[i].filename;
          sound["empty"] = config->measure_list[m].beat_list[b].step_list[s].sound_list[i].empty;
        }
      }
    }
  }
  // Serialize JSON to file
  if (serializeJsonPretty(doc, file) == 0)
  // if (serializeJson(doc, file) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
}

int sd_delete_track(const char *filename)
{

  // Calculate the length of the string
  size_t filename_len = strlen(filename);

  // Check if the filename length exceeds the buffer size
  if (filename_len >= MAX_FILENAME_LENGTH - 8)
  {
    Serial.println("Filename is too long for buffer");
    return 1;
  }
  // Copy CUSTOM_SOUNDS_DIRECTORY prefix into temp_str
  strcpy(temp_str, TRACKS_DIRECTORY);
  // Concatenate filename to temp_str
  strcat(temp_str, filename);
  // Delete existing file, otherwise the configuration is appended to the file
  if (SD.remove(temp_str))
  {
    return 0;
  }
  return 1;
}

bool find_sd_sound(const char *filename)
{
  Serial.println("filename not cached");

  File root = SD.open(CUSTOM_SOUNDS_DIRECTORY);
  Serial.print("checking for: ");
  Serial.print(filename);
  Serial.print("\tsize: ");
  Serial.println(sizeof(filename));

  while (true)
  {
    File entry = root.openNextFile();
    if (!entry)
    {
      break;
    }
    if (!strncmp(entry.name(), "._", 2))
    {
      continue;
    }
    Serial.print("file: ");
    Serial.print(entry.name());
    Serial.print("\tsize: ");
    Serial.print(sizeof(entry.name()));

    if (strcmp(entry.name(), filename) == 0)
    {
      Serial.println("\tEQUAL");
      return true;
    }
    else
    {
      Serial.println("\tNOT EQUAL");
    }
    entry.close();
  }

  Serial.println("file not found on sd");
  root.close();

  return false;
}

#endif

// Function to free the memory allocated for the array of strings
void freeArrayOfStrings(char **stringArray, size_t numStrings)
{
  for (size_t i = 0; i < numStrings; ++i)
  {
    free(stringArray[i]);
  }

  free(stringArray);
}

void listfiles()
{
  const int chipSelect = BUILTIN_SDCARD;
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect.
  }

  // Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect))
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  File root = SD.open("/sounds");

  printDirectory(root, 0);

  Serial.println("done!");
}

void printDirectory(File dir, int numSpaces)
{
  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry)
    {
      // Serial.println("** no more files **");
      break;
    }
    printSpaces(numSpaces);
    // Serial.print(entry.name());
    if (entry.isDirectory())
    {
      Serial.println("/");
      printDirectory(entry, numSpaces + 2);
    }
    else
    {
      // files have sizes, directories do not
      int n = log10f(entry.size());
      if (n < 0)
        n = 10;
      if (n > 10)
        n = 10;
      printSpaces(50 - numSpaces - strlen(entry.name()) - n);
      // Serial.print("  ");
      // Serial.print(entry.size(), DEC);
      DateTimeFields datetime;
      if (entry.getModifyTime(datetime))
      {
        printSpaces(4);
        printTime(datetime);
      }
      Serial.println();
    }
    entry.close();
  }
}

void printSpaces(int num)
{
  for (int i = 0; i < num; i++)
  {
    // Serial.print(" ");
  }
}

void printTime(const DateTimeFields tm)
{
  const char *months[12] = {
      "January", "February", "March", "April", "May", "June",
      "July", "August", "September", "October", "November", "December"};
  if (tm.hour < 10)
    Serial.print('0');
  Serial.print(tm.hour);
  Serial.print(':');
  if (tm.min < 10)
    Serial.print('0');
  Serial.print(tm.min);
  Serial.print("  ");
  Serial.print(tm.mon < 12 ? months[tm.mon] : "???");
  Serial.print(" ");
  Serial.print(tm.mday);
  Serial.print(", ");
  Serial.print(tm.year + 1900);
}

// Prints the content of a file to the Serial
void print_JSON(const char *filename)
{

  // Calculate the length of the string
  size_t filename_len = strlen(filename);

  // Check if the filename length exceeds the buffer size
  if (filename_len >= MAX_FILENAME_LENGTH - 8)
  {
    Serial.println("Filename is too long for buffer");
    return;
  }
  // Copy CUSTOM_SOUNDS_DIRECTORY prefix into temp_str
  strcpy(temp_str, TRACKS_DIRECTORY);
  // Concatenate filename to temp_str
  strcat(temp_str, filename);
  // Open file for reading
  File file = SD.open(temp_str);
  if (!file)
  {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available())
  {
    // Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}

int free_track(Track *track)
{
  for (int m = 0; m < track->active_measures; m++)
  {
    for (int b = 0; b < MAX_BEATS; b++)
    {
      for (int s = 0; s < MAX_STEPS; s++)
      {

        for (int i = 0; i < MAX_STEP_SOUNDS; i++)
        {
          Serial.print("deleting: i=");
          Serial.println(i);
          delete &(track->measure_list[m].beat_list[b].step_list[s].sound_list[i]);
        }
        Serial.print("deleting: s=");
        Serial.println(s);

        delete &(track->measure_list[m].beat_list[b].step_list[s]);
      }
      Serial.print("deleting: b=");
      Serial.println(b);
      delete &(track->measure_list[m].beat_list[b]);
    }
    Serial.print("deleting: m=");
    Serial.println(m);
    delete &(track->measure_list[m]);
  }

  return 0;
}
