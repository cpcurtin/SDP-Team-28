#ifndef SD_STORAGE_MODULE_H
#define SD_STORAGE_MODULE_H 
#include <Arduino.h>
#include <SD.h>

struct array_with_size {
    char** arr;
    size_t size;
    char** lcd_state;
    int index;
};

void sd_init(void);
struct array_with_size *parsefiles(void);
void freeArrayOfStrings(char **stringArray, size_t numStrings);
void listfiles(void);
void printDirectory(File dir, int numSpaces);
void printSpaces(int num);
void printTime(const DateTimeFields tm);

#endif // SD_STORAGE_MODULE_H