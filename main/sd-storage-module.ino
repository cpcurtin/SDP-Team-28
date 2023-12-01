/*
 *       SD card function library
 *
 *
 *
 */
#include <Arduino.h>
#include <SD.h>
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
  char **filenames = NULL;
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
    filenames = (char **)realloc(filenames, (numFiles + 1) * sizeof(char *));

    // Store the filename in the array
    filenames[numFiles] = filename;
    ++numFiles;

    entry.close();
  }

  // Close the "sounds" directory
  root.close();

  // Resize the filenames array to its final size
  filenames = (char **)realloc(filenames, (numFiles + 1) * sizeof(char *));

  // Set the last element of the array to NULL to indicate the end
  filenames[numFiles] = NULL;

  // for (size_t i = 0; i < numFiles; i++) {
  // Serial.print(filenames[2]);
  //       Serial.println(); // Print a newline after each filename
  //   }
  char **state = new char *[2];
  struct array_with_size *sound_list = new struct array_with_size;
  sound_list->arr = filenames;
  sound_list->size = numFiles;
  sound_list->lcd_state = state;
  sound_list->index = 0;
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
