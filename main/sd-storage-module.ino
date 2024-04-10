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
  const int chipSelect = BUILTIN_SDCARD;

  // Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect))
  {
    Serial.println("initialization failed!");
    return 1;
  }
  Serial.println("initialization done.");

  // File root = SD.open("/");
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

void read_track(const char *filename, Track &config)
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

  // Allocate a temporary JsonDocument
  JsonDocument doc;

  // Deserialize the JSON document

  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
  strlcpy(config.filename, // <- destination
          doc["filename"], // <- source
          sizeof(config.filename));
  config.id = doc["id"];
  config.bpm = doc["bpm"];
  config.measure_steps = doc["measure_steps"];

  file.close();
}

// Saves the configuration to a file
void save_track(const char *filename, Track &config)
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

  // Set the values in the document
  // doc["filename"] = config.filename;
  // doc["id"] = config.id;
  // doc["bpm"] = config.bpm;
  // doc["measure_steps"] = config.measure_steps;
  /*===================================================================================================================================================================*/
  JsonObject track = doc["track"].to<JsonObject>();
  track["filename"] = config.filename;
  track["id"] = config.id;
  track["active_measures"] = config.active_measures;
  track["measure_beats"] = config.measure_beats;
  track["measure_steps"] = config.measure_steps;
  JsonArray measure_list = track["measure_list"].to<JsonArray>();

  for (int m = 0; m < config.active_measures; m++)
  {
    JsonObject measure = track["measure"].add<JsonObject>();
    measure["id"] = config.measure_list[m].id;
    measure["active_beats"] = config.measure_list[m].active_beats;
    measure["step"] = config.measure_list[m].step;
    measure["beat"] = config.measure_list[m].beat;
    measure["effect_mode"] = config.measure_list[m].effect_mode;

    JsonObject current_step = measure["current_step"].add<JsonObject>();
    current_step["id"] = config.measure_list[m].current_step.id;
    current_step["active_sounds"] = config.measure_list[m].current_step.active_sounds;
    JsonArray current_step_sound_list = current_step["sound_list"].to<JsonArray>();
    for (int i = 0; i < MAX_STEP_SOUNDS; i++)
    {
      JsonObject current_step_sound_list_sound = current_step_sound_list.add<JsonObject>();
      current_step_sound_list_sound["bank"] = config.measure_list[m].current_step.sound_list[i].bank;
      current_step_sound_list_sound["instrument"] = config.measure_list[m].current_step.sound_list[i].instrument;
      current_step_sound_list_sound["note"] = config.measure_list[m].current_step.sound_list[i].note;
      current_step_sound_list_sound["filename"] = config.measure_list[m].current_step.sound_list[i].filename;
      current_step_sound_list_sound["empty"] = config.measure_list[m].current_step.sound_list[i].empty;
    }

    JsonObject prior_step = measure["prior_step"].add<JsonObject>();
    prior_step["id"] = config.measure_list[m].prior_step.id;
    prior_step["active_sounds"] = config.measure_list[m].prior_step.active_sounds;
    JsonArray sound_list_prior_step = prior_step["sound_list"].to<JsonArray>();
    for (int i = 0; i < MAX_STEP_SOUNDS; i++)
    {
      JsonObject sound_list_prior_step_sound = sound_list_prior_step.add<JsonObject>();
      sound_list_prior_step_sound["bank"] = config.measure_list[m].prior_step.sound_list[i].bank;
      sound_list_prior_step_sound["instrument"] = config.measure_list[m].prior_step.sound_list[i].instrument;
      sound_list_prior_step_sound["note"] = config.measure_list[m].prior_step.sound_list[i].note;
      sound_list_prior_step_sound["filename"] = config.measure_list[m].prior_step.sound_list[i].filename;
      sound_list_prior_step_sound["empty"] = config.measure_list[m].prior_step.sound_list[i].empty;
    }

    JsonArray beat_list = measure["beat_list"].to<JsonArray>();
    for (int b = 0; b < MAX_BEATS; b++)
    {
      JsonObject beat = beat_list.add<JsonObject>();
      beat["id"] = config.measure_list[m].beat_list[b].id;
      beat["active_steps"] = config.measure_list[m].beat_list[b].active_steps;

      JsonArray step_list = beat["step_list"].to<JsonArray>();
      for (int s = 0; s < MAX_STEPS; s++)
      {
        JsonObject step = step_list.add<JsonObject>();
        step["id"] = config.measure_list[m].beat_list[b].step_list[s].id;
        step["active_steps"] = config.measure_list[m].beat_list[b].step_list[s].active_sounds;

        JsonArray sound_list = step["sound_list"].to<JsonArray>();
        for (int i = 0; i < MAX_STEP_SOUNDS; i++)
        {
          JsonObject sound = sound_list.add<JsonObject>();
          sound["bank"] = config.measure_list[m].beat_list[b].step_list[s].sound_list[i].bank;
          sound["instrument"] = config.measure_list[m].beat_list[b].step_list[s].sound_list[i].instrument;
          sound["note"] = config.measure_list[m].beat_list[b].step_list[s].sound_list[i].note;
          sound["filename"] = config.measure_list[m].beat_list[b].step_list[s].sound_list[i].filename;
          sound["empty"] = config.measure_list[m].beat_list[b].step_list[s].sound_list[i].empty;
        }
      }
    }
  }
  /*===================================================================================================================================================================*/

  // Serialize JSON to file
  // if (serializeJsonPretty(doc, file) == 0)
  // if (serializeJson(doc, file) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file
  file.close();
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