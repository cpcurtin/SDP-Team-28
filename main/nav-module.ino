/*
 *	example function library
 *
 *
 *
 */
#include "nav-module.h"

void nav_add(Nav *node)
{
}

#if USING_SAFE_STRINGS == 1
Nav *nav_selection(Nav *nav, int direction)
{
    if (direction > 0)
    {
        if (nav->child != nullptr)
        {
            nav = nav->child[nav->index];
            return nav;
        }
    }
    else if (direction < 0)
    {
        if (nav->parent != nullptr)
        {
            nav = nav->parent;
            return nav;
        }
    }
    return nullptr;
}

void array_scroll(Nav *nav, int direction)
{
    // Serial.print(nav->name.c_str());
    Serial.print(" S=");
    Serial.println(nav->data_array.size());
    int new_index;
    // new index changes +/- and goes from end to 0
    new_index = (nav->index + direction + nav->data_array.size()) % nav->data_array.size();

    nav->index = new_index;
    nav->lcd_state[0] = format_row(nav->data_array, new_index, 1).c_str();

    // THIS IS WHERE IT REPEATS
    for (size_t row = 1; row < LCD_ROWS - 1; row++)
    {
        if (row < nav->data_array.size())
        {
            int temp_index = (new_index + row) % nav->data_array.size();
            nav->lcd_state[row] = format_row(nav->data_array, temp_index, 0).c_str();
        }
        else
        {
            nav->lcd_state[row] = std::string(20, ' ');
        }
    }
}

std::string format_row(std::vector<std::string> data_array, int index, int format)
{
    std::string rt_st;

    // spacing, enumerated
    if (format == 0)
    {
        rt_st = " " + std::to_string(index + 1) + " " + std::string(data_array[index]);
    }
    else if (format == 1)
    {
        rt_st = ">" + std::to_string(index + 1) + " " + std::string(data_array[index]);
    }

    Serial.println(rt_st.c_str());
    Serial.println("RESIZE");
    rt_st.resize(20);
    rt_st.shrink_to_fit();
    Serial.println(rt_st.c_str());
    return rt_st;
}

std::string tracks_update(void)
{
    std::string rt_st;

    // spacing, enumerated
    if (current_track->bpm != 0)
    {
        rt_st = "BPM:" + std::to_string(current_track->bpm) + " STEPS:" + std::to_string(current_track->measure_steps) + " ID:" + std::to_string(current_track->id) + " ";
    }
    else
    {
        rt_st = "NO TRACK SELECTED";
    }
    Serial.println(rt_st.c_str());
    Serial.println("RESIZE");
    rt_st.resize(20);
    rt_st.shrink_to_fit();
    Serial.println(rt_st.c_str());
    return rt_st;
}

