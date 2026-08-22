class_name TestGDMidiPlayer
extends GdUnitTestSuite

var player: GDMidiAudioStreamPlayer

func before():
	player = GDMidiAudioStreamPlayer.new()
	add_child(player)
	await get_tree().process_frame

func after():
	if player:
		remove_child(player)
		player.free()

func test_properties_defaults():
	assert_str(player.get_soundfont()).is_equal("")
	assert_str(player.get_midi_file()).is_equal("")

func test_set_get_soundfont():
	player.set_soundfont("res://assets/example.sf2")
	assert_str(player.get_soundfont()).is_equal("res://assets/example.sf2")

func test_set_get_midi_file():
	player.set_midi_file("res://assets/example.mid")
	assert_str(player.get_midi_file()).is_equal("res://assets/example.mid")

func test_fluidsynth_play_with_valid_assets():
	player.set_soundfont("res://assets/example.sf2")
	player.set_midi_file("res://assets/example.mid")
	await get_tree().process_frame
	player.fluidsynth_play()
	await get_tree().process_frame
	# Reaching here without errors is the bar under the headless dummy audio driver.
	player.stop()

func test_fluidsynth_play_invalid_paths_is_graceful():
	player.set_soundfont("res://assets/does_not_exist.sf2")
	player.set_midi_file("res://assets/does_not_exist.mid")
	player.fluidsynth_play()
	await get_tree().process_frame

func test_note_and_control_smoke_during_playback():
	player.set_soundfont("res://assets/example.sf2")
	player.set_midi_file("res://assets/example.mid")
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
	player.set_soundfont("res://assets/example.sf2")
	player.set_midi_file("res://assets/example.mid")
	await get_tree().process_frame
	player.fluidsynth_play()
	await get_tree().process_frame
	player.stop()
	await get_tree().process_frame
	player.fluidsynth_play()
	await get_tree().process_frame
	player.stop()
