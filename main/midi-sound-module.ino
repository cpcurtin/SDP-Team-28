/*
 *	example function library
 *
 *
 *
 */
#include "midi-sound-module.h"

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