Nav *nav_init(struct nav_config *cfg)
{
    // ptr arrays
    Nav **main_child = new Nav *[3];
    main_child[0] = sounds_nav;
    main_child[1] = effects_nav;
    main_child[2] = tracks_nav;

    Nav **sounds_child = new Nav *[2];
    sounds_child[0] = sounds_custom_nav;
    sounds_child[1] = sounds_midi_nav;

    Nav **midi_child = new Nav *[2];
    midi_child[0] = sounds_midi_percussion_nav;
    midi_child[1] = sounds_midi_melodic_nav;

    Nav **tracks_child = new Nav *[4];
    tracks_child[0] = tracks_set_steps_nav; // set steps
    // tracks_child[1] = SAVE TRACK
    tracks_child[2] = tracks_load_nav; // load track
    // tracks_child[3] = DELETE TRACK

    // Main presets
    std::vector<std::string> main_preset_options = {"Sounds", "Effects", "Tracks"};

    // main_nav->name = "main";
    main_nav->id = NAVIGATION_MAIN;
    main_nav->data_array = std::move(main_preset_options);
    main_nav->parent = nullptr;
    main_nav->child = main_child;
    main_nav->index = 0;
    main_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(main_nav, 0);

    // Sounds presets
    std::vector<std::string> sounds_preset_options = {"Custom Sounds", "MIDI Sounds"};

    // sounds_nav->name = "sounds";
    sounds_nav->id = NAVIGATION_SOUNDS;
    sounds_nav->data_array = std::move(sounds_preset_options);
    sounds_nav->parent = main_nav;
    sounds_nav->child = sounds_child;
    sounds_nav->index = 0;
    sounds_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_nav, 0);

    // Effects presets

    // effects_nav->name = "effects";
    effects_nav->id = NAVIGATION_EFFECTS;
    effects_nav->data_array = cfg->effects;
    effects_nav->parent = main_nav;
    effects_nav->child = nullptr;
    effects_nav->index = 0;
    effects_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(effects_nav, 0);

    // Tracks presets
    std::vector<std::string> tracks_preset_options = {"Set # steps", "Save Track", "Load Track", "Delete Track"};

    // tracks_nav->name = "tracks";
    tracks_nav->id = NAVIGATION_TRACKS;
    tracks_nav->data_array = std::move(tracks_preset_options);
    tracks_nav->parent = main_nav;
    tracks_nav->child = tracks_child;
    tracks_nav->index = 0;
    tracks_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(tracks_nav, 0);

    // Tracks Load presets

    // tracks_load_nav->name = "tracks_load";
    tracks_load_nav->id = NAVIGATION_TRACK_LOAD;
    tracks_load_nav->data_array = cfg->tracks_load;
    tracks_load_nav->parent = tracks_nav;
    tracks_load_nav->child = nullptr;
    tracks_load_nav->index = 0;
    tracks_load_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(tracks_load_nav, 0);

    // Tracks Set Steps presets
    std::vector<std::string> tracks_preset_options_steps;
    for (int i = 0; i < MAX_MEASURE_STEPS; ++i)
    {
        tracks_preset_options_steps.push_back("Step");
    }

    // tracks_set_steps_nav->name = "tracks_set_steps";
    tracks_set_steps_nav->id = NAVIGATION_SET_GLOBAL_STEPS;
    tracks_set_steps_nav->data_array = std::move(tracks_preset_options_steps);
    tracks_set_steps_nav->parent = tracks_nav;
    tracks_set_steps_nav->child = nullptr;
    tracks_set_steps_nav->index = 0;
    tracks_set_steps_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(tracks_set_steps_nav, 0);

    // Custom Sounds presets

    // sounds_custom_nav->name = "custom_sounds";
    sounds_custom_nav->id = NAVIGATION_SOUNDS_CUSTOM;
    sounds_custom_nav->data_array = cfg->sounds_custom;
    sounds_custom_nav->parent = sounds_nav;
    sounds_custom_nav->child = nullptr;
    sounds_custom_nav->index = 0;
    sounds_custom_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_custom_nav, 0);

    // MIDI Sounds presets
    std::vector<std::string> midi_preset_options = {"Percussion Instruments", "Melodic Instruments"};

    // sounds_midi_nav->name = "sounds_midi";
    sounds_midi_nav->id = NAVIGATION_SOUNDS_MIDI;
    sounds_midi_nav->data_array = std::move(midi_preset_options);
    sounds_midi_nav->parent = sounds_nav;
    sounds_midi_nav->child = midi_child;
    sounds_midi_nav->index = 0;
    sounds_midi_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_midi_nav, 0);

    // MIDI Melodic presets

    // sounds_midi_melodic_nav->name = "sounds_midi_melodic";
    sounds_midi_melodic_nav->id = NAVIGATION_SOUNDS_MIDI_MELODIC;
    sounds_midi_melodic_nav->data_array = cfg->sounds_midi_melodic;
    sounds_midi_melodic_nav->parent = sounds_midi_nav;
    sounds_midi_melodic_nav->child = nullptr;
    sounds_midi_melodic_nav->index = 0;
    sounds_midi_melodic_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_midi_melodic_nav, 0);

    // MIDI Percussion presets

    // sounds_midi_percussion_nav->name = "sounds_midi_percussion";
    sounds_midi_percussion_nav->id = NAVIGATION_SOUNDS_MIDI_PERCUSSION;
    sounds_midi_percussion_nav->data_array = cfg->sounds_midi_percussion;
    sounds_midi_percussion_nav->parent = sounds_midi_nav;
    sounds_midi_percussion_nav->child = nullptr;
    sounds_midi_percussion_nav->index = 0;
    sounds_midi_percussion_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_midi_percussion_nav, 0);

    // MIDI Octaves presets

    // sounds_midi_octaves_nav->name = "sounds_midi_octaves";
    sounds_midi_octaves_nav->id = NAVIGATION_MIDI_OCTAVES;
    sounds_midi_octaves_nav->data_array = std::move(octaves);
    sounds_midi_octaves_nav->parent = sounds_midi_melodic_nav;
    sounds_midi_octaves_nav->child = nullptr;
    sounds_midi_octaves_nav->index = 0;
    sounds_midi_octaves_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_midi_octaves_nav, 0);

    // MIDI Notes presets

    // sounds_midi_notes_nav->name = "sounds_midi_notes";
    sounds_midi_notes_nav->id = NAVIGATION_MIDI_NOTES;
    sounds_midi_notes_nav->data_array = std::move(note_names);
    sounds_midi_notes_nav->parent = sounds_midi_octaves_nav;
    sounds_midi_notes_nav->child = nullptr;
    sounds_midi_notes_nav->index = 0;
    sounds_midi_notes_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_midi_notes_nav, 0);

    // Return the main_nav as per original function
    return main_nav;
}

