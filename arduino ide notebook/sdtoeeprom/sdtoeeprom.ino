/*
 TESTING TIME TO SAVE SD WAV TO EEPROM 
 */
#include <SD.h>
#include <EEPROM.h>
// change this to match your SD shield or module;
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
// Wiz820+SD board: pin 4
// Teensy audio board: pin 10
// Teensy 3.5 & 3.6 & 4.1 on-board: BUILTIN_SDCARD
const int chipSelect = BUILTIN_SDCARD;
const int eepromStartAddress = 0; // Start address in EEPROM

void setup()
{
  //Uncomment these lines for Teensy 3.x Audio Shield (Rev C)
  //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
  //SPI.setSCK(14);  // Audio shield has SCK on pin 14  
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  int address = eepromStartAddress; // Declare 'address' here
   while (!Serial) {
    ; // wait for serial port to connect.
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // File root = SD.open("/");
  
  // printDirectory(root, 0);
  // sdToEeprom(root, 0);

    // Record the start time
  unsigned long startTime = millis();
  
// Open the input .wav file for reading
  File inputFile = SD.open("freesound.wav", FILE_READ);
  if (!inputFile) {
    Serial.println("Failed to open input.wav!");
    return;
  }


  // Write the file to EEPROM
  while (inputFile.available() && address < EEPROM.length()) {
    char data = inputFile.read();
    EEPROM.write(address, data);
    address++;
  }

// Record the end time
  unsigned long endTime = millis();

  Serial.println("File copied to EEPROM successfully!");

  // Calculate and print the elapsed time
  unsigned long transferTime = endTime - startTime;
  Serial.print("Transfer time (ms): ");
  Serial.println(transferTime);
  Serial.println("File copied to EEPROM successfully!");

// Read and print the first 44 bytes from EEPROM
// int headerSize = 44; // .wav file header size
address = eepromStartAddress; // Reset 'address' to the start
while (address < EEPROM.length()) {
  char data = EEPROM.read(address);
  Serial.write(data); // Print the data to the serial monitor
  address++;
}


  Serial.println("\nPROGRAM COMPLETE");

}

void loop()
{
  // nothing happens after setup finishes.
}

void sdToEeprom(File dir, int numSpaces){
  const char *findName="heart-on-my-sleave-ai.wav";
  // Serial.print(findName);  
   while(true) {
    File entry = dir.openNextFile();
     if (! entry) {
       //Serial.println("** no more files **");
       break;
     }
     if (entry.name()==findName){
    Serial.print("\nHELLO\n");  
     }
    Serial.print(entry.name());
    entry.close();
   
}
}

void printDirectory(File dir, int numSpaces) {
   while(true) {
     File entry = dir.openNextFile();
     if (! entry) {
       //Serial.println("** no more files **");
       break;
     }
     printSpaces(numSpaces);
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numSpaces+2);
     } else {
       // files have sizes, directories do not
       int n = log10f(entry.size());
       if (n < 0) n = 10;
       if (n > 10) n = 10;
       printSpaces(50 - numSpaces - strlen(entry.name()) - n);
       Serial.print("  ");
       Serial.print(entry.size(), DEC);
       DateTimeFields datetime;
       if (entry.getModifyTime(datetime)) {
         printSpaces(4);
         printTime(datetime);
       }
       Serial.println();
     }
     entry.close();
   }
}

void printSpaces(int num) {
  for (int i=0; i < num; i++) {
    Serial.print(" ");
  }
}

void printTime(const DateTimeFields tm) {
  const char *months[12] = {
    "January","February","March","April","May","June",
    "July","August","September","October","November","December"
  };
  if (tm.hour < 10) Serial.print('0');
  Serial.print(tm.hour);
  Serial.print(':');
  if (tm.min < 10) Serial.print('0');
  Serial.print(tm.min);
  Serial.print("  ");
  Serial.print(tm.mon < 12 ? months[tm.mon] : "???");
  Serial.print(" ");
  Serial.print(tm.mday);
  Serial.print(", ");
  Serial.print(tm.year + 1900);
}
