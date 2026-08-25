# Godot Fluidsynth

Godot gdnative fluidsynth library to allow playing music using fluidsynth. Based on the template at <https://github.com/godotengine/godot-cpp-template>

Godot 4.x GDExtension wrapping FluidSynth for MIDI playback:

-   `AudioStreamPLayerFluidSynth` - AudioStreamPlayer subclass for MIDI playback
-   `MidiFileReader` - Resource class for loading .mid files
-   `SoundFontFileReader` - Resource class for loading .sf2 soundfont files



### Build Commands

1.  Prerequisites (Ubuntu/Debian)

        apt install fluidsynth libfluidsynth-dev scons

2.  First-time setup

        git submodule update --init --recursive

3.  Build the extension

        # Editor build
        scons api_version=4.7 target=editor

        # With compile_commands.json for IDE support
        scons api_version=4.7 target=editor compiledb=yes



### Architecture

1.  Source Files (src/)
| File                        | Purpose                                                |
|-----------------------------+--------------------------------------------------------|
| register_types.cpp          | GDExtension entry point, registers all classes         |
| godot_fluidsynth.cpp/h      | Main AudioStreamPLayerFluidSynth class wrapping FluidSynth |
| midi_file_reader.cpp/h      | Resource loader for MIDI files                         |
| soundfont_file_reader.cpp/h | Resource loader for SoundFont files                    |

2.  Integration Pattern

    FluidSynth expects file paths, but Godot resources are in-memory. The extension uses a custom sfloader with memory callbacks (`my_open`, `my_read`, `my_seek`, `my_tell`, `my_close`) that interpret a pointer address encoded as a string (`"&%p"`) to load soundfonts from Godot's resource system.

3.  Build Output

    -   `bin/<platform>/` - Raw build output
    -   `gdmidiplayer/bin/<platform>/` - Copied for Godot project use



### Platform Support
| Platform | Method     | Status    |
|----------+------------+-----------|
| Linux    | pkg-config | Supported |
| Windows  | vcpkg      | Manual    |
| macOS    | vcpkg      | Manual    |
| Android  | NDK        | Manual    |
| iOS      | Xcode      | Manual    |
| Web      | Emscripten | Manual    |