void dpad_nav_routine(int dpad_pressed)
{

    switch (dpad_pressed)
    {
    case BUTTON_DPAD_LEFT:
        if (nav_selection(nav_state, NAV_BACKWARD) != nullptr)
        {
            Serial.print("MOVED TO NODE:");
            Serial.println(nav_state->id);
        }
        else
        {
            Serial.print("ALREADY AT ROOT");
        }
        break;
    case BUTTON_DPAD_DOWN:
        array_scroll(nav_state, NAV_DOWN);
        break;
    case BUTTON_DPAD_UP:
        array_scroll(nav_state, NAV_UP);

        break;
    case BUTTON_DPAD_RIGHT:
        if (nav_selection(nav_state, NAV_FORWARD) != nullptr)
        {
            Serial.print("MOVED TO NODE:");
            Serial.println(nav_state->id);
        }
        else
        {
            Serial.print("REACHED SELECTION LEAF");
            execute_leaf();
        }
        break;
    default:
        break;
        lcd_display(lcd, nav_state->lcd_state);
    }
}

int execute_leaf(void)
{
    switch (nav_state->id)
    {
    case NAVIGATION_TRACKS:
        break;
    case NAVIGATION_SOUNDS_CUSTOM:
        temp_sample = cache_sd_sound(sounds_custom_nav->data_array[sounds_custom_nav->index].c_str());
        if (temp_sample != nullptr)
        {
            dispFlag = 3;

            new_sound.bank = -1;
            new_sound.instrument = -1;
            new_sound.note = -1;
            new_sound_assignment = true;
            new_sound.sd_cached_sound = temp_sample;

            new_sound.filename = sounds_custom_nav->data_array[sounds_custom_nav->index];
            // strlcpy(new_sound.filename,                                      // <- destination
            //         sounds_custom_nav->data_array[sounds_custom_nav->index], // <- source
            //         sizeof(new_sound.filename));

            lcd_splash(lcd, nav_state, selected_sound);

            char str[20];
            sprintf(str, "%p", (void *)temp_sample); // Using sprintf to format the pointer address
            Serial.println("\tEXPECTED: " + String(str));

            sprintf(str, "%p", (void *)new_sound.sd_cached_sound); // Using sprintf to format the pointer address
            Serial.println("\tACTUAL: " + String(str));

            current_track->cached_sounds.push_back(new_sound);
        }
        else
        {
            new_sound.bank = -1;
            new_sound.instrument = -1;
            new_sound.note = -1;
            new_sound.sd_cached_sound = nullptr;
            // memset(new_sound.filename, 0, sizeof(new_sound.filename));
            new_sound.filename = "";
            // NO SIZE ON PSRAM TO CACHE SOUND
            lcd_splash(lcd, nullptr, error_psram_full);
        }
        nav_state = main_nav;
        break;

    case NAVIGATION_SOUNDS_MIDI_PERCUSSION:
        new_sound.bank = sounds_midi_nav->index;
        new_sound.instrument = midi_percussion_values[sounds_midi_percussion_nav->index];
        new_sound.note = -1;
        new_sound.sd_cached_sound = nullptr;
        // memset(new_sound.filename, 0, sizeof(new_sound.filename));
        new_sound.filename = "";
        new_sound_assignment = true;

        lcd_splash(lcd, nav_state, selected_sound);
        nav_state = main_nav;
        break;

    case NAVIGATION_SOUNDS_MIDI_MELODIC:
        nav_state = sounds_midi_octaves_nav;
        break;

    case NAVIGATION_MIDI_OCTAVES:
        nav_state = sounds_midi_notes_nav;
        break;

    case NAVIGATION_MIDI_NOTES:
        Serial.println("MIDI SOUNDS SELECTION:");
        Serial.print("BANK: ");
        // Serial.println(((nav_state->parent)->parent)->name.c_str());
        Serial.print("Octave: ");
        Serial.println((nav_state->parent)->data_array[(nav_state->parent)->index].c_str());
        Serial.print("Note: ");
        Serial.println(nav_state->data_array[nav_state->index].c_str());
        Serial.print("TEST: ");
        Serial.println(midi_mapping[nav_state->index][(nav_state->parent)->index]);

        new_sound.bank = sounds_midi_nav->index;
        new_sound.instrument = midi_melodic_values[sounds_midi_melodic_nav->index];
        new_sound.note = midi_mapping[sounds_midi_notes_nav->index][sounds_midi_octaves_nav->index];
        new_sound.sd_cached_sound = nullptr;
        new_sound.filename = "";
        new_sound_assignment = true;

        lcd_splash(lcd, nav_state, selected_sound);
        nav_state = main_nav;
        break;

        // default:
        //     break;
    }

    return 0;
}
int track_options(void)
{

    switch (nav_state->index)
    {
    case LEAF_TRACKS_GLOBAL_STEPS:
        current_track->measure_steps = nav_state->index + 1;
        for (int i = 0; i < 4; i++)
        {
            current_measure->beat_list[i].active_steps = current_track->measure_steps;
        }
        break;

    case LEAF_TRACKS_SAVE:
        std::string new_track_filename = "TRACK" + std::to_string((nav_state->child[2])->data_array.size()) + ".json";
        current_track->id = (nav_state->child[2])->data_array.size();
        save_track(new_track_filename.c_str(), current_track);
        nav_state->child[2]->data_array.clear();
        track_list = sd_fetch_tracks();
        std::copy(track_list.begin(), track_list.end(), std::back_inserter(nav_state->child[2]->data_array));
        (nav_state->child[2])->index = 0;
        array_scroll(nav_state->child[2], 0);
        break;

    case LEAF_TRACKS_LOAD:

        Serial.println("START LOAD TRACK");
        Serial.println("free cached sounds");
        free_cached_sounds(current_track);
        Serial.println("free previous track");
        delete current_track;
        // free_track(current_track);
        Serial.println("load new track");
        read_track(nav_state->data_array[nav_state->index], current_track);
        Serial.println("RETURN LOAD TRACK");
        break;

    case LEAF_TRACKS_DELETE:
        std::string delete_track_filename = "TRACK" + std::to_string(current_track->id) + ".json";
        if (sd_delete_track(delete_track_filename))
        {
            // Serial.print("FAILED TO DELETE: ");
            Serial.println(current_track->filename.c_str());
        }
        else
        {
            nav_state->child[2]->data_array.clear();
            track_list = sd_fetch_tracks();
            std::copy(track_list.begin(), track_list.end(), std::back_inserter(nav_state->child[2]->data_array));
            (nav_state->child[2])->index = 0;
            array_scroll(nav_state->child[2], 0);
            read_track((nav_state->child[2])->data_array[(nav_state->child[2])->data_array.size() - 1], current_track);
        }
        break;

        // default:

        //     break;
    }
    return 0;
}

