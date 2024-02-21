/*
 *
 *       example function library
 *
 *
 */
#ifndef MIDI_SOUND_MODULE_H
#define MIDI_SOUND_MODULE_H


// functions, extern variables, structs go here
void midiSetInstrument(uint8_t chan, uint8_t inst);

void midiSetChannelVolume(uint8_t chan, uint8_t vol);

void midiSetChannelBank(uint8_t chan, uint8_t bank);

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel);

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel);

#endif // MIDI_SOUND_MODULE_H