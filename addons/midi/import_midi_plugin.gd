@tool
extends EditorImportPlugin

func _get_importer_name():
	return "midi"

func _get_visible_name():
	return "MIDI"

func _get_recognized_extensions():
	return ["mid"]

func _get_save_extension():
	return "midstr"

func _get_resource_type():
	return "MidiFileReader"

func _get_option_visibility(_path, _option, _options):
	return true

func _get_preset_count():
	return 1;

func _get_preset_name(_preset):
	return "Default"

func _get_import_options(_path, _preset):
	return []

func import(source_file, save_path, _options, _r_platform_variants, _r_gen_files):
	var file = FileAccess.open(source_file, FileAccess.READ)
	var err = file.get_error()
	if err != OK:
		return err
	var data = file.get_buffer(file.get_length())
	var midi_file = MidiFileReader.new()
	midi_file.set_data(data)
	file.close()
	return ResourceSaver.save(midi_file, "%s.%s" % [save_path, _get_save_extension()])
