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

Nav *nav_selection(Nav *nav, int direction)
{
    if (direction > 0)
    {
        if (nav->child != nullptr)
        {
            return nav->child[nav->index];
        }
    }
    else if (direction < 0)
    {
        if (nav->parent != nullptr)
        {
            return nav->parent;
        }
    }
    return nullptr;
}

void array_scroll(Nav *nav, int direction)
{
    if (nav->data_array.empty())
    {
        return;
    }

    // new index changes +/- and goes from end to 0
    int new_index = (nav->index + direction + nav->data_array.size()) % nav->data_array.size();
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
            nav->lcd_state[row] = "                    ";
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

    rt_st.resize(20);
    rt_st.shrink_to_fit();
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
    rt_st.resize(20);
    rt_st.shrink_to_fit();
    return rt_st;
}

Nav *nav_init(struct nav_config *cfg)
{
    // ptr arrays
    Nav **main_child = new Nav *[4];
    main_child[0] = sounds_nav;
    main_child[1] = effects_nav;
    main_child[2] = tracks_nav;
    main_child[3] = delete_nav;

    Nav **sounds_child = new Nav *[2];
    sounds_child[0] = sounds_custom_nav;
    sounds_child[1] = sounds_midi_nav;

    Nav **midi_child = new Nav *[2];
    midi_child[0] = sounds_midi_percussion_nav;
    midi_child[1] = sounds_midi_melodic_nav;

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
    effects_nav->data_array = std::move(cfg->effects);
    effects_nav->parent = main_nav;
    effects_nav->child = nullptr;
    effects_nav->index = 0;
    effects_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(effects_nav, 0);

    // Tracks presets
    std::vector<std::string> tracks_preset_options = {"Save Track", "Load Track", "Delete Track", "Set # beats", "Set # global steps", "Set # local steps"};

    // tracks_nav->name = "tracks";
    tracks_nav->id = NAVIGATION_TRACKS;
    tracks_nav->data_array = std::move(tracks_preset_options);
    tracks_nav->parent = main_nav;
    tracks_nav->child = nullptr;
    tracks_nav->index = 0;
    tracks_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(tracks_nav, 0);

    // Tracks presets
    std::vector<std::string> delete_preset_options = {"Delete last sound", "Delete all sounds on step", "Delete whole measure"};

    // delete_nav->name = "tracks";
    delete_nav->id = NAVIGATION_DELETE;
    delete_nav->data_array = std::move(delete_preset_options);
    delete_nav->parent = main_nav;
    delete_nav->child = nullptr;
    delete_nav->index = 0;
    delete_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(delete_nav, 0);

    // Tracks Save presets
    std::vector<std::string> tracks_preset_options_save = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    tracks_save_nav->id = NAVIGATION_TRACK_SAVE;
    tracks_save_nav->data_array = std::move(tracks_preset_options_save);
    tracks_save_nav->parent = tracks_nav;
    tracks_save_nav->child = nullptr;
    tracks_save_nav->index = 0;
    tracks_save_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(tracks_save_nav, 0);

    // Tracks Load presets
    // tracks_load_nav->name = "tracks_load";
    tracks_load_nav->id = NAVIGATION_TRACK_LOAD;
    tracks_load_nav->data_array = std::move(cfg->tracks_load);
    tracks_load_nav->parent = tracks_nav;
    tracks_load_nav->child = nullptr;
    tracks_load_nav->index = 0;
    tracks_load_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(tracks_load_nav, 0);

    // Tracks Set Global Beats presets
    std::vector<std::string> tracks_preset_options_beats = {" Beat", " Beat", " Beat", " Beat"};
    tracks_set_beats_nav->id = NAVIGATION_SET_BEATS;
    tracks_set_beats_nav->data_array = std::move(tracks_preset_options_beats);
    tracks_set_beats_nav->parent = tracks_nav;
    tracks_set_beats_nav->child = nullptr;
    tracks_set_beats_nav->index = 0;
    tracks_set_beats_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(tracks_set_beats_nav, 0);

    // Tracks Set Global Steps presets
    std::vector<std::string> tracks_preset_options_steps = {"Step", "Step", "Step", "Step", "Step", "Step"};
    tracks_set_steps_nav->id = NAVIGATION_SET_STEPS;
    tracks_set_steps_nav->data_array = std::move(tracks_preset_options_steps);
    tracks_set_steps_nav->parent = tracks_nav;
    tracks_set_steps_nav->child = nullptr;
    tracks_set_steps_nav->index = 0;
    tracks_set_steps_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(tracks_set_steps_nav, 0);

    // Custom Sounds presets
    // sounds_custom_nav->name = "custom_sounds";
    sounds_custom_nav->id = NAVIGATION_SOUNDS_CUSTOM;
    sounds_custom_nav->data_array = std::move(cfg->sounds_custom);
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

    // MIDI Percussion presets
    // sounds_midi_percussion_nav->name = "sounds_midi_percussion";
    sounds_midi_percussion_nav->id = NAVIGATION_SOUNDS_MIDI_PERCUSSION;
    sounds_midi_percussion_nav->data_array = std::move(cfg->sounds_midi_percussion);
    sounds_midi_percussion_nav->parent = sounds_midi_nav;
    sounds_midi_percussion_nav->child = nullptr;
    sounds_midi_percussion_nav->index = 0;
    sounds_midi_percussion_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_midi_percussion_nav, 0);

    // MIDI Melodic presets
    // sounds_midi_melodic_nav->name = "sounds_midi_melodic";
    sounds_midi_melodic_nav->id = NAVIGATION_SOUNDS_MIDI_MELODIC;
    sounds_midi_melodic_nav->data_array = std::move(cfg->sounds_midi_melodic);
    sounds_midi_melodic_nav->parent = sounds_midi_nav;
    sounds_midi_melodic_nav->child = nullptr;
    sounds_midi_melodic_nav->index = 0;
    sounds_midi_melodic_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_midi_melodic_nav, 0);

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
    {
        if (nav_state->parent != nullptr)
        {
            if (track_save_panel == 0)
            {
                tracks_save_nav->parent = tracks_nav;
            }
            if (nav_state->id == NAVIGATION_TRACK_SAVE)
            {
                track_save_panel--;
            }
            nav_state = nav_state->parent;
            Serial.print("MOVED TO NODE:");
            Serial.println(nav_state->id);
        }
        else
        {
            Serial.print("ALREADY AT ROOT");
        }
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case BUTTON_DPAD_DOWN:
    {
        array_scroll(nav_state, NAV_DOWN);
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case BUTTON_DPAD_UP:
    {
        array_scroll(nav_state, NAV_UP);
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }

    case BUTTON_DPAD_RIGHT:
    {
        if (nav_state->child != nullptr)
        {
            nav_state = nav_state->child[nav_state->index];
            Serial.print("MOVED TO NODE:");
            Serial.println(nav_state->id);
            lcd_display(lcd, nav_state->lcd_state);
        }
        else
        {
            Serial.println("REACHED SELECTION LEAF");
            execute_leaf();
        }

        break;
    }

    default:
        // lcd_display(lcd, nav_state->lcd_state);
        break;
    }
}

