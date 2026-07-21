@tool
class_name MidiDataLoader
extends ResourceFormatLoader

func _get_recognized_extensions():
	return PackedStringArray(["midstr"])


func _get_resource_type(path):
	var ext = path.get_extension().to_lower()
	if ext == "midstr":
		return "MidiFileReader"
	return ""

func _handles_type(typename):
	return typename == "MidiFileReader"

func _load(path, _original_path, _use_sub_threads, _cache_mode):
	var f = FileAccess.open(path, FileAccess.READ)
	if f == null:
		return ERR_FILE_CANT_OPEN

	var res = MidiFileReader.new()
	var data = f.get_buffer(f.get_length())
	res.set_data(data)
	f.close()

	return res
