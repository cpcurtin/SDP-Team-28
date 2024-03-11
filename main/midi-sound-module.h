/*
 *
 *       example function library
 *
 *
 */
#ifndef MIDI_SOUND_MODULE_H
#define MIDI_SOUND_MODULE_H
// #define (x) #x
#define NUM_NOTES 12
#define NUM_OCTAVES 11
/**************************
Midi Definitions
**************************/
// Pins
#define VS1053 1
#define VS1053_RST 9
// Sound Banks
#define DEFAULT 0x00
#define Melody 0x79
#define Drums1 0x78
#define Drums2 0x7F
// Commands
#define NoteOn 0x90
#define NoteOff 0x80
#define MESSAGE 0xB0
#define BANK 0x00
#define VOLUME 0x07
#define PROGRAM 0xC0
#define OFF1 0x7B
#define OFF2 0x7C
#define OFF3 0x7D
// Define Serial Bus
#define MIDI Serial1
/**************************
Melodic Sounds
**************************/
#define AcousticGrandPiano 1
#define BrightAcousticPiano 2
#define ElectricGrandPiano 3
#define HonkyTonkPiano 4
#define ElectricPiano1 5
#define ElectricPiano2 6
#define Harpsichord 7
#define Clavi 8
#define Celesta 9
#define Clockenspiel 10
#define MusicBox 11
#define Vibraphone 12
#define Marimba 13
#define Xylophone 14
#define TubularBells 15
#define Dulcimer 16
#define DrawbarOrgan 17
#define PercussiveOrgan 18
#define RockOrgan 19
#define ChurchOrgan 20
#define ReedOrgan 21
#define Accordion 22
#define Harmonica 23
#define TangoAccordion 24
#define AcousticGuitarNylon 25
#define AcousticGuitarSteel 26
#define ElectricGuitarJazz 27
#define ElectricGuitarClean 28
#define ElectricGuitarMuted 29
#define OverdrivenGuitar 30
#define DistortionGuitar 31
#define GuitarHarmonics 32
#define AcousticBass 33
#define ElectricBassFinger 34
#define ElectricBassPick 35
#define FretlessBass 36
#define SlapBass1 37
#define SlapBass2 38
#define SynthBass1 39
#define SynthBass2 40
#define Violin 41
#define Viola 42
#define Cello 43
#define ContraBass 44
#define TremoloStrings 45
#define PizzicatoStrings 46
#define OrchestralHarp 47
#define Timpani 48
#define StringEnsemble1 49
#define StringEnsemble2 50
#define SynthStrings1 51
#define SynthStrings2 52
#define ChoirAahs 53
#define VoiceOohs 54
#define SynthVoice 55
#define OrchestraHit 56
#define Trumpet 57
#define Trombone 58
#define Tuba 59
#define MutedTrumpet 60
#define FrenchHorn 61
#define BrassSelection 62
#define SynthBrass1 63
#define SynthBrass2 64
#define SopranoSax 65
#define AltoSax 66
#define TenorSax 67
#define BaritoneSax 68
#define Oboe 69
#define EnglishHorn 70
#define Bassoon 71
#define Clarinet 72
#define Piccolo 73
#define Flute 74
#define Recorder 75
#define PanFlute 76
#define BlownBottle 77
#define Shakuhachi 78
#define Whistle 79
#define Ocarina 80
#define SquareLead 81
#define SawLead 82
#define CalliopeLead 83
#define ChiffLead 84
#define CharangLead 85
#define VoiceLead 86
#define FifthsLead 87
#define BassLead 88
#define NewAge 89
#define WarmPad 90
#define Polysynth 91
#define Choir 92
#define Bowed 93
#define Metallic 94
#define Halo 95
#define Sweep 96
#define Rain 97
#define SoundTrack 98
#define Crystal 99
#define Atmosphere 100
#define Brightness 101
#define Goblins 102
#define Echoes 103
#define SciFi 104
#define Sitar 105
#define Banjo 106
#define Shamisen 107
#define Koto 108
#define Kalimba 109
#define BagPipe 110
#define Fiddle 111
#define Shanai 112
#define TinkleBell 113
#define Agogo 114
#define PitchedPercussion 115
#define WoodBlock 116
#define TaikoDrum 117
#define MelodicTom 118
#define SynthDrum 119
#define ReverseCymbal 120
#define GuitarFretNoise 121
#define BreathNoise 122
#define Seashore 123
#define BirdTweet 124
#define TelephoneRing 125
#define Helicopter 126
#define Applause 127
#define Gunshot 128