#else

Nav *nav_selection(Nav *nav, int direction)
{

    if (direction > 0)
    {
        if ((nav->child) != NULL)
        {
            Serial.println(nav->name);
            return nav->child[nav->index];
        }
        else if (strcmp(nav->name, "sounds_midi_melodic") == 0)
        {
            return sounds_midi_octaves_nav;
        }
        else if (strcmp(nav->name, "sounds_midi_octaves") == 0)
        {
            return sounds_midi_notes_nav;
        }
    }
    if (direction < 0)
    {
        if ((nav->parent) != NULL)
        {
            Serial.println(nav->name);
            return nav->parent;
        }
    }

    return nav;
}

Nav *nav_init(struct nav_config *cfg)
{

    const char **main_preset_options = new const char *[3];
    main_preset_options[0] = strdup("Sounds");
    main_preset_options[1] = strdup("Effects");
    main_preset_options[2] = strdup("Tracks");

    const char **sounds_preset_options = new const char *[2];
    sounds_preset_options[0] = strdup("Custom Sounds");
    sounds_preset_options[1] = strdup("MIDI Sounds");

    const char **tracks_preset_options = new const char *[4];
    tracks_preset_options[0] = strdup("Set # steps");
    tracks_preset_options[1] = strdup("Save Track");
    tracks_preset_options[2] = strdup("Load Track");
    tracks_preset_options[3] = strdup("Delete Track");

    const char **tracks_preset_options_steps = new const char *[6];
    for (int i = 0; i < MAX_MEASURE_STEPS; i++)
    {
        tracks_preset_options_steps[i] = strdup("Step");
    }

    const char **midi_preset_options = new const char *[2];
    midi_preset_options[0] = strdup("Percussion Instruments");
    midi_preset_options[1] = strdup("Melodic Instruments");
    // LCD STATES INITIALIZATION
    const char **state_main = new const char *[LCD_ROWS];
    const char **state_sounds = new const char *[LCD_ROWS];
    const char **state_effects = new const char *[LCD_ROWS];
    const char **state_tracks = new const char *[LCD_ROWS];
    const char **state_sounds_custom = new const char *[LCD_ROWS];
    const char **state_sounds_midi = new const char *[LCD_ROWS];
    const char **state_tracks_load = new const char *[LCD_ROWS];
    const char **state_tracks_set_steps = new const char *[LCD_ROWS];
    const char **state_midi_melodic = new const char *[LCD_ROWS];
    const char **state_midi_percussion = new const char *[LCD_ROWS];
    const char **state_midi_octaves = new const char *[LCD_ROWS];
    const char **state_midi_notes = new const char *[LCD_ROWS];

    // ptr arrays
    Nav **main_child = new Nav *[3];
    main_child[0] = sounds_nav;
    main_child[1] = effects_nav;
    main_child[2] = tracks_nav;

    Nav **sounds_child = new Nav *[2];
    sounds_child[0] = sounds_custom_nav;
    sounds_child[1] = sounds_midi_nav;

    Nav **midi_child = new Nav *[2];
    midi_child[0] = sounds_midi_percussion_nav;
    midi_child[1] = sounds_midi_melodic_nav;

    Nav **tracks_child = new Nav *[4];
    tracks_child[0] = tracks_set_steps_nav; // set steps
    // tracks_child[1] = SAVE TRACK
    tracks_child[2] = tracks_load_nav; // load track
    // tracks_child[3] = DELETE TRACK

    // main
    main_nav->name = strdup("main");
    main_nav->data_array = main_preset_options;
    main_nav->parent = NULL;
    main_nav->child = main_child;
    main_nav->size = 3; // sizeof(main_preset_options) / sizeof(main_preset_options[0]);
    main_nav->lcd_state = state_main;
    main_nav->index = 0;
    array_scroll(main_nav, 0);

    // sounds
    sounds_nav->name = strdup("sounds");
    sounds_nav->data_array = sounds_preset_options;
    sounds_nav->parent = main_nav;
    sounds_nav->child = sounds_child;
    sounds_nav->size = 2; // sizeof(sounds_preset_options) / sizeof(sounds_preset_options[0]);
    sounds_nav->lcd_state = state_sounds;
    sounds_nav->index = 0;
    array_scroll(sounds_nav, 0);

    // effects
    effects_nav->name = strdup("effects");
    effects_nav->data_array = (cfg->effects)->array;
    effects_nav->parent = main_nav;
    effects_nav->child = NULL;
    effects_nav->size = (cfg->effects)->size;
    effects_nav->lcd_state = state_effects;
    effects_nav->index = 0;
    array_scroll(effects_nav, 0);

    // tracks
    tracks_nav->name = strdup("tracks");
    tracks_nav->data_array = tracks_preset_options;
    tracks_nav->parent = main_nav;
    tracks_nav->child = tracks_child;
    tracks_nav->size = 4; // sizeof(tracks_preset_options) / sizeof(tracks_preset_options[0]);
    tracks_nav->lcd_state = state_tracks;
    tracks_nav->index = 0;
    array_scroll(tracks_nav, 0);

    // tracks LOAD
    tracks_load_nav->name = strdup("tracks_load");
    tracks_load_nav->data_array = (cfg->tracks_load)->array;
    tracks_load_nav->parent = tracks_nav;
    tracks_load_nav->child = NULL;
    tracks_load_nav->size = (cfg->tracks_load)->size;
    tracks_load_nav->lcd_state = state_tracks_load;
    tracks_load_nav->index = 0;
    array_scroll(tracks_load_nav, 0);

    // tracks SET STEPS
    tracks_set_steps_nav->name = strdup("tracks_set_steps");
    tracks_set_steps_nav->data_array = tracks_preset_options_steps;
    tracks_set_steps_nav->parent = tracks_nav;
    tracks_set_steps_nav->child = NULL;
    tracks_set_steps_nav->size = 6; // sizeof(tracks_preset_options_steps) / sizeof(tracks_preset_options_steps[0]);
    tracks_set_steps_nav->lcd_state = state_tracks_set_steps;
    tracks_set_steps_nav->index = 0;
    array_scroll(tracks_set_steps_nav, 0);

    // custom_sounds
    sounds_custom_nav->name = strdup("custom_sounds");
    sounds_custom_nav->data_array = (cfg->sounds_custom)->array;
    sounds_custom_nav->parent = sounds_nav;
    sounds_custom_nav->child = NULL;
    sounds_custom_nav->size = (cfg->sounds_custom)->size;
    sounds_custom_nav->lcd_state = state_sounds_custom;
    sounds_custom_nav->index = 0;
    array_scroll(sounds_custom_nav, 0);

    // sounds_midi
    sounds_midi_nav->name = strdup("sounds_midi");
    sounds_midi_nav->data_array = midi_preset_options;
    sounds_midi_nav->parent = sounds_nav;
    sounds_midi_nav->child = midi_child;
    sounds_midi_nav->size = 2;
    sounds_midi_nav->lcd_state = state_sounds_midi;
    sounds_midi_nav->index = 0;
    array_scroll(sounds_midi_nav, 0);

    // sounds_midi_melodic
    sounds_midi_melodic_nav->name = strdup("sounds_midi_melodic");
    sounds_midi_melodic_nav->data_array = (cfg->sounds_midi_melodic)->array;
    sounds_midi_melodic_nav->parent = sounds_midi_nav;
    sounds_midi_melodic_nav->child = NULL;
    sounds_midi_melodic_nav->size = (cfg->sounds_midi_melodic)->size;
    sounds_midi_melodic_nav->lcd_state = state_midi_melodic;
    sounds_midi_melodic_nav->index = 0;
    array_scroll(sounds_midi_melodic_nav, 0);

    // sounds_midi_percussion
    sounds_midi_percussion_nav->name = strdup("sounds_midi_percussion");
    sounds_midi_percussion_nav->data_array = (cfg->sounds_midi_percussion)->array;
    sounds_midi_percussion_nav->parent = sounds_midi_nav;
    sounds_midi_percussion_nav->child = NULL;
    sounds_midi_percussion_nav->size = (cfg->sounds_midi_percussion)->size;
    sounds_midi_percussion_nav->lcd_state = state_midi_percussion;
    sounds_midi_percussion_nav->index = 0;
    array_scroll(sounds_midi_percussion_nav, 0);

    // sounds_midi_octaves
    sounds_midi_octaves_nav->name = strdup("sounds_midi_octaves");
    sounds_midi_octaves_nav->data_array = octaves;
    sounds_midi_octaves_nav->parent = sounds_midi_melodic_nav;
    sounds_midi_octaves_nav->child = NULL;
    sounds_midi_octaves_nav->size = 11;
    sounds_midi_octaves_nav->lcd_state = state_midi_octaves;
    sounds_midi_octaves_nav->index = 0;
    array_scroll(sounds_midi_octaves_nav, 0);

    // sounds_midi_notes
    sounds_midi_notes_nav->name = strdup("sounds_midi_notes");
    sounds_midi_notes_nav->data_array = note_names;
    sounds_midi_notes_nav->parent = sounds_midi_octaves_nav;
    sounds_midi_notes_nav->child = NULL;
    sounds_midi_notes_nav->size = 12;
    sounds_midi_notes_nav->lcd_state = state_midi_notes;
    sounds_midi_notes_nav->index = 0;
    array_scroll(sounds_midi_notes_nav, 0);

    return main_nav;
}

