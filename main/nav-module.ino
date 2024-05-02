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
    nav->lcd_state[0] = format_row(nav->data_array, new_index, FORMAT_ROW_INDEX_SELECT);

    // THIS IS WHERE IT REPEATS
    for (size_t row = 1; row < LCD_ROWS - 1; row++)
    {
        if (row < nav->data_array.size())
        {
            int temp_index = (new_index + row) % nav->data_array.size();
            nav->lcd_state[row] = format_row(nav->data_array, temp_index, FORMAT_ROW_INDEX_SPACED);
        }
        else
        {
            nav->lcd_state[row] = std::string(LCD_COLUMNS, ' ');
        }
    }
}

std::string format_row(std::vector<std::string> data_array, int index, int format)
{
    std::string rt_st;
    if (!data_array[index].empty())
    {

        switch (format)
        {
        case FORMAT_ROW_INDEX_SPACED:
        {
            rt_st = " " + std::to_string(index + 1) + " " + std::string(data_array[index]);
            break;
        }
        case FORMAT_ROW_INDEX_SELECT:
        {
            rt_st = ">" + std::to_string(index + 1) + " " + std::string(data_array[index]);
            break;
        }
        case FORMAT_ROW_INDEX_NONE:
        {
            rt_st = std::to_string(index + 1) + " " + std::string(data_array[index]);
            break;
        }
        case FORMAT_ROW_CENTER:
        {
            if (data_array[index].size() < LCD_COLUMNS)
            {
                rt_st.assign((LCD_COLUMNS - data_array[index].size()) / 2, ' ');
                rt_st.append(data_array[index]);
            }
            break;
        }
        }
    }
    rt_st.resize(LCD_COLUMNS, ' ');
    return rt_st;
}

std::string tracks_update(void)
{
    std::string rt_st;

    // spacing, enumerated
    if (current_track->bpm != 0)
    {
        rt_st = "BPM:" + std::to_string(current_track->bpm) + " M:" + std::to_string(current_measure->id + 1) + " ID:" + current_track->filename.substr(6, 3);
    }
    else
    {
        rt_st = "NO TRACK SELECTED";
    }
    rt_st.resize(LCD_COLUMNS);
    rt_st.shrink_to_fit();
    return rt_st;
}

