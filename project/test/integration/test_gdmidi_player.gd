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