void array_scroll(Nav *nav, int direction)
{

    int new_index;
    // new index changes +/- and goes from end to 0
    new_index = (nav->index + direction + nav->size) % nav->size;

    nav->index = new_index;
    nav->lcd_state[0] = format_row(nav->data_array, new_index, 1);

    // THIS IS WHERE IT REPEATS
    for (int row = 1; row < LCD_ROWS - 1; row++)
    {
        if (row < (int)nav->size)
        {
            int temp_index = (new_index + row) % nav->size;
            nav->lcd_state[row] = format_row(nav->data_array, temp_index, 0);
        }
        else
        {

            nav->lcd_state[row] = strdup("");
        }
    }
}

const char *format_row(const char **data_array, int index, int format)
{
    char *temp_str = (char *)malloc(LCD_COLUMNS + NULL_TERMINATION); // Allocate memory dynamically

    if (temp_str == NULL)
    {
        // Allocation failed
        return NULL;
    }

    // spacing, enumerated
    if (format == 0)
    {
        snprintf(temp_str, LCD_COLUMNS + NULL_TERMINATION, " %d %s", index + 1, data_array[index]);
    }
    else if (format == 1)
    {
        snprintf(temp_str, LCD_COLUMNS + NULL_TERMINATION, ">%d %s", index + 1, data_array[index]);
    }

    return temp_str;
}
const char *tracks_update(void)
{
    int max_length = 20;
    char *temp_str = (char *)malloc(max_length + 1);

    if (temp_str == NULL)
    {
        // Allocation failed
        return NULL;
    }

    // spacing, enumerated
    if (current_track->bpm != 0)
    {
        snprintf(temp_str, LCD_COLUMNS + NULL_TERMINATION, "BPM:%d STEPS:%d ID:%d ", current_track->bpm, current_track->measure_steps, current_track->id);
    }
    else
    {
        snprintf(temp_str, LCD_COLUMNS + NULL_TERMINATION, "NO TRACK SELECTED");
    }
    return temp_str;
}