/**************************
Percussive Sounds
**************************/
#define HighQ 27
#define Slap 28
#define ScratchPush 29
#define ScratchPull 30
#define Sticks 31
#define SquareClick 32
#define MetClick 33
#define MetBell 34
#define AcousticBassDrum 35
#define BassDrum 36
#define SideStick 37
#define AcousticSnare 38
#define Clap 39
#define ElectricSnare 40
#define LowFloorTom 41
#define ClosedHiHat 42
#define HighFloorTom 43
#define PedalHiHat 44
#define LowTom 45
#define OpenHiHat 46
#define LowMidTom 47
#define HighMidTom 48
#define Crash1 49
#define HighTom 50
#define Ride1 51
#define China 52
#define RideBell 53
#define Tambourine 54
#define Splash 55
#define Cowbell 56
#define Crash2 57
#define Vibraslap 58
#define Ride2 59
#define HighBongo 60
#define LowBogo 61
#define MuteHighConga 62
#define HighConga 63
#define LowConga 64
#define HighTimbale 65
#define LowTimbale 66
#define HighAgogo 67
#define LowAgogo 68
#define Cabasa 69
#define Maracas 70
#define ShortWhistle 71
#define LongWhistle 72
#define ShortGuiro 73
#define LongGuiro 74
#define Claves 75
#define HighWoodBlock 76
#define LowWoodBlock 77
#define MuteCuica 78
#define Cuica 79
#define MuteTriangle 80
#define Triangle 81
#define Shaker 82
#define JingleBell 83
#define BellTree 84
#define Castanets 85
#define MuteSurdo 86
#define Surdo 87

const char *midi_percussion_sounds[] = {"HighQ",
                                        "Slap",
                                        "ScratchPush",
                                        "ScratchPull", "Sticks", "SquareClick",
                                        "MetClick", "MetBell", "AcousticBassDrum",
                                        "BassDrum", "SideStick", "AcousticSnare",
                                        "Clap", "ElectricSnare", "LowFloorTom",
                                        "ClosedHiHat", "HighFloorTom", "PedalHiHat",
                                        "LowTom", "OpenHiHat", "LowMidTom",
                                        "HighMidTom", "Crash1", "HighTom",
                                        "Ride1", "China", "RideBell",
                                        "Tambourine", "Splash", "Cowbell",
                                        "Crash2", "Vibraslap", "Ride2",
                                        "HighBongo", "LowBogo", "MuteHighConga",
                                        "HighConga", "LowConga", "HighTimbale",
                                        "LowTimbale", "HighAgogo", "LowAgogo",
                                        "Cabasa", "Maracas", "ShortWhistle",
                                        "LongWhistle", "ShortGuiro", "LongGuiro",
                                        "Claves", "HighWoodBlock", "LowWoodBlock",
                                        "Mute Cuica", "Cuica", "MuteTriangle",
                                        "Triangle", "Shaker", "JingleBell",
                                        "BellTree", "Castanets", "MuteSurdo",
                                        "Surdo"};

