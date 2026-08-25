class_name TestGDMidiPlayer
extends GdUnitTestSuite

const MISSING_SOUNDFONT := "res://test/no_such_soundfont.sf2"
const MISSING_MIDI_FILE := "res://test/no_such_song.mid"

var player: AudioStreamPLayerFluidSynth

func before():
	player = AudioStreamPLayerFluidSynth.new()
	add_child(player)
	await get_tree().process_frame
	# One suite-wide load: every extra set_soundfont call stacks another full
	# copy of the SoundFont's samples inside the same synth.
	player.set_soundfont("res://assets/example.sf2")
	player.set_midi_file("res://assets/example.mid")

func after():
	if player:
		player.stop()
		remove_child(player)
		player.free()

func after_test():
	if player:
		player.stop()

func test_properties_defaults():
	var fresh = auto_free(AudioStreamPLayerFluidSynth.new())
	assert_str(fresh.get_soundfont()).is_equal("")
	assert_str(fresh.get_midi_file()).is_equal("")

func test_set_get_soundfont():
	# Throwaway instance: the shared fixture keeps its loaded assets, and a real
	# path here would stack another full SoundFont load onto the synth.
	var fresh = auto_free(AudioStreamPLayerFluidSynth.new())
	fresh.set_soundfont(MISSING_SOUNDFONT)
	assert_str(fresh.get_soundfont()).is_equal(MISSING_SOUNDFONT)

func test_set_get_midi_file():
	var fresh = auto_free(AudioStreamPLayerFluidSynth.new())
	fresh.set_midi_file(MISSING_MIDI_FILE)
	assert_str(fresh.get_midi_file()).is_equal(MISSING_MIDI_FILE)

func test_fluidsynth_play_with_valid_assets():
	await get_tree().process_frame
	player.fluidsynth_play()
	await get_tree().process_frame
	# Reaching here without errors is the bar under the headless dummy audio driver.
	player.stop()

func test_fluidsynth_play_invalid_paths_is_graceful():
	# A throwaway player keeps the shared fixture paths intact; setting them on
	# the shared instance would force a reload in later tests.
	var orphan = auto_free(AudioStreamPLayerFluidSynth.new())
	orphan.set_soundfont(MISSING_SOUNDFONT)
	orphan.set_midi_file(MISSING_MIDI_FILE)
	orphan.fluidsynth_play()
	await get_tree().process_frame

func test_note_and_control_smoke_during_playback():
	await get_tree().process_frame
	player.fluidsynth_play()
	await get_tree().process_frame
	player.program_select(0, 0, 0)
	player.note_on(0, 60, 127)
	await get_tree().process_frame
	player.pitch_bend(0, 10000)
	await get_tree().process_frame
	player.note_off(0, 60)
	player.stop()

func test_replay_after_stop():
	await get_tree().process_frame
	player.fluidsynth_play()
	await get_tree().process_frame
	player.stop()
	await get_tree().process_frame
	player.fluidsynth_play()
	await get_tree().process_frame
	player.stop()
