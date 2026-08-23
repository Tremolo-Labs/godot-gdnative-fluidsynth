#include "midi_file_reader.h"

using namespace godot;

MidiFileReader::MidiFileReader() {
}

MidiFileReader::~MidiFileReader() {
}

void MidiFileReader::_init() {
}

void MidiFileReader::clear_data() {
}

void MidiFileReader::set_data(PackedByteArray data) {
	// Copy-on-write: shares the incoming buffer until the first mutation.
	array_data = data;
}

PackedByteArray MidiFileReader::get_data() {
	return array_data;
}

String MidiFileReader::get_extension() {
	return "midstr";
}

void MidiFileReader::_bind_methods() {
	// 	godot::ClassDB::bind_method(D_METHOD("print_type", "variant"), &ExampleClass::print_type);
	ClassDB::bind_method(D_METHOD("set_data", "data"), &MidiFileReader::set_data);
	ClassDB::bind_method(D_METHOD("get_data"), &MidiFileReader::get_data);
	ClassDB::bind_method(D_METHOD("get_extension"), &MidiFileReader::get_extension);
	ADD_PROPERTY(godot::PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data"), "set_data", "get_data");
}
