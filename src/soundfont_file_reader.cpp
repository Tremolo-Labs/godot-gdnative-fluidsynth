#include "soundfont_file_reader.h"

#include <cstring>

using namespace godot;

SoundFontFileReader::SoundFontFileReader() :
	array_size(0),
	array_data(nullptr) {
}

SoundFontFileReader::~SoundFontFileReader() {
	delete[] array_data;
}

void SoundFontFileReader::_init() {
}

void SoundFontFileReader::clear_data() {
	delete[] array_data;
	array_data = nullptr;
	array_size = 0;
}

void SoundFontFileReader::set_data(PackedByteArray data) {
	if (array_data != nullptr) {
		delete[] array_data;
		array_data = nullptr;
	}

	array_size = data.size();
	array_data = new char[array_size + 1];

	if (array_size > 0) {
		memcpy(array_data, data.ptr(), array_size);
	}
	array_data[array_size] = 0;
}

PackedByteArray SoundFontFileReader::get_data() {
	PackedByteArray out_array;
	out_array.resize(array_size);

	if (array_size > 0) {
		memcpy(out_array.ptrw(), array_data, array_size);
	}
	return out_array;
}

char *SoundFontFileReader::get_array_data() {
	return array_data;
}

long SoundFontFileReader::get_array_size() {
	return array_size;
}

String SoundFontFileReader::get_extension() {
	return "sf2str";
}

void SoundFontFileReader::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_data", "data"), &SoundFontFileReader::set_data);
	ClassDB::bind_method(D_METHOD("get_data"), &SoundFontFileReader::get_data);
	ClassDB::bind_method(D_METHOD("get_extension"), &SoundFontFileReader::get_extension);
	ADD_PROPERTY(godot::PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data"), "set_data", "get_data");
}
