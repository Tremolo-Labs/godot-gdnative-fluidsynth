@tool
extends EditorImportPlugin

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

func _get_option_visibility(_str, _option, _options):
	return true

func _get_preset_count():
	return 1;

func _get_preset_name(_preset):
	return "Default"

func _get_import_options(_preset, _int):
	return []

func import(source_file, save_path, _options, _r_platform_variants, _r_gen_files):
	var file = FileAccess.open(source_file, FileAccess.READ)
	var err = file.get_error()
	if err != OK:
		return err

	var data = file.get_buffer(file.get_length())
	var soundfont_file = SoundFontFileReader.new()
	soundfont_file.set_data(data)
	file.close()

	return ResourceSaver.save(soundfont_file, "%s.%s" % [save_path, _get_save_extension()])