Nav *nav_init(struct nav_config *cfg)
{
    // ptr arrays
    Nav **main_child = new Nav *[5];
    main_child[0] = sounds_nav;
    main_child[1] = effects_nav;
    main_child[2] = measure_nav;
    main_child[3] = tracks_nav;
    main_child[4] = delete_nav;

    Nav **sounds_child = new Nav *[2];
    sounds_child[0] = sounds_custom_nav;
    sounds_child[1] = sounds_midi_nav;

    Nav **midi_child = new Nav *[2];
    midi_child[0] = sounds_midi_percussion_nav;
    midi_child[1] = sounds_midi_melodic_nav;

    // MAIN
    std::vector<std::string> main_preset_options = {"Sounds", "Effects", "Measures", "Tracks", "Delete", "(PAUSE)"};
    main_nav->id = NAVIGATION_MAIN;
    main_nav->name = "Main";
    main_nav->data_array = std::move(main_preset_options);
    main_nav->parent = nullptr;
    main_nav->child = main_child;
    main_nav->index = 0;
    main_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(main_nav, 0);

    // SOUNDS
    std::vector<std::string> sounds_preset_options = {"Custom Sounds", "MIDI Sounds"};
    sounds_nav->id = NAVIGATION_SOUNDS;
    sounds_nav->name = "Sounds";
    sounds_nav->data_array = std::move(sounds_preset_options);
    sounds_nav->parent = main_nav;
    sounds_nav->child = sounds_child;
    sounds_nav->index = 0;
    sounds_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_nav, 0);

    // EFFECTS
    effects_nav->id = NAVIGATION_EFFECTS;
    effects_nav->name = "Effects";
    effects_nav->data_array = std::move(cfg->effects);
    effects_nav->parent = main_nav;
    effects_nav->child = nullptr;
    effects_nav->index = 0;
    effects_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(effects_nav, 0);

    // MEASURES
    std::vector<std::string> measure_preset_options = {"Add Measure", "Delete Measure", "Swap Measure", "Set # beats", "Set # global steps", "Set # local steps"};
    measure_nav->id = NAVIGATION_MEASURE;
    measure_nav->name = "Measures";
    measure_nav->data_array = std::move(measure_preset_options);
    measure_nav->parent = main_nav;
    measure_nav->child = nullptr;
    measure_nav->index = 0;
    measure_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(measure_nav, 0);

    // TRACKS
    std::vector<std::string> tracks_preset_options = {"Save Track", "Load Track", "Delete Track"};
    tracks_nav->id = NAVIGATION_TRACKS;
    tracks_nav->name = "Tracks";
    tracks_nav->data_array = std::move(tracks_preset_options);
    tracks_nav->parent = main_nav;
    tracks_nav->child = nullptr;
    tracks_nav->index = 0;
    tracks_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(tracks_nav, 0);

    // DELETE
    std::vector<std::string> delete_preset_options = {"Delete last sound", "Delete all sounds on step", "Delete whole measure"};
    delete_nav->id = NAVIGATION_DELETE;
    delete_nav->name = "Delete";
    delete_nav->data_array = std::move(delete_preset_options);
    delete_nav->parent = main_nav;
    delete_nav->child = nullptr;
    delete_nav->index = 0;
    delete_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(delete_nav, 0);

    // Tracks Save presets
    std::vector<std::string> tracks_preset_options_save = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    tracks_save_nav->id = NAVIGATION_TRACK_SAVE;
    tracks_save_nav->name = "Tracks Save";
    tracks_save_nav->data_array = std::move(tracks_preset_options_save);
    tracks_save_nav->parent = tracks_nav;
    tracks_save_nav->child = nullptr;
    tracks_save_nav->index = 0;
    tracks_save_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(tracks_save_nav, 0);

    // Tracks Load presets
    tracks_load_nav->id = NAVIGATION_TRACK_LOAD;
    tracks_load_nav->name = "Tracks Load";
    tracks_load_nav->data_array = std::move(cfg->tracks_load);
    tracks_load_nav->parent = tracks_nav;
    tracks_load_nav->child = nullptr;
    tracks_load_nav->index = 0;
    tracks_load_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(tracks_load_nav, 0);

    // Measures Select
    std::vector<std::string> measure_select_preset_options = {"Measure"};
    measure_select_nav->id = NAVIGATION_MEASURE_SELECT;
    measure_select_nav->name = "Measures Select";
    measure_select_nav->data_array = std::move(measure_select_preset_options);
    measure_select_nav->parent = measure_nav;
    measure_select_nav->child = nullptr;
    measure_select_nav->index = 0;
    measure_select_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(measure_select_nav, 0);

    // Measure Beats
    std::vector<std::string> measures_preset_options_beats = {" Beat", " Beat", " Beat", " Beat"};
    measures_set_beats_nav->id = NAVIGATION_SET_BEATS;
    measures_set_beats_nav->name = "Tracks Beats";
    measures_set_beats_nav->data_array = std::move(measures_preset_options_beats);
    measures_set_beats_nav->parent = measure_select_nav;
    measures_set_beats_nav->child = nullptr;
    measures_set_beats_nav->index = 0;
    measures_set_beats_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(measures_set_beats_nav, 0);

    // Measure Steps
    std::vector<std::string> measures_preset_options_steps = {"Step", "Step", "Step", "Step", "Step", "Step"};
    measures_set_steps_nav->id = NAVIGATION_SET_STEPS;
    measures_set_steps_nav->name = "Tracks Steps";
    measures_set_steps_nav->data_array = std::move(measures_preset_options_steps);
    measures_set_steps_nav->parent = measure_select_nav;
    measures_set_steps_nav->child = nullptr;
    measures_set_steps_nav->index = 0;
    measures_set_steps_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(measures_set_steps_nav, 0);

    // Custom Sounds
    sounds_custom_nav->id = NAVIGATION_SOUNDS_CUSTOM;
    sounds_custom_nav->name = "Custom Sounds";
    sounds_custom_nav->data_array = std::move(cfg->sounds_custom);
    sounds_custom_nav->parent = sounds_nav;
    sounds_custom_nav->child = nullptr;
    sounds_custom_nav->index = 0;
    sounds_custom_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_custom_nav, 0);

    // MIDI Sounds presets
    std::vector<std::string> midi_preset_options = {"Percussion Instruments", "Melodic Instruments"};
    sounds_midi_nav->id = NAVIGATION_SOUNDS_MIDI;
    sounds_midi_nav->name = "MIDI Sounds";
    sounds_midi_nav->data_array = std::move(midi_preset_options);
    sounds_midi_nav->parent = sounds_nav;
    sounds_midi_nav->child = midi_child;
    sounds_midi_nav->index = 0;
    sounds_midi_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_midi_nav, 0);

    // MIDI Percussion presets
    sounds_midi_percussion_nav->id = NAVIGATION_SOUNDS_MIDI_PERCUSSION;
    sounds_midi_percussion_nav->name = "MIDI Percussion Instruments";
    sounds_midi_percussion_nav->data_array = std::move(cfg->sounds_midi_percussion);
    sounds_midi_percussion_nav->parent = sounds_midi_nav;
    sounds_midi_percussion_nav->child = nullptr;
    sounds_midi_percussion_nav->index = 0;
    sounds_midi_percussion_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_midi_percussion_nav, 0);

    // MIDI Melodic presets
    sounds_midi_melodic_nav->id = NAVIGATION_SOUNDS_MIDI_MELODIC;
    sounds_midi_melodic_nav->name = "MIDI Melodic Instruments";
    sounds_midi_melodic_nav->data_array = std::move(cfg->sounds_midi_melodic);
    sounds_midi_melodic_nav->parent = sounds_midi_nav;
    sounds_midi_melodic_nav->child = nullptr;
    sounds_midi_melodic_nav->index = 0;
    sounds_midi_melodic_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_midi_melodic_nav, 0);

    // MIDI Octaves presets
    sounds_midi_octaves_nav->id = NAVIGATION_MIDI_OCTAVES;
    sounds_midi_octaves_nav->name = "MIDI Octaves";
    sounds_midi_octaves_nav->data_array = std::move(octaves);
    sounds_midi_octaves_nav->parent = sounds_midi_melodic_nav;
    sounds_midi_octaves_nav->child = nullptr;
    sounds_midi_octaves_nav->index = 0;
    sounds_midi_octaves_nav->lcd_state.resize(LCD_ROWS);
    array_scroll(sounds_midi_octaves_nav, 0);

    // MIDI Notes presets
    sounds_midi_notes_nav->id = NAVIGATION_MIDI_NOTES;
    sounds_midi_notes_nav->name = "MIDI Notes";
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
            if (nav_state->id == NAVIGATION_TRACK_SAVE)
            {
                if (track_save_panel == 0)
                {
                    lcd_mode = LCD_DEFAULT;
                    tracks_save_nav->parent = tracks_nav;
                }
                else
                {
                    track_save_panel--;
                    track_save_string.replace(track_save_panel, 1, "_");
                    lcd_display_banner(lcd, BANNER_TRACK_SAVE, LCD_PERSIST);
                }
            }
            if (nav_state->id == NAVIGATION_MEASURE_SELECT)
            {
                if (measure_swap_panel == 1)
                {

                    measure_swap_panel = 0;
                    break;
                }
            }
            nav_state = nav_state->parent;
            Serial.print("MOVED TO NODE:");
            Serial.println(nav_state->id);
        }
        else
        {
            Serial.print("ALREADY AT ROOT");
            if (lcd_mode == LCD_SPLASH)
            {
                palette_assignment = PALETTE_ASSIGNMENT_DEFAULT;
                LED_mode = LED_DEFAULT_MODE;
                new_palette_slot.sound.bank = -1;
                new_palette_slot.sound.instrument = -1;
                new_palette_slot.sound.note = -1;
                new_palette_slot.sound.sd_cached_sound = nullptr;
                new_palette_slot.sound.filename = "";
                new_palette_slot.sound.empty = true;
                new_palette_slot.effect = -1;
                new_palette_slot.is_empty = true;
            }
        }
        lcd_display(lcd, nav_state->lcd_state);
        if (nav_state->id == NAVIGATION_TRACK_SAVE)
        {
            lcd_display_banner(lcd, BANNER_TRACK_SAVE, LCD_PERSIST);
        }
        else
        {
            lcd_display_banner(lcd, BANNER_NAV_NAME, LCD_VANISH);
        }

        break;
    }
    case BUTTON_DPAD_DOWN:
    {
        array_scroll(nav_state, NAV_DOWN);
        lcd_display(lcd, nav_state->lcd_state);
        if (nav_state->id == NAVIGATION_TRACK_SAVE)
        {
            lcd_display_banner(lcd, BANNER_TRACK_SAVE, LCD_PERSIST);
        }
        else
        {
            lcd_display_banner(lcd, BANNER_DEFAULT, LCD_PERSIST);
        }
        break;
    }
    case BUTTON_DPAD_UP:
    {
        array_scroll(nav_state, NAV_UP);
        lcd_display(lcd, nav_state->lcd_state);
        if (nav_state->id == NAVIGATION_TRACK_SAVE)
        {
            lcd_display_banner(lcd, BANNER_TRACK_SAVE, LCD_PERSIST);
        }
        else
        {
            lcd_display_banner(lcd, BANNER_DEFAULT, LCD_PERSIST);
        }
        break;
    }

    case BUTTON_DPAD_RIGHT:
    {
        if (nav_state->child != nullptr)
        {
            if (nav_state->id == NAVIGATION_MAIN && nav_state->index == nav_state->data_array.size() - 1)
            {
                play_pause_toggle ^= 1;
                if (play_pause_toggle)
                {
                    main_nav->data_array[main_nav->data_array.size() - 1] = "(PAUSE)";
                    // LED_Off(current_measure->beat, current_measure->step);
                    LED_routine(current_measure->beat, current_measure->step);
                }
                else
                {
                    main_nav->data_array[main_nav->data_array.size() - 1] = "(PLAY)";
                    // LED_On(current_measure->beat, current_measure->step);
                    LED_routine(current_measure->beat, current_measure->step);
                    step_timer.reset();
                }
                array_scroll(nav_state, 0);
                lcd_display(lcd, nav_state->lcd_state);
            }
            else
            {
                nav_state = nav_state->child[nav_state->index];
                Serial.print("MOVED TO NODE:");
                Serial.println(nav_state->id);
                lcd_display(lcd, nav_state->lcd_state);
                lcd_display_banner(lcd, BANNER_NAV_NAME, LCD_VANISH);
            }
        }
        else
        {
            Serial.println("REACHED SELECTION LEAF");
            execute_leaf();
        }

        break;
    }
    }
}

