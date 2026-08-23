#include "godot_fluidsynth.h"

using namespace godot;

int position = 0;
long fsize = 0;

void *my_open(const char *filename) {
	void *p;
	if (filename[0] != '&') {
		return NULL;
	}
	sscanf(filename, "&%p", &p);
	return p;
}

int my_read(void *buf, long long count, void *handle) {
	memcpy(buf, static_cast<char*>(handle) + position, count);
	position = position + count;

	return FLUID_OK;
}

int my_seek(void *handle, long long offset, int origin) {
	switch (origin) {
		case SEEK_SET:
			position = offset;
			break;
		case SEEK_CUR:
			position = position + offset;
			break;
		default:
			position = fsize + offset;
			break;
	}

	if (position < 0 || position > fsize) {
		return FLUID_FAILED;
	}

	return FLUID_OK;
}

int my_close(void *handle) {
	return FLUID_OK;
}

long long my_tell(void *handle) {
	return position;
}

void GDMidiAudioStreamPlayer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("fluidsynth_play"), &GDMidiAudioStreamPlayer::fluidsynth_play);
	ClassDB::bind_method(D_METHOD("program_select", "channel", "bank_num", "preset_num"), &GDMidiAudioStreamPlayer::program_select);
	ClassDB::bind_method(D_METHOD("note_on", "channel", "key", "velocity"), &GDMidiAudioStreamPlayer::note_on);
	ClassDB::bind_method(D_METHOD("note_off", "channel", "key"), &GDMidiAudioStreamPlayer::note_off);
	ClassDB::bind_method(D_METHOD("pitch_bend", "channel", "value"), &GDMidiAudioStreamPlayer::pitch_bend);
	ClassDB::bind_method(D_METHOD("set_soundfont", "soundfont"), &GDMidiAudioStreamPlayer::set_soundfont);
	ClassDB::bind_method(D_METHOD("get_soundfont"), &GDMidiAudioStreamPlayer::get_soundfont);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "soundfont"), "set_soundfont", "get_soundfont");
	ClassDB::bind_method(D_METHOD("set_midi_file", "midi_file"), &GDMidiAudioStreamPlayer::set_midi_file);
	ClassDB::bind_method(D_METHOD("get_midi_file"), &GDMidiAudioStreamPlayer::get_midi_file);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "midi file"), "set_midi_file", "get_midi_file");
}

GDMidiAudioStreamPlayer::GDMidiAudioStreamPlayer() :
	buffer(nullptr),
	fluidsynth_playing(false),
	sfont_id(0),
	settings(nullptr),
	synth(nullptr),
	player(nullptr),
	adriver(nullptr) {
	settings = new_fluid_settings();
	synth = new_fluid_synth(settings);
	player = new_fluid_player(synth);
	fluid_sfloader_t *my_sfloader = new_fluid_defsfloader(settings);
	fluid_sfloader_set_callbacks(my_sfloader,
			my_open,
			my_read,
			my_seek,
			my_tell,
			my_close);
	fluid_synth_add_sfloader(synth, my_sfloader);
}

GDMidiAudioStreamPlayer::~GDMidiAudioStreamPlayer() {
	delete[] buffer;
	if (player) delete_fluid_player(player);
	if (synth) delete_fluid_synth(synth);
	if (settings) delete_fluid_settings(settings);
}

void GDMidiAudioStreamPlayer::_ready() {
	AudioServer *as = AudioServer::get_singleton();
	if (as) {
		int buf_size = as->get_mix_rate() * 2;
		buffer = new float[buf_size];
	}
	fluidsynth_playing = false;
	stream_playback = get_stream_playback();
}

void GDMidiAudioStreamPlayer::_process(double delta) {
	if (player && fluid_player_get_status(player) == FLUID_PLAYER_DONE && fluidsynth_playing) {
		fluid_player_stop(player);
		fluid_player_join(player);
		delete_fluid_player(player);
		fluidsynth_playing = false;

		player = new_fluid_player(synth);
	}

	if (!synth || !player) {
		return;
	}

	if (is_playing() && !fluidsynth_playing) {
		fluidsynth_play();
	}

	if (is_playing()) {
		fill_buffer();
	}
}

void GDMidiAudioStreamPlayer::fill_buffer() {
	if (stream_playback.is_null() || !buffer || !synth) {
		return;
	}
	int64_t to_fill = stream_playback->get_frames_available();
	if (to_fill > 44100) {
		to_fill = 44100;
	}
	fluid_synth_write_float(synth, to_fill, buffer, 0, 2, buffer, 1, 2);
	int index = 0;
	while (to_fill > 0) {
		stream_playback->push_frame(Vector2(buffer[index], buffer[index + 1]));
		index = index + 2;
		to_fill = to_fill - 1;
	}
}

void GDMidiAudioStreamPlayer::set_soundfont(String p_soundfont) {
	soundfont = p_soundfont;

	if (!ResourceLoader::get_singleton()->exists(soundfont)) {
		return;
	}
	Ref<SoundFontFileReader> soundfont_file = ResourceLoader::get_singleton()->load(soundfont);
	if (soundfont_file.is_null()) {
		return;
	}
	// FluidSynth >= 2.6 stats the sfload path for its sample cache; our encoded
	// memory pseudo-path makes std::filesystem throw and abort the process.
	// Write the SoundFont to a real file and let the default loader handle it.
	static int temp_counter = 0;
	String temp_path = OS::get_singleton()->get_user_data_dir()
			+ "/godot_fluidsynth_" + String::num_int64(OS::get_singleton()->get_process_id())
			+ "_" + String::num_int64(temp_counter++) + ".sf2";

	Ref<FileAccess> f = FileAccess::open(temp_path, FileAccess::WRITE);
	if (f.is_null()) {
		return;
	}
	f->store_buffer(soundfont_file->get_data());
	f->close();

	sfont_id = fluid_synth_sfload(synth, temp_path.utf8().get_data(), 1);

	DirAccess::remove_absolute(temp_path);
}

String GDMidiAudioStreamPlayer::get_soundfont() {
	return soundfont;
}

void GDMidiAudioStreamPlayer::set_midi_file(String p_midi_file) {
	midi_file = p_midi_file;
}

String GDMidiAudioStreamPlayer::get_midi_file() {
	return midi_file;
}

void GDMidiAudioStreamPlayer::fluidsynth_play() {
	if (!ResourceLoader::get_singleton()->exists(midi_file)) {
		return;
	}
	Ref<MidiFileReader> midi = ResourceLoader::get_singleton()->load(midi_file);
	if (midi.is_null()) {
		return;
	}
	PackedByteArray byte_array = midi->get_data();

	if (byte_array.size() > 0) {
		fluid_player_add_mem(player, byte_array.ptr(), byte_array.size());
	}

	fluid_player_play(player);
	fluidsynth_playing = true;
}

void GDMidiAudioStreamPlayer::program_select(int chan, int bank_num, int preset_num) {
	fluid_synth_program_select(synth, chan, sfont_id, bank_num, preset_num);
}

void GDMidiAudioStreamPlayer::note_on(int chan, int key, int vel) {
	fluid_synth_noteon(synth, chan, key, vel);
}

void GDMidiAudioStreamPlayer::note_off(int chan, int key) {
	fluid_synth_noteoff(synth, chan, key);
}

void GDMidiAudioStreamPlayer::pitch_bend(int chan, int val) {
	fluid_synth_pitch_bend(synth, chan, val);
}
