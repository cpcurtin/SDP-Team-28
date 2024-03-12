/*
 *	example function library
 *
 *
 *
 */
#include "midi-sound-module.h"

int midi_init(void)
{

  // Midi Init
  MIDI.begin(31250);
  pinMode(VS1053_RST, OUTPUT);
  digitalWrite(VS1053_RST, LOW);
  delay(10);
  digitalWrite(VS1053_RST, HIGH);
  delay(10);
  midiSetChannelVolume(0, 127);
  midiSetChannelBank(0, Drums2);
  midiSetChannelBank(1, Melody);

  midiSetInstrument(0, 128);
  /*****************************************
  PUT THIS LINE AT THE TOP OF ALL PERCUSSIVE SOUND BLOCKS SO IT WILL
  START AS A PERCUSION SOUND:
  midiSetInstrument(0,128);
  *****************************************/
  return 0;
}

array_with_size *fetch_midi_melodic_sounds(void)
{
  midi_melodic_sound_list->array = midi_melodic_sounds;
  midi_melodic_sound_list->size = sizeof(midi_melodic_sounds)/sizeof(midi_melodic_sounds[0]);
  return midi_melodic_sound_list;
}

array_with_size *fetch_midi_percussion_sounds(void)
{
  midi_percussion_sound_list->array = midi_percussion_sounds;
  midi_percussion_sound_list->size = sizeof(midi_percussion_sounds)/sizeof(midi_percussion_sounds[0]);
  return midi_percussion_sound_list;
}

void midiSetInstrument(uint8_t chan, uint8_t inst)
{
  if (chan > 15)
    return;
  inst--; // page 32 has instruments starting with 1 not 0 :(
  if (inst > 127)
    return;

  MIDI.write(PROGRAM | chan);
  MIDI.write(inst);
}

void midiSetChannelVolume(uint8_t chan, uint8_t vol)
{
  if (chan > 15)
    return;
  if (vol > 127)
    return;

  MIDI.write(MESSAGE | chan);
  MIDI.write(VOLUME);
  MIDI.write(vol);
}

void midiSetChannelBank(uint8_t chan, uint8_t bank)
{
  if (chan > 15)
    return;
  if (bank > 127)
    return;

  MIDI.write(MESSAGE | chan);
  MIDI.write((uint8_t)BANK);
  MIDI.write(bank);
  Serial.println("Bank change");
  Serial.println(bank);
}

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel)
{
  if (chan > 15)
    return;
  if (n > 127)
    return;
  if (vel > 127)
    return;

  MIDI.write(NoteOn | chan);
  MIDI.write(n);
  MIDI.write(vel);
}

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel)
{
  if (chan > 15)
    return;
  if (n > 127)
    return;
  if (vel > 127)
    return;

  MIDI.write(NoteOff | chan);
  MIDI.write(n);
  MIDI.write(vel);
}