int execute_leaf(void)
{
    switch (nav_state->id)
    {
    case NAVIGATION_MEASURE:
    {
        Serial.println("NAVIGATION_MEASURE");
        measure_options();
        break;
    }

    case NAVIGATION_TRACKS:
    {
        Serial.println("NAVIGATION_TRACKS");
        track_options();
        break;
    }
    case NAVIGATION_DELETE:
    {
        Serial.println("NAVIGATION_DELETE");
        delete_options();
        break;
    }
    case NAVIGATION_SOUNDS_CUSTOM:
    {
        Serial.println("NAVIGATION_SOUNDS_CUSTOM");
        Sound *new_csound = cache_sd_sound(sounds_custom_nav->data_array[sounds_custom_nav->index]);

        /*  CHECK IF SOUND ALREADY CACHED    */
        if (new_csound == nullptr) // not cachable
        {

            Serial.println("SOUND NOT FOUND IN CACHE AND UNABLE TO BE CACHED");
            new_palette_slot.sound.bank = -1;
            new_palette_slot.sound.instrument = -1;
            new_palette_slot.sound.note = -1;
            new_palette_slot.sound.sd_cached_sound = nullptr;
            new_palette_slot.sound.filename = "";
            new_palette_slot.sound.empty = true;
            new_palette_slot.effect = -1;
            new_palette_slot.is_empty = true;

            // NO SIZE ON PSRAM TO CACHE SOUND
            lcd_splash(lcd, nullptr, error_psram_full);
            run_nav_name = false;
            nav_state = main_nav;
            break;
        }
        Serial.println("SOUND FOUND IN CACHE");
        new_palette_slot.sound = *new_csound;

        new_palette_slot.effect = EFFECT_NULL;
        new_palette_slot.is_empty = false;

        palette_assignment = PALETTE_ASSIGNMENT_SOUND;
        lcd_splash(lcd, nav_state, selected_sound);
        run_nav_name = false;
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
        new_palette_slot.sound.empty = true;
        new_palette_slot.effect = nav_state->index;
        new_palette_slot.is_empty = false;

        palette_assignment = PALETTE_ASSIGNMENT_EFFECT;
        // LED_mode = LED_PALETTE_SELECT;
        lcd_splash(lcd, nav_state, selected_effect);
        run_nav_name = false;

        nav_state = main_nav;
        break;
    }
    case NAVIGATION_SOUNDS_MIDI_PERCUSSION:
    {
        Serial.println("NAVIGATION_SOUNDS_MIDI_PERCUSSION");

        new_palette_slot.sound.bank = sounds_midi_nav->index;
        new_palette_slot.sound.instrument = midi_percussion_values[sounds_midi_percussion_nav->index];
        new_palette_slot.sound.note = -1;
        new_palette_slot.sound.sd_cached_sound = nullptr;
        new_palette_slot.sound.filename = "";
        new_palette_slot.sound.empty = false;
        new_palette_slot.effect = -1;
        new_palette_slot.is_empty = false;

        // new_sound_assignment = true;
        palette_assignment = PALETTE_ASSIGNMENT_SOUND;
        // LED_mode = LED_PALETTE_SELECT;

        lcd_splash(lcd, nav_state, selected_sound);
        run_nav_name = false;
        nav_state = main_nav;
        break;
    }

    case NAVIGATION_SOUNDS_MIDI_MELODIC:
    {
        Serial.println("NAVIGATION_SOUNDS_MIDI_MELODIC");
        nav_state = sounds_midi_octaves_nav;
        lcd_display(lcd, nav_state->lcd_state);
        // lcd_display_banner(lcd, BANNER_NAV_NAME, LCD_VANISH);
        break;
    }

    case NAVIGATION_MIDI_OCTAVES:
    {
        Serial.println("NAVIGATION_MIDI_OCTAVES");
        nav_state = sounds_midi_notes_nav;
        lcd_display(lcd, nav_state->lcd_state);
        // lcd_display_banner(lcd, BANNER_NAV_NAME, LCD_VANISH);
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

        new_palette_slot.sound.bank = sounds_midi_nav->index;
        new_palette_slot.sound.instrument = midi_melodic_values[sounds_midi_melodic_nav->index];
        new_palette_slot.sound.note = midi_mapping[sounds_midi_notes_nav->index][sounds_midi_octaves_nav->index];
        new_palette_slot.sound.sd_cached_sound = nullptr;
        new_palette_slot.sound.filename = "";
        new_palette_slot.sound.empty = false;
        new_palette_slot.effect = -1;
        new_palette_slot.is_empty = false;
        palette_assignment = PALETTE_ASSIGNMENT_SOUND;

        lcd_splash(lcd, nav_state, selected_sound);
        run_nav_name = false;
        // lcd_display_banner(lcd, BANNER_NAV_NAME, LCD_VANISH);
        nav_state = main_nav;
        break;
    }
    case NAVIGATION_SET_BEATS:
    {
        Serial.println("NAVIGATION_SET_BEATS");
        if (measure_nav->index == LEAF_MEASURES_GLOBAL_BEATS)
        {
            current_track->measure_list[measure_select_nav->index]->active_beats = measures_set_beats_nav->index + 1;
            nav_state = measure_nav;
            lcd_display(lcd, nav_state->lcd_state);
        }
        else if (measure_nav->index == LEAF_MEASURES_LOCAL_STEPS)
        {
            nav_state = measures_set_steps_nav;
            lcd_display(lcd, nav_state->lcd_state);
        }

        break;
    }
    case NAVIGATION_SET_STEPS:
    {
        Serial.println("NAVIGATION_SET_STEPS");
        if (measure_nav->index == LEAF_MEASURES_GLOBAL_STEPS)
        {
            for (int i = 0; i < 4; i++)
            {
                current_track->measure_list[measure_select_nav->index]->beat_list[i].active_steps = nav_state->index + 1;
            }
        }
        else if (measure_nav->index == LEAF_MEASURES_LOCAL_STEPS)
        {
            current_track->measure_list[measure_select_nav->index]->beat_list[measures_set_beats_nav->index].active_steps = measures_set_steps_nav->index + 1;
        }

        nav_state = measure_nav;
        lcd_display(lcd, nav_state->lcd_state);
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
            run_nav_name = false;
            lcd_display_banner(lcd, BANNER_TRACK_SAVE, LCD_PERSIST);
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
            lcd_splash(lcd, nav_state, track_saved_splash);
            nav_state = tracks_nav;
            run_nav_name = false;
        }
        break;
    }
    case NAVIGATION_TRACK_LOAD:
    {
        Serial.println("NAVIGATION_TRACK_LOAD");
        Serial.println("free previous track");
        free_track(current_track);
        Serial.println("load new track");
        read_track(nav_state->data_array[nav_state->index], current_track);
        Serial.println("RETURN LOAD TRACK");

        lcd_splash(lcd, nav_state, track_loaded_splash);
        nav_state = tracks_nav;
        run_nav_name = false;
        break;
    }
    case NAVIGATION_MEASURE_SELECT:
    {
        Serial.println("NAVIGATION_MEASURE_SELECT");
        measure_select_options();

        break;
    }
    }

    if (run_nav_name)
    {
        lcd_display_banner(lcd, BANNER_NAV_NAME, LCD_VANISH);
    }
    else
    {
        run_nav_name = true;
    }

    return 0;
}