int execute_leaf(void)
{
    switch (nav_state->id)
    {
    case NAVIGATION_TRACKS:
    {
        Serial.println("NAVIGATION_TRACKS");
        track_options();
        break;
    }
    case NAVIGATION_SOUNDS_CUSTOM:
    {
        Serial.println("NAVIGATION_SOUNDS_CUSTOM");
        temp_sample = cache_sd_sound(sounds_custom_nav->data_array[sounds_custom_nav->index]);
        if (temp_sample != nullptr)
        {

            new_sound.bank = -1;
            new_sound.instrument = -1;
            new_sound.note = -1;
            new_sound.sd_cached_sound = temp_sample;
            new_sound.filename = sounds_custom_nav->data_array[sounds_custom_nav->index];

            new_palette_slot.sound.bank = -1;
            new_palette_slot.sound.instrument = -1;
            new_palette_slot.sound.note = -1;
            new_palette_slot.sound.sd_cached_sound = temp_sample;
            new_palette_slot.sound.filename = sounds_custom_nav->data_array[sounds_custom_nav->index];
            new_palette_slot.effect = -1;
            new_palette_slot.is_empty = false;

            // new_sound_assignment = true;
            palette_assignment = PALETTE_ASSIGNMENT_SOUND;
            LED_mode = LED_PALETTE_SELECT;
            lcd_splash(lcd, nav_state, selected_sound);

            char str[20];
            sprintf(str, "%p", (void *)temp_sample); // Using sprintf to format the pointer address
            Serial.println("\tEXPECTED: " + String(str));

            // current_track->cached_sounds.push_back(new_sound);
            current_track->cached_sounds.push_back(new_palette_slot.sound);
        }
        else
        {
            new_sound.bank = -1;
            new_sound.instrument = -1;
            new_sound.note = -1;
            new_sound.sd_cached_sound = nullptr;
            new_sound.filename = "";

            new_palette_slot.sound.bank = -1;
            new_palette_slot.sound.instrument = -1;
            new_palette_slot.sound.note = -1;
            new_palette_slot.sound.sd_cached_sound = nullptr;
            new_palette_slot.sound.filename = "";
            new_palette_slot.effect = -1;
            new_palette_slot.is_empty = true;

            // NO SIZE ON PSRAM TO CACHE SOUND
            lcd_splash(lcd, nullptr, error_psram_full);
        }
        nav_state = main_nav;
        break;
    }
    case NAVIGATION_EFFECTS:
    {
        Serial.println("NAVIGATION_EFFECTS");
        new_palette_slot.sound.bank = -1;
        new_palette_slot.sound.instrument = -1;
        new_palette_slot.sound.note = -1;
        new_palette_slot.sound.sd_cached_sound = nullptr;
        new_palette_slot.sound.filename = "";
        new_palette_slot.effect = nav_state->index;
        new_palette_slot.is_empty = false;

        palette_assignment = PALETTE_ASSIGNMENT_EFFECT;
        LED_mode = LED_PALETTE_SELECT;
        lcd_splash(lcd, nav_state, selected_effect);

        nav_state = main_nav;
        break;
    }
    case NAVIGATION_SOUNDS_MIDI_PERCUSSION:
    {
        Serial.println("NAVIGATION_SOUNDS_MIDI_PERCUSSION");
        new_sound.bank = sounds_midi_nav->index;
        new_sound.instrument = midi_percussion_values[sounds_midi_percussion_nav->index];
        new_sound.note = -1;
        new_sound.sd_cached_sound = nullptr;
        new_sound.filename = "";

        new_palette_slot.sound.bank = sounds_midi_nav->index;
        new_palette_slot.sound.instrument = midi_percussion_values[sounds_midi_percussion_nav->index];
        new_palette_slot.sound.note = -1;
        new_palette_slot.sound.sd_cached_sound = nullptr;
        new_palette_slot.sound.filename = "";
        new_palette_slot.effect = -1;
        new_palette_slot.is_empty = false;

        // new_sound_assignment = true;
        palette_assignment = PALETTE_ASSIGNMENT_SOUND;
        LED_mode = LED_PALETTE_SELECT;

        lcd_splash(lcd, nav_state, selected_sound);
        nav_state = main_nav;
        break;
    }

    case NAVIGATION_SOUNDS_MIDI_MELODIC:
    {
        Serial.println("NAVIGATION_SOUNDS_MIDI_MELODIC");
        nav_state = sounds_midi_octaves_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }

    case NAVIGATION_MIDI_OCTAVES:
    {
        Serial.println("NAVIGATION_MIDI_OCTAVES");
        nav_state = sounds_midi_notes_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }

    case NAVIGATION_MIDI_NOTES:
    {
        Serial.println("NAVIGATION_MIDI_NOTES");
        Serial.println("MIDI SOUNDS SELECTION:");
        // Serial.print("BANK: ");
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

        new_palette_slot.sound.bank = sounds_midi_nav->index;
        new_palette_slot.sound.instrument = midi_melodic_values[sounds_midi_melodic_nav->index];
        new_palette_slot.sound.note = midi_mapping[sounds_midi_notes_nav->index][sounds_midi_octaves_nav->index];
        new_palette_slot.sound.sd_cached_sound = nullptr;
        new_palette_slot.sound.filename = "";
        new_palette_slot.effect = -1;
        new_palette_slot.is_empty = false;
        // new_sound_assignment = true;
        palette_assignment = PALETTE_ASSIGNMENT_SOUND;
        LED_mode = LED_PALETTE_SELECT;

        lcd_splash(lcd, nav_state, selected_sound);
        nav_state = main_nav;
        break;
    }
    case NAVIGATION_SET_BEATS:
    {
        Serial.println("NAVIGATION_SET_BEATS");

        if (tracks_nav->index == LEAF_TRACKS_GLOBAL_BEATS)
        {
            current_track->measure_list[0].active_beats = tracks_set_beats_nav->index + 1;
        }
        else if (tracks_nav->index == LEAF_TRACKS_LOCAL_STEPS)
        {
            nav_state = tracks_set_steps_nav;
            lcd_display(lcd, nav_state->lcd_state);
        }

        break;
    }
    case NAVIGATION_SET_STEPS:
    {
        Serial.println("NAVIGATION_SET_STEPS");
        if (tracks_nav->index == LEAF_TRACKS_GLOBAL_STEPS)
        {
            current_track->measure_steps = nav_state->index + 1;
            for (int i = 0; i < 4; i++)
            {
                current_measure->beat_list[i].active_steps = current_track->measure_steps;
            }
        }
        else if (tracks_nav->index == LEAF_TRACKS_LOCAL_STEPS)
        {
            nav_state = tracks_set_steps_nav;

            // COMPLETE
            current_measure->beat_list[tracks_set_beats_nav->index].active_steps = tracks_set_steps_nav->index + 1;
        }
        break;
    }
    case NAVIGATION_TRACK_SAVE:
    {
        Serial.println("NAVIGATION_TRACK_SAVE");
        if (track_save_panel < TRACK_SAVE_NAME_LENGTH - 1)
        {
            track_save_string.replace(track_save_panel, 1, nav_state->data_array[nav_state->index]);
            track_save_panel++;
            tracks_save_nav->parent = tracks_save_nav;
        }
        else
        {
            track_save_string.replace(track_save_panel, 1, nav_state->data_array[nav_state->index]);

            current_track->filename = "TRACK-" + track_save_string + ".json";
            save_track(current_track->filename, current_track);
            tracks_load_nav->data_array.clear();
            tracks_load_nav->data_array = std::move(sd_fetch_tracks());
            tracks_load_nav->index = 0;
            array_scroll(tracks_load_nav, 0);
            nav_state = tracks_nav;
            lcd_display(lcd, nav_state->lcd_state);

            // int filename_count = 0;
            // if (tracks_load_nav->data_array.empty() == false)
            // {
            //     filename_count = tracks_load_nav->data_array.size();
            // }
            // // std::string new_track_filename = "TRACK" + std::to_string(filename_count) + ".json";
            // current_track->filename = "TRACK" + std::to_string(filename_count) + ".json";
            // save_track(current_track->filename, current_track);
            // tracks_load_nav->data_array.clear();
            // tracks_load_nav->data_array = std::move(sd_fetch_tracks());
            // tracks_load_nav->index = 0;
            // array_scroll(tracks_load_nav, 0);
        }
        break;
    }
    case NAVIGATION_TRACK_LOAD:
    {
        Serial.println("NAVIGATION_TRACK_LOAD");
        free_cached_sounds(current_track);
        Serial.println("free previous track");
        delete current_track;
        Serial.println("load new track");
        read_track(nav_state->data_array[nav_state->index], current_track);
        Serial.println("RETURN LOAD TRACK");
        break;
    }
    }
    return 0;
}
int track_options(void)
{

    switch (nav_state->index)
    {

    case LEAF_TRACKS_SAVE:
    {
        Serial.println("LEAF_TRACKS_SAVE");
        track_save_panel = 0;
        track_save_string.clear();
        tracks_save_nav->parent = tracks_nav;
        nav_state = tracks_save_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case LEAF_TRACKS_LOAD:
    {
        Serial.println("LEAF_TRACKS_LOAD");
        nav_state = tracks_load_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case LEAF_TRACKS_DELETE:
    {
        Serial.println("LEAF_TRACKS_DELETE");
        if (tracks_load_nav->data_array.empty())
        {
            break;
        }
        if (sd_delete_track(current_track->filename))
        {
            Serial.print("FAILED TO DELETE: ");
            Serial.println(current_track->filename.c_str());
        }
        else
        {
            tracks_load_nav->data_array.clear();
            tracks_load_nav->data_array = std::move(sd_fetch_tracks());
            tracks_load_nav->index = 0;
            array_scroll(tracks_load_nav, 0);
            if (tracks_load_nav->data_array.empty() == false)
            {
                read_track(tracks_load_nav->data_array[0], current_track);
            }
        }
        break;
    }
    case LEAF_TRACKS_GLOBAL_BEATS:
    {
        Serial.println("LEAF_TRACKS_GLOBAL_BEATS");
        nav_state = tracks_set_beats_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case LEAF_TRACKS_GLOBAL_STEPS:
    {
        Serial.println("LEAF_TRACKS_GLOBAL_STEPS");
        tracks_set_steps_nav->parent = tracks_nav;
        nav_state = tracks_set_steps_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case LEAF_TRACKS_LOCAL_STEPS:
    {
        Serial.println("LEAF_TRACKS_LOCAL_STEPS");
        tracks_set_steps_nav->parent = tracks_set_beats_nav;
        nav_state = tracks_set_beats_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    }
    return 0;
}
