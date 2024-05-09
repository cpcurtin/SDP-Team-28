/*************************************************** 
Shawn Colby 

This program is used to test the functionality of the
palette to measure matrix transfer of MIDI sounds 
 ****************************************************/


#include <Metro.h>

#define Crash1 49
#define ClosedHiHat 42
#define AcousticBassDrum 35
#define ElectricGuitarClean 28

#define LED1 7
#define LED2 8
#define LED3 10

#define LED4 3
#define LED5 11
#define BUTTON1 15
#define BUTTON2 17
#define BUTTON3 16

// define the pins used
#define VS1053_RX  1 // This is the pin that connects to the RX pin on VS1053
//#define VS1053_MIDI Serial1
#define VS1053_RESET 9 // This is the pin that connects to the RESET pin on VS1053
// If you have the Music Maker shield, you don't need to connect the RESET pin!

// If you're using the VS1053 breakout:
// Don't forget to connect the GPIO #0 to GROUND and GPIO #1 pin to 3.3V
// If you're using the Music Maker shield:
// Don't forget to connect the GPIO #1 pin to 3.3V and the RX pin to digital #2

// See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 31
#define VS1053_BANK_DEFAULT 0x00
#define VS1053_BANK_DRUMS1 0x78
#define VS1053_BANK_DRUMS2 0x7F
#define VS1053_BANK_MELODY 0x79

// See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 32 for more!
#define VS1053_GM1_OCARINA 80

#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
  #include <SoftwareSerial.h>
  SoftwareSerial VS1053_MIDI(0, 2); // TX only, do not use the 'rx' side
#else
  // on a Mega/Leonardo you may have to change the pin to one that 
  // software serial support uses OR use a hardware serial port!
  #define VS1053_MIDI Serial1
#endif

// Metronome Definition
Metro ledMetro = Metro(500);
int count = 0;

// Measure Matrix sound variables
int currNote = 0;
int currBank = 0;
int prevCount = 0;
int meMat[][6] = { {-1, -1, -1, -1, -1, -1}, {0, Crash1, -1, -1, -1, -1}, {0, Crash1, -1, 0, AcousticBassDrum, -1}};
int LEDmat[] = {LED1, LED2, LED3};
int palette[][3] = {{0, Crash1, -1},{1, ElectricGuitarClean, 60}};

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

int palbut = -1;
int stop = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("VS1053 MIDI test");
  
  VS1053_MIDI.begin(31250); // MIDI uses a 'strange baud rate'
  
  pinMode(VS1053_RESET, OUTPUT);
  digitalWrite(VS1053_RESET, LOW);
  delay(10);
  digitalWrite(VS1053_RESET, HIGH);
  delay(10);
  
  //midiSetChannelBank(0, VS1053_BANK_MELODY);
  //midiSetChannelBank(0, VS1053_BANK_DRUMS1);
  //midiSetChannelBank(0, VS1053_BANK_DRUMS2);
  //midiSetInstrument(0, VS1053_GM1_OCARINA);
  //midiSetInstrument(0, 39);
  midiSetChannelVolume(0, 127);
  midiSetChannelBank(0, VS1053_BANK_DRUMS2);
  midiSetChannelBank(1, VS1053_BANK_MELODY);

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT); 
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(LED5,OUTPUT);
  pinMode(BUTTON1,INPUT);
  pinMode(BUTTON2,INPUT);
  pinMode(BUTTON3,INPUT);

  Serial.println("VS1053 MIDI test");

}

void loop() {  
  buttonState1 = digitalRead(BUTTON1);
  buttonState2 = digitalRead(BUTTON2);
  buttonState3 = digitalRead(BUTTON3);
  
  if (ledMetro.check() == 1)
  {
    Serial.println(count);
    // Checking contents of meMat[][]
    for (int i = 0; i < 6; i++)
    {
      if (i == 0)
      {
        Serial.println("Instrument 1");
      }
      if (i == 3)
      {
        Serial.println("Instrument 2");
      }
      Serial.println(meMat[0][i]);
    }

    // turning off all midi sounds on last step
    if (count == 0){
      prevCount = 2;
    }
    else
    {
      prevCount = count - 1;
    }
    for (int i = 0; i < 6; i+=3) 
    {
      currBank = meMat[prevCount][i];
      if (currBank == 0)
      {
        currNote = meMat[prevCount][i+1];
      }
      if (currBank == 1)
      {
        currNote = meMat[prevCount][i+2];
      }
      midiNoteOff(currBank, currNote, 127);
    }

    // Turn on and off measure matrix LEDs
    digitalWrite(LEDmat[count-1],LOW);
    if (count == 0)
    {
      digitalWrite(LED3,LOW);
    }
    digitalWrite(LEDmat[count],HIGH);

    // play sounds on measure matrix
    for (int i = 0; i < 6; i+=3) {
      if (meMat[count][i] == 0)
      {
        midiSetInstrument(0,128);
        int channel = meMat[count][i];
        int note = meMat[count][i+1];
        midiNoteOn(channel, note, 127);
        currNote = note;
        currBank = 0;
      }
      if (meMat[count][i] == 1) {
        int instrum = meMat[count][i+1];
        midiSetInstrument(1,instrum);
        int channel = meMat[count][i];
        int note = meMat[count][i+2];
        midiNoteOn(channel, note, 127);
        currNote = note;
        currBank = 1;
      }
    }
    count++;
    if (count == 3)
    {
      count = 0;
    }
    ledMetro.reset();
  }
  
  if (buttonState1 == HIGH)
  {
    digitalWrite(LED4,HIGH);
    palbut = 0;
    stop = 0;
  }

  if (buttonState3 == HIGH)
  {
    digitalWrite(LED5,HIGH);
    palbut = 1;
    stop = 0;
  }

  if (buttonState2 == HIGH && palbut != -1)
  {
    digitalWrite(LED4,LOW);
    digitalWrite(LED5,LOW);
    int channel = palette[palbut][0];
    int instr = palette[palbut][1];
    int note = palette[palbut][2];
    for (int i = 0; i < 6; i+=3)
    {
      if (meMat[0][i] == channel && meMat[0][i+1] == instr && meMat[0][i+2] == note && stop == 0)
      {
        meMat[0][i] = -1;
        meMat[0][i+1] = -1;
        meMat[0][i+2] = -1;
        stop = 1;
      }
    }
    for (int i = 0; i < 6; i+=3)
    {
      if (meMat[0][i] == -1 && stop == 0)
      {
        meMat[0][i] = channel;
        meMat[0][i+1] = instr;
        meMat[0][i+2] = note;
        stop = 1;
        //Serial.println("here");
      }
    }
    palbut = -1;
  }
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
  
  VS1053_MIDI.write(MIDI_CHAN_PROGRAM | chan);  
  VS1053_MIDI.write(inst);
}


void midiSetChannelVolume(uint8_t chan, uint8_t vol) {
  if (chan > 15) return;
  if (vol > 127) return;
  
  VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
  VS1053_MIDI.write(MIDI_CHAN_VOLUME);
  VS1053_MIDI.write(vol);
}

void midiSetChannelBank(uint8_t chan, uint8_t bank) {
  if (chan > 15) return;
  if (bank > 127) return;
  
  VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
  VS1053_MIDI.write((uint8_t)MIDI_CHAN_BANK);
  VS1053_MIDI.write(bank);
}

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  VS1053_MIDI.write(MIDI_NOTE_ON | chan);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vel);
}

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel) {
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  VS1053_MIDI.write(MIDI_NOTE_OFF | chan);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vel);
}