int measure_options(void)
{
    switch (nav_state->index)
    {
    case LEAF_MEASURES_ADD:
    {
        Serial.println("LEAF_MEASURES_ADD");

        current_track->measure_list.push_back(measure_create(current_track->measure_list.size()));
        measure_select_nav->data_array.push_back("Measure");
        current_track->active_measures = current_track->measure_list.size();
        measure_select_nav->index = 0;
        array_scroll(measure_select_nav, 0);
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case LEAF_MEASURES_REMOVE:
    {
        Serial.println("LEAF_MEASURES_REMOVE");
        // select which to delete
        nav_state = measure_select_nav;
        lcd_display(lcd, nav_state->lcd_state);
        // lcd_display_banner(lcd, BANNER_NAV_NAME, LCD_VANISH);
        break;
    }
    case LEAF_MEASURES_SWAP:
    {
        Serial.println("LEAF_MEASURES_SWAP");
        // select which to delete
        nav_state = measure_select_nav;
        lcd_display(lcd, nav_state->lcd_state);
        run_nav_name = false;
        lcd_display_banner(lcd, BANNER_MEASURE_SWAP, LCD_VANISH);
        break;
    }
    case LEAF_MEASURES_GLOBAL_BEATS:
    {
        Serial.println("LEAF_MEASURES_GLOBAL_BEATS");
        // nav_state = measures_set_beats_nav;
        nav_state = measure_select_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case LEAF_MEASURES_GLOBAL_STEPS:
    {
        Serial.println("LEAF_MEASURES_GLOBAL_STEPS");
        nav_state = measure_select_nav;
        // measures_set_steps_nav->parent = tracks_nav;
        // nav_state = measures_set_steps_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case LEAF_MEASURES_LOCAL_STEPS:
    {
        Serial.println("LEAF_MEASURES_LOCAL_STEPS");
        nav_state = measure_select_nav;
        // measures_set_steps_nav->parent = measures_set_beats_nav;
        // nav_state = measures_set_beats_nav;
        lcd_display(lcd, nav_state->lcd_state);
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
        track_save_string.assign(TRACK_SAVE_NAME_LENGTH, '_');
        tracks_save_nav->parent = tracks_nav;
        nav_state = tracks_save_nav;
        lcd_mode = LCD_SPLASH;
        lcd_display(lcd, nav_state->lcd_state);
        lcd_display_banner(lcd, BANNER_TRACK_SAVE, LCD_PERSIST);
        run_nav_name = false;
        break;
    }
    case LEAF_TRACKS_LOAD:
    {
        Serial.println("LEAF_TRACKS_LOAD");
        nav_state = tracks_load_nav;
        lcd_display(lcd, nav_state->lcd_state);
        // lcd_display_banner(lcd, BANNER_MEASURE_SWAP, LCD_VANISH);
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
            if (!tracks_load_nav->data_array.empty())
            {
                free_track(current_track);
                read_track(tracks_load_nav->data_array[0], current_track);
            }
            lcd_display(lcd, nav_state->lcd_state);
        }
        break;
    }
    }
    return 0;
}

int delete_options(void)
{
    switch (nav_state->index)
    {
    case LEAF_DELETE_LAST:
    {
        placeholder->sound_list[placeholder_sound] = empty_sound;
        placeholder->active_sounds--;
        nav_state = main_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case LEAF_DELETE_STEP:
    {
        lcd_splash(lcd, nav_state, delete_step_splash);
        nav_state = main_nav;
        delete_step = true;

        break;
    }
    case LEAF_DELETE_MEASURE:
    {
        current_measure = measure_create(current_measure->id);
        nav_state = main_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    }

    return 0;
}

int measure_select_options(void)
{
    switch (measure_nav->index)
    {
    case LEAF_MEASURES_REMOVE:
    {
        Serial.println("LEAF_MEASURES_REMOVE");
        if (current_track->measure_list.size() > 1)
        {
            current_track->measure_list.erase(current_track->measure_list.begin() + measure_select_nav->index);
            measure_select_nav->data_array.pop_back();
            measure_select_nav->index = 0;
            array_scroll(measure_select_nav, 0);
            for (int i = 0; i < current_track->measure_list.size(); i++)
            {
                // reassign ids
                current_track->measure_list[i]->id = i;
            }
        }
        else
        {
            current_track->measure_list[0] = measure_create(0);
            current_track->current_measure = current_measure;
        }
        current_track->active_measures = current_track->measure_list.size();
        nav_state = measure_nav;
        lcd_display(lcd, nav_state->lcd_state);
        // lcd_display_banner(lcd, BANNER_NAV_NAME, LCD_VANISH);
        break;
    }
    case LEAF_MEASURES_SWAP:
    {
        Serial.println("LEAF_MEASURES_SWAP");
        if (measure_swap_panel == 0)
        {
            measure_swap_id = nav_state->index;
            measure_swap_panel++;
            run_nav_name = false;
            lcd_display_banner(lcd, BANNER_MEASURE_SWAP, LCD_VANISH);
        }
        else if (measure_swap_panel == 1)
        {
            swap(current_track->measure_list[measure_swap_id], current_track->measure_list[nav_state->index]);
            measure_swap_panel = 0;

            for (int i = 0; i < current_track->measure_list.size(); i++)
            {
                // reassign ids
                current_track->measure_list[i]->id = i;
            }

            nav_state = measure_nav;
            lcd_display(lcd, nav_state->lcd_state);
            // lcd_display_banner(lcd, BANNER_NAV_NAME, LCD_VANISH);
        }
        break;
    }
    case LEAF_MEASURES_GLOBAL_BEATS:
    {
        Serial.println("LEAF_MEASURES_GLOBAL_BEATS");
        nav_state = measures_set_beats_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case LEAF_MEASURES_GLOBAL_STEPS:
    {
        Serial.println("LEAF_MEASURES_GLOBAL_STEPS");
        measures_set_steps_nav->parent = measure_select_nav;
        nav_state = measures_set_steps_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    case LEAF_MEASURES_LOCAL_STEPS:
    {
        Serial.println("LEAF_MEASURES_LOCAL_STEPS");
        measures_set_steps_nav->parent = measures_set_beats_nav;
        nav_state = measures_set_beats_nav;
        lcd_display(lcd, nav_state->lcd_state);
        break;
    }
    }

    return 0;
}
