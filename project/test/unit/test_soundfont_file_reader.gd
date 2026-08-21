class_name TestSoundFontFileReader
extends GdUnitTestSuite

func test_create_and_get_data():
	var reader = SoundFontFileReader.new()
	var data = PackedByteArray([0x52, 0x49, 0x46, 0x46])
	reader.set_data(data)
	var result = reader.get_data()
	assert_array(result).has_size(4)
	assert_str(reader.get_extension()).is_equal("sf2str")

func test_empty_data():
	var reader = SoundFontFileReader.new()
	var result = reader.get_data()
	assert_array(result).is_empty()

func test_get_extension():
	var reader = SoundFontFileReader.new()
	assert_str(reader.get_extension()).is_equal("sf2str")
