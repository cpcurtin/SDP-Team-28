/*
 *       SD card function library
 *
 *
 *
 */
#include <Arduino.h>
// #include <SD.h>
#include "sd-storage-module.h"

void sd_init(void)
{
  const int chipSelect = BUILTIN_SDCARD;

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect))
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // File root = SD.open("/");
}

struct array_with_size *parsefiles(void)
{
  File root = SD.open("/sounds");

  // Check if the directory is open
  // if (!root)
  // {
  //   Serial.println("Failed to open directory");
  //   return NULL; // Return NULL in case of failure
  // }

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

  // for (size_t i = 0; i < numFiles; i++) {
  // Serial.print(filenames[2]);
  //       Serial.println(); // Print a newline after each filename
  //   }
  // char **state = new char *[2];
  struct array_with_size *sound_list = new struct array_with_size;

  sound_list->array = filenames;
  sound_list->size = numFiles;
  // sound_list->lcd_state = state;
  // sound_list->index = 0;

  // char *name;
  //  char **ptr_str_array;
  //  struct lcd_nav **parent;
  //  struct lcd_nav **child;
  //  size_t size;
  //  char **lcd_state;
  //  int index;
  //  int depth;
  // struct lcd_nav *custom_sounds = new struct lcd_nav;
  // custom_sounds->ptr_str_array = filenames;
  // custom_sounds->size = numFiles;
  // custom_sounds->lcd_state = state;
  // custom_sounds->index = 0;

  return sound_list;
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

  Serial.print("Initializing SD card...");

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
    Serial.print(entry.name());
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
      Serial.print("  ");
      Serial.print(entry.size(), DEC);
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
    Serial.print(" ");
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

// save current program to card at selected location
void saveTracks(struct track singleTrack)
{
  if (SD.exists(fileName))
  {

    Serial.println("example.txt exists.");
  }
  else
  {

    Serial.println("example.txt doesn't exist.");
  }

  // open a new file and immediately close it:

  Serial.println("Creating example.txt...");

  File dataFile = SD.open(fileName, FILE_WRITE);

  dataFile.close();

  // Check to see if the file exists:

  if (SD.exists(fileName))
  {

    Serial.println("example.txt exists.");
  }
  else
  {

    Serial.println("example.txt doesn't exist.");
  }

  // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  //  dataFile = SD.open(fileName, FILE_WRITE);
  //  if (dataFile) {
  //     // Write the single track to the file
  //     dataFile.write((uint8_t*)&singleTrack, sizeof(track));
  //     // Close the file
  //     Serial.println("Track saved successfully");

  //     // while (dataFile.available()) {
  //     //   Serial.write(dataFile.read());
  //     // }
  //   } else {
  //     // If the file didn't open, print an error message
  //     Serial.println("Error opening data.txt");
  //   }
  //   dataFile.close();
  // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

  // delete the file:

  Serial.println("Removing example.txt...");

  SD.remove(fileName);

  if (SD.exists(fileName))
  {

    Serial.println("example.txt exists.");
  }
  else
  {

    Serial.println("example.txt doesn't exist.");
  }
  Serial.println("fin");
}

void read_track(const char *filename, struct track &config)
{
  // Calculate the length of the string
  size_t filename_len = strlen(filename);

  // Check if the filename length exceeds the buffer size
  if (filename_len >= MAX_FILENAME_LENGTH - 8)
  {
    Serial.println("Filename is too long for buffer");
    return;
  }
  Serial.println("hi");
  // Copy "/sounds/" prefix into temp_str
  strcpy(temp_str, "/tracks/");
  // Concatenate filename to temp_str
  strcat(temp_str, filename);
  // Open file for reading
  Serial.println("hi2");
  File file = SD.open(temp_str);
  Serial.println("hi3");

  // Allocate a temporary JsonDocument
  JsonDocument doc;

  // Deserialize the JSON document
  Serial.println("hi4");
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));
  Serial.println("hi5");
  // Copy values from the JsonDocument to the Config
  config.bpm = doc["bpm"];
  strlcpy(config.name,          // <- destination
          doc["name"],          // <- source
          sizeof(config.name)); // <- destination's capacity
  Serial.println("hi6");
  file.close();
}

// Saves the configuration to a file
void save_track(const char *filename, struct track &config)
{

  // Calculate the length of the string
  size_t filename_len = strlen(filename);

  // Check if the filename length exceeds the buffer size
  if (filename_len >= MAX_FILENAME_LENGTH - 8)
  {
    Serial.println("Filename is too long for buffer");
    return;
  }
  // Copy "/sounds/" prefix into temp_str
  strcpy(temp_str, "/tracks/");
  // Concatenate filename to temp_str
  strcat(temp_str, filename);
  // Delete existing file, otherwise the configuration is appended to the file
  SD.remove(filename);

  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file)
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  JsonDocument doc;

  // Set the values in the document
  doc["name"] = config.name;
  doc["bpm"] = config.bpm;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0)
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
  // Copy "/sounds/" prefix into temp_str
  strcpy(temp_str, "/tracks/");
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
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}