const char *midi_melodic_sounds[] = {"AcousticGrandPiano",
                                     "BrightAcousticPiano",
                                     "ElectricGrandPiano",
                                     "HonkyTonkPiano",
                                     "ElectricPiano1",
                                     "ElectricPiano2",
                                     "Harpsichord",
                                     "Clavi",
                                     "Celesta",
                                     "Clockenspiel",
                                     "MusicBox",
                                     "Vibraphone", "Marimba",
                                     "Xylophone",
                                     "TubularBells",
                                     "Dulcimer",
                                     "DrawbarOrgan",
                                     "PercussiveOrgan",
                                     "RockOrgan",
                                     "ChurchOrgan",
                                     "ReedOrgan",
                                     "Accordion",
                                     "Harmonica", "TangoAccordion", "AcousticGuitarNylon", "AcousticGuitarSteel", "ElectricGuitarJazz", "ElectricGuitarClean", "ElectricGuitarMuted", "OverdrivenGuitar", "DistortionGuitar", "GuitarHarmonics", "AcousticBass",
                                     "ElectricBassFinger", "ElectricBassPick",
                                     "FretlessBass",
                                     "SlapBass1", "SlapBass2", "SynthBass1",
                                     "SynthBass2", "Violin",
                                     "Viola",
                                     "Cello",
                                     "Contrabass", "TremoloStrings",
                                     "PizzicatoStrings",
                                     "OrchestralHarp", "Timpani", "StringEnsemble1",
                                     "StringEnsemble2",
                                     "SynthStrings1",
                                     "SynthStrings2",
                                     "ChoirAahs", "VoiceOohs", "SynthVoice",
                                     "OrchestraHit", "Trumpet",
                                     "Trombone",
                                     "Tuba", "MutedTrumpet", "FrenchHorn", "BrassSelection",
                                     "SynthBrass1",
                                     "SynthBrass2",
                                     "SopranoSax",
                                     "AltoSax", "TenorSax",
                                     "BaritoneSax", "Oboe", "EnglishHorn", "Bassoon", "Clarinet",
                                     "Piccolo",
                                     "Flute",
                                     "Recorder",
                                     "PanFlute",
                                     "BlownBottle",
                                     "Shakuhachi", "Whistle", "Ocarina", "SquareLead", "SawLead",
                                     "CalliopeLead",
                                     "ChiffLead",
                                     "CharangLead",
                                     "VoiceLead",
                                     "FifthsLead",
                                     "BassLead", "NewAge", "WarmPad", "Polysynth",
                                     "Choir",
                                     "Bowed",
                                     "Metallic",
                                     "Halo",
                                     "Sweep",
                                     "Rain",
                                     "SoundTrack", "Crystal", "Atmosphere", "Brightness",
                                     "Goblins",
                                     "Echoes",
                                     "SciFi",
                                     "Sitar", "Banjo", "Shamisen",
                                     "Koto",
                                     "Kalimba",
                                     "BagPipe", "Fiddle", "Shanai",
                                     "TinkleBell",
                                     "Agogo",
                                     "PitchedPercussion", "WoodBlock",
                                     "TaikoDrum",
                                     "MelodicTom", "SynthDrum",
                                     "ReverseCymbal", "GuitarFretNoise", "BreathNoise", "Seashore",
                                     "BirdTweet",
                                     "TelephoneRing",
                                     "Helicopter",
                                     "Applause",
                                     "Gunshot"};

// functions, extern variables, structs go here
const int midi_percussion_values[] = {HighQ, Slap, ScratchPush, ScratchPull, Sticks,
                                      SquareClick, MetClick, MetBell, AcousticBassDrum, BassDrum,
                                      SideStick, AcousticSnare, Clap, ElectricSnare, LowFloorTom,
                                      ClosedHiHat, HighFloorTom, PedalHiHat, LowTom, OpenHiHat,
                                      LowMidTom, HighMidTom, Crash1, HighTom, Ride1,
                                      China, RideBell, Tambourine, Splash, Cowbell,
                                      Crash2, Vibraslap, Ride2, HighBongo, LowBogo,
                                      MuteHighConga, HighConga, LowConga, HighTimbale, LowTimbale,
                                      HighAgogo, LowAgogo, Cabasa, Maracas, ShortWhistle,
                                      LongWhistle, ShortGuiro, LongGuiro, Claves, HighWoodBlock,
                                      LowWoodBlock, MuteCuica, Cuica, MuteTriangle, Triangle,
                                      Shaker, JingleBell, BellTree, Castanets, MuteSurdo,
                                      Surdo};

