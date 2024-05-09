/*
 This code will blink an LED attached to pin 13 on and off. 
 It will stay on for 0.25 seconds.
 It will stay off for 1 second.
 */
#include <Metro.h> //Include Metro library
/*#define LED1 22 
#define LED2 21
#define LED3 20
#define LED4 19

#define LED5 18 
#define LED6 17
#define LED7 16
#define LED8 15

#define LED9 8 
#define LED10 9
#define LED11 10
#define LED12 11
*/

/* VS1053b Setup */
#define VS1053 1
#define VS1053_RST 2

#define DEFAULT 0x00
#define Melody 0x79
#define Drums1 0x78 // Appears as if Drums1 & Drums2 are the same bank at separate addresses
#define Drums2 0x7F


#define NoteOn 0x90
#define NoteOff 0x80
#define MESSAGE 0xB0
#define BANK 0x00
#define VOLUME 0x07
#define PROGRAM 0xC0

#define OFF1 0x7B
#define OFF2 0x7C
#define OFF3 0x7D

//Start Channel, MIDI doesn't use rx pin
#define MIDI Serial1

// Define Midi Sound Addresses
#define Snare 38
#define Kick 35
#define HiHat 42
#define Crash 49

//Create a variable to hold the led's current state
int count = 0;

// Instanciate a metro object and set the interval to 250 milliseconds (0.25 seconds).
Metro ledMetro = Metro(250); 

void setup()
{
  /*pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT); 
  pinMode(LED3,OUTPUT); 
  pinMode(LED4,OUTPUT); 

  pinMode(LED5,OUTPUT);
  pinMode(LED6,OUTPUT); 
  pinMode(LED7,OUTPUT); 
  pinMode(LED8,OUTPUT);
  */
  Serial.begin(9600);
  MIDI.begin(31250); // Midi serial

  pinMode(VS1053_RST, OUTPUT);
  digitalWrite(VS1053_RST, LOW);
  delay(10);
  digitalWrite(VS1053_RST, HIGH);
  delay(10);

  midiSetChannelVolume(0, 127); // Default Volume Select
  midiSetChannelBank(0, Drums1); // Percussive Bank Select
  //delay(10000);
}

void loop()
{
  /***************************************** 
  PUT THIS LINE AT THE TOP OF ALL PERCUSSIVE SOUND BLOCKS SO IT WILL 
  START AS A PERCUSION SOUND:
  midiSetInstrument(0,128); 
  *****************************************/
  midiSetInstrument(0,128); 

  if (ledMetro.check() == 1) {
    if (count == 0){
      //digitalWrite(LED12,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED1,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Kick, 127);
      midiNoteOn(0, Crash, 127);
    }
    if (count == 1){
      //digitalWrite(LED1,LOW);
      //midiNoteOff(0, Kick, 127);
      //digitalWrite(LED2,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 2){
      //digitalWrite(LED2,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED3,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Kick, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 3){
      //digitalWrite(LED3,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED4,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 4){
      //digitalWrite(LED4,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED5,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Snare, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 5){
      //digitalWrite(LED5,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED6,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 6){
      //digitalWrite(LED6,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED7,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Kick, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 7){
      //digitalWrite(LED7,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED8,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 8){
      //digitalWrite(LED8,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED9,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Kick, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 9){
      //digitalWrite(LED9,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED10,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 10){
      //digitalWrite(LED10,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED11,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Snare, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 11){
      //digitalWrite(LED11,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED12,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 12){
      //digitalWrite(LED12,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED1,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Kick, 127);
      //midiNoteOn(0, Crash, 127);
    }
    if (count == 13){
      //digitalWrite(LED1,LOW);
      //midiNoteOff(0, Kick, 127);
      //digitalWrite(LED2,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 14){
      //digitalWrite(LED2,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED3,HIGH);
      //midiNoteOn(0, 38, 127);
      //midiNoteOn(0, Kick, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 15){
      //digitalWrite(LED3,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED4,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Kick, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 16){
      //digitalWrite(LED4,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED5,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Snare, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 17){
      //digitalWrite(LED5,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED6,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 18){
      //digitalWrite(LED6,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED7,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Kick, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 19){
      //digitalWrite(LED7,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED8,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 20){
      //digitalWrite(LED8,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED9,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Kick, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 21){
      //digitalWrite(LED9,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED10,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 22){
      //digitalWrite(LED10,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED11,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, Snare, 127);
      midiNoteOn(0, HiHat, 127);
    }
    if (count == 23){
      //digitalWrite(LED11,LOW);
      //midiNoteOff(0, 38, 127);
      //digitalWrite(LED12,HIGH);
      //midiNoteOn(0, 38, 127);
      midiNoteOn(0, HiHat, 127);
    }
    count++;
    if (count == 24){
      count = 0;
    }
  }
  //delay(1);
}

void check(int instrument, int intensity) {
  midiNoteOn(0, instrument, intensity);
  delay(125);
  midiNoteOff(0, instrument, intensity);
  midiNoteOn(0, instrument, intensity);
  delay(125);
  midiNoteOff(0, instrument, intensity);
  midiNoteOn(0, instrument, intensity);
  delay(125);
  midiNoteOff(0, instrument, intensity);
  midiNoteOn(0, instrument, intensity);
  delay(125);
  midiNoteOff(0, instrument, intensity);
}

void check1(int instrument, int intensity) {
  delay(125);
  midiNoteOn(0, instrument, intensity);
  delay(125);
  midiNoteOff(0, instrument, intensity);
  midiNoteOn(0, instrument, intensity);
  delay(125);
  midiNoteOff(0, instrument, intensity);
  midiNoteOn(0, instrument, intensity);
  delay(125);
  midiNoteOff(0, instrument, intensity);
}

void check2(int instrument, int intensity) {
  
}

void midiSetInstrument(uint8_t chan, uint8_t inst) {
  if (chan > 15) return;
  inst --; // page 32 has instruments starting with 1 not 0 :(
  if (inst > 127) return;
  
  MIDI.write(PROGRAM | chan);  
  MIDI.write(inst);
}


void midiSetChannelVolume(uint8_t chan, uint8_t vol) {
  if (chan > 15) return;
  if (vol > 127) return;
  
  MIDI.write(MESSAGE | chan);
  MIDI.write(VOLUME);
  MIDI.write(vol);
}

void midiSetChannelBank(uint8_t chan, uint8_t bank) {
  if (chan > 15) return;
  if (bank > 127) return;
  
  MIDI.write(MESSAGE | chan);
  MIDI.write((uint8_t)BANK);
  MIDI.write(bank);
  Serial.println("Bank change");
  Serial.println(bank);
}

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  MIDI.write(NoteOn | chan);
  MIDI.write(n);
  MIDI.write(vel);
}

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  MIDI.write(NoteOff | chan);
  MIDI.write(n);
  MIDI.write(vel);
}

