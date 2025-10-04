@tool
extends EditorImportPlugin

const SoundFontFileReader = preload("res://bin/soundfont_file_reader.gdns")

func _get_importer_name():
	return "soundfont"

func _get_visible_name():
	return "SoundFont"

func _get_recognized_extensions():
	return ["sf2"]

func _get_save_extension():
	return "sf2str"

func _get_resource_type():
	return "SoundFontFileReader"

func _get_option_visibility(option, options):
	return true

func _get_preset_count():
	return 1;

func _get_preset_name(preset):
	return "Default"

func _get_import_options(preset):
	return []

func import(source_file, save_path, options, r_platform_variants, r_gen_files):
	var file = File.new()
	var err = file.open(source_file, File.READ)
	if err != OK:
		return err

	var data = file.get_buffer(file.get_length())
	var soundfont_file = SoundFontFileReader.new()
	soundfont_file.set_data(data)
	file.close()

	return ResourceSaver.save("%s.%s" % [save_path, _get_save_extension()], soundfont_file)