void dpad_nav_routine(int dpad_pressed)
{
    /****************************     DPAD LEFT      ****************************/
    if (dpad_pressed == BUTTON_DPAD_LEFT) // return / exit
    {
        nav_state = nav_selection(nav_state, NAV_BACKWARD);

        lcd_display(lcd, nav_state->lcd_state);
    }

    /****************************     DPAD DOWN      ****************************/
    if (dpad_pressed == BUTTON_DPAD_DOWN) // scroll down
    {
        array_scroll(nav_state, NAV_DOWN);
        lcd_display(lcd, nav_state->lcd_state);
    }

    /****************************     DPAD UP        ****************************/
    if (dpad_pressed == BUTTON_DPAD_UP) // scroll up
    {
        array_scroll(nav_state, NAV_UP);
        lcd_display(lcd, nav_state->lcd_state);
    }

    /****************************     DPAD RIGHT      ***************************/
    if (dpad_pressed == BUTTON_DPAD_RIGHT) // select
    {
        /*************************     TRACKS OPTIONS     *************************/

        /*
        SAVE TRACK
        */
        if (strcmp(nav_state->data_array[nav_state->index], "Save Track") == 0)
        {
            char *new_track_filename = (char *)malloc(LCD_COLUMNS + NULL_TERMINATION);
            snprintf(new_track_filename, LCD_COLUMNS + NULL_TERMINATION, "TRACK%d.json", (nav_state->child[2])->size);
            strncpy(current_track->filename, new_track_filename, 63); // Copy up to 63 characters to ensure null-termination
            current_track->filename[63] = '\0';

            current_track->id = (nav_state->child[2])->size;
            save_track(new_track_filename, current_track);

            track_list = sd_fetch_tracks();
            (nav_state->child[2])->data_array = track_list->array;
            (nav_state->child[2])->size = track_list->size;
            (nav_state->child[2])->index = 0;
            array_scroll(nav_state->child[2], 0);
            free(new_track_filename);
        }
        /*
        DELETE TRACK
        */
        else if (strcmp(nav_state->data_array[nav_state->index], "Delete Track") == 0)
        {
            char *delete_track_filename = (char *)malloc(LCD_COLUMNS + NULL_TERMINATION);
            snprintf(delete_track_filename, LCD_COLUMNS + NULL_TERMINATION, "TRACK%d.json", current_track->id);

            if (sd_delete_track(delete_track_filename))
            {
                // Serial.print("FAILED TO DELETE: ");
                Serial.println(current_track->filename);
            }
            else
            {
                track_list = sd_fetch_tracks();
                (nav_state->child[2])->data_array = track_list->array;
                (nav_state->child[2])->size = track_list->size;
                (nav_state->child[2])->index = 0;
                array_scroll(nav_state->child[2], 0);
                read_track((nav_state->child[2])->data_array[(nav_state->child[2])->size - 1], current_track);
            }
            free(delete_track_filename);
        }
        /*
        LOAD TRACKS
        */
        else if (strcmp(nav_state->name, "tracks_load") == 0)
        {
            Serial.println("START LOAD TRACK");
            Serial.println("free cached sounds");
            free_cached_sounds(current_track);
            Serial.println("free previous track");
            delete current_track;
            // free_track(current_track);
            Serial.println("load new track");
            read_track(nav_state->data_array[nav_state->index], current_track);
            Serial.println("RETURN LOAD TRACK");
        }
        /*
        SET TRACK STEPS
        */
        else if (strcmp(nav_state->name, "tracks_set_steps") == 0)
        {
            current_track->measure_steps = nav_state->index + 1;
            for (int i = 0; i < 4; i++)
            {
                current_measure->beat_list[i].active_steps = current_track->measure_steps;
            }
        }
        /*************************     SOUNDS SELECT     **************************/
        /*
        SELECTED MELODIC MIDI SOUND
        */
        else if (strcmp(nav_state->name, "sounds_midi_notes") == 0)
        {
            Serial.println("MIDI SOUNDS SELECTION:");
            Serial.print("BANK: ");
            Serial.println(((nav_state->parent)->parent)->name);
            Serial.print("Octave: ");
            Serial.println((nav_state->parent)->data_array[(nav_state->parent)->index]);
            Serial.print("Note: ");
            Serial.println(nav_state->data_array[nav_state->index]);
            Serial.print("TEST: ");
            Serial.println(midi_mapping[nav_state->index][(nav_state->parent)->index]);

            dispBank = sounds_midi_nav->index;
            dispInstrum = midi_melodic_values[sounds_midi_melodic_nav->index];
            dispNote = midi_mapping[sounds_midi_notes_nav->index][sounds_midi_octaves_nav->index];
            dispFlag = 0;

            new_sound.bank = sounds_midi_nav->index;
            new_sound.instrument = midi_melodic_values[sounds_midi_melodic_nav->index];
            new_sound.note = midi_mapping[sounds_midi_notes_nav->index][sounds_midi_octaves_nav->index];
            new_sound.sd_cached_sound = nullptr;
            // memset(new_sound.filename, 0, sizeof(new_sound.filename));
            new_sound.filename = nullptr;
            // new_sound.filename = "";
            new_sound_assignment = true;

            Serial.println(dispBank);
            Serial.println(dispInstrum);
            Serial.println(dispNote);

            // BANK
            // sounds_midi_nav->index;

            // melodic sound
            // midi_melodic_values[sounds_midi_melodic_nav->index];

            // midi standard mapping (octave & note)
            // midi_mapping[sounds_midi_notes_nav->index][sounds_midi_octaves_nav->index]
            lcd_splash(lcd, nav_state, selected_sound);
            nav_state = main_nav;
        }
        /*
        SELECTED PERCUSSION MIDI SOUND
        */
        else if (strcmp(nav_state->name, "sounds_midi_percussion") == 0)
        {
            // BANK
            // sounds_midi_nav->index;

            // currently selected midi sound
            // midi_percussion_values[sounds_midi_percussion_nav->index];
            dispBank = sounds_midi_nav->index;
            dispInstrum = midi_percussion_values[sounds_midi_percussion_nav->index];
            dispNote = -1;
            dispFlag = 0;

            new_sound.bank = sounds_midi_nav->index;
            new_sound.instrument = midi_percussion_values[sounds_midi_percussion_nav->index];
            new_sound.note = -1;
            new_sound.sd_cached_sound = nullptr;
            // memset(new_sound.filename, 0, sizeof(new_sound.filename));
            new_sound.filename = nullptr;
            new_sound_assignment = true;

            Serial.println(dispBank);
            Serial.println(dispInstrum);
            Serial.println(dispNote);

            lcd_splash(lcd, nav_state, selected_sound);
            nav_state = main_nav;
        }
        /*
        SELECTED CUSTOM SOUND
        */
        else if (strcmp(nav_state->name, "custom_sounds") == 0)
        {
            // currently selected custom sound
            // sounds_custom_nav->data_array[sounds_custom_nav->index];
            temp_sample = cache_sd_sound(sounds_custom_nav->data_array[sounds_custom_nav->index]);
            // new_sound.sd_cached_sound = cache_sd_sound(sounds_custom_nav->data_array[sounds_custom_nav->index]);
            if (temp_sample != nullptr)
            {
                dispFlag = 3;

                new_sound.bank = -1;
                new_sound.instrument = -1;
                new_sound.note = -1;
                new_sound_assignment = true;
                new_sound.sd_cached_sound = temp_sample;

                new_sound.filename = sounds_custom_nav->data_array[sounds_custom_nav->index];
                // strlcpy(new_sound.filename,                                      // <- destination
                //         sounds_custom_nav->data_array[sounds_custom_nav->index], // <- source
                //         sizeof(new_sound.filename));

                lcd_splash(lcd, nav_state, selected_sound);

                char str[20];
                sprintf(str, "%p", (void *)temp_sample); // Using sprintf to format the pointer address
                Serial.println("\tEXPECTED: " + String(str));

                sprintf(str, "%p", (void *)new_sound.sd_cached_sound); // Using sprintf to format the pointer address
                Serial.println("\tACTUAL: " + String(str));

                current_track->cached_sounds.push_back(new_sound);
            }
            else
            {
                new_sound.bank = -1;
                new_sound.instrument = -1;
                new_sound.note = -1;
                new_sound.sd_cached_sound = nullptr;
                // memset(new_sound.filename, 0, sizeof(new_sound.filename));
                new_sound.filename = nullptr;
                // NO SIZE ON PSRAM TO CACHE SOUND
                lcd_splash(lcd, nullptr, error_psram_full);
            }
            nav_state = main_nav;
        }
        /************************     DEFAULT BEHAVIOR     ************************/
        /*
        SELECT NEXT NAV
        */
        else
        {
            nav_state = nav_selection(nav_state, NAV_FORWARD);
        }
        if (splash_screen_active == false)
        {
            lcd_display(lcd, nav_state->lcd_state);
        }
    }
}
#endif