const int midi_values[] = {AcousticGrandPiano,
                           BrightAcousticPiano,
                           ElectricGrandPiano,
                           HonkyTonkPiano,
                           ElectricPiano1,
                           ElectricPiano2,
                           Harpsichord,
                           Clavi,
                           Celesta,
                           Clockenspiel,
                           MusicBox,
                           Vibraphone,
                           Marimba,
                           Xylophone,
                           TubularBells,
                           Dulcimer,
                           DrawbarOrgan,
                           PercussiveOrgan,
                           RockOrgan,
                           ChurchOrgan,
                           ReedOrgan,
                           Accordion,
                           Harmonica,
                           TangoAccordion,
                           AcousticGuitarNylon,
                           AcousticGuitarSteel,
                           ElectricGuitarJazz,
                           ElectricGuitarClean,
                           ElectricGuitarMuted,
                           OverdrivenGuitar,
                           DistortionGuitar,
                           GuitarHarmonics,
                           AcousticBass,
                           ElectricBassFinger,
                           ElectricBassPick,
                           FretlessBass,
                           SlapBass1,
                           SlapBass2,
                           SynthBass1,
                           SynthBass2,
                           Violin,
                           Viola,
                           Cello,
                           ContraBass,
                           TremoloStrings,
                           PizzicatoStrings,
                           OrchestralHarp,
                           Timpani,
                           StringEnsemble1,
                           StringEnsemble2,
                           SynthStrings1,
                           SynthStrings2,
                           ChoirAahs,
                           VoiceOohs,
                           SynthVoice,
                           OrchestraHit,
                           Trumpet,
                           Trombone,
                           Tuba,
                           MutedTrumpet,
                           FrenchHorn,
                           BrassSelection,
                           SynthBrass1,
                           SynthBrass2,
                           SopranoSax,
                           AltoSax,
                           TenorSax,
                           BaritoneSax,
                           Oboe,
                           EnglishHorn,
                           Bassoon,
                           Clarinet,
                           Piccolo,
                           Flute,
                           Recorder,
                           PanFlute,
                           BlownBottle,
                           Shakuhachi,
                           Whistle,
                           Ocarina,
                           SquareLead,
                           SawLead,
                           CalliopeLead,
                           ChiffLead,
                           CharangLead,
                           VoiceLead,
                           FifthsLead,
                           BassLead,
                           NewAge,
                           WarmPad,
                           Polysynth,
                           Choir,
                           Bowed,
                           Metallic,
                           Halo,
                           Sweep,
                           Rain,
                           SoundTrack,
                           Crystal,
                           Atmosphere,
                           Brightness,
                           Goblins,
                           Echoes,
                           SciFi,
                           Sitar,
                           Banjo,
                           Shamisen,
                           Koto,
                           Kalimba,
                           BagPipe,
                           Fiddle,
                           Shanai,
                           TinkleBell,
                           Agogo,
                           PitchedPercussion,
                           WoodBlock,
                           TaikoDrum,
                           MelodicTom,
                           SynthDrum,
                           ReverseCymbal,
                           GuitarFretNoise,
                           BreathNoise,
                           Seashore,
                           BirdTweet,
                           TelephoneRing,
                           Helicopter,
                           Applause,
                           Gunshot};

// Array for MIDI note values
// when displaying octaves need to decrement by 2 to begin with -2, -1, 0,.. etc
int midi_mapping[NUM_NOTES][NUM_OCTAVES] = {
    {0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 120},
    {1, 13, 25, 37, 49, 61, 73, 85, 97, 109, 121},
    {2, 14, 26, 38, 50, 62, 74, 86, 98, 110, 122},
    {3, 15, 27, 39, 51, 63, 75, 87, 99, 111, 123},
    {4, 16, 28, 40, 52, 64, 76, 88, 100, 112, 124},
    {5, 17, 29, 41, 53, 65, 77, 89, 101, 113, 125},
    {6, 18, 30, 42, 54, 66, 78, 90, 102, 114, 126},
    {7, 19, 31, 43, 55, 67, 79, 91, 103, 115, 127},
    {8, 20, 32, 44, 56, 68, 80, 92, 104, 116, -1},
    {9, 21, 33, 45, 57, 69, 81, 93, 105, 117, -1},
    {10, 22, 34, 46, 58, 70, 82, 94, 106, 118, -1},
    {11, 23, 35, 47, 59, 71, 83, 95, 107, 119, -1}};

array_with_size *midi_melodic_sound_list = new array_with_size;
array_with_size *midi_percussion_sound_list = new array_with_size;

int midi_init(void);

array_with_size *fetch_midi_melodic_sounds(void);
array_with_size *fetch_midi_percussion_sounds(void);

void midiSetInstrument(uint8_t chan, uint8_t inst);

void midiSetChannelVolume(uint8_t chan, uint8_t vol);

void midiSetChannelBank(uint8_t chan, uint8_t bank);

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel);

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel);

#endif // MIDI_SOUND_MODULE_H