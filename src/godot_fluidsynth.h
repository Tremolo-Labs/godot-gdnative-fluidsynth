#ifndef GDMIDIPLAYER_H
#define GDMIDIPLAYER_H

#include "midi_file_reader.h"
#include "soundfont_file_reader.h"
#include <fluidsynth.h>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/godot.hpp>

namespace godot {
class AudioStreamFluidsynth : public AudioStream {};
class AudioStreamPlayerFluidSynth : public AudioStreamPlayer {
	GDCLASS(AudioStreamPlayerFluidSynth, AudioStreamPlayer)

private:
	String soundfont;
	String midi_file;
	Ref<SoundFontFileReader> soundfont_resource;
	float *buffer;
	bool fluidsynth_playing;
	int sfont_id;

	fluid_settings_t *settings;
	fluid_synth_t *synth;
	fluid_player_t *player;
	fluid_audio_driver_t *adriver;
	Ref<AudioStreamGeneratorPlayback> stream_playback;

	void fill_buffer();

public:
	static void _bind_methods();

	AudioStreamPlayerFluidSynth();
	~AudioStreamPlayerFluidSynth();

	void _ready(); // our initializer called by Godot

	void _process(double delta) override;
	void set_soundfont(String p_soundfont);
	String get_soundfont();

	void set_midi_file(String p_midi_file);
	String get_midi_file();

	void fluidsynth_play();

	void program_select(int chan, int bank_num, int preset_num);
	void note_on(int chan, int key, int vel);
	void note_off(int chan, int key);
	void pitch_bend(int chan, int val); //val value (0-16383 with 8192 being center)
};

} //namespace godot

#endif
