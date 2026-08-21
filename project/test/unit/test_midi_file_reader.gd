class_name TestMidiFileReader
extends GdUnitTestSuite

func test_create_and_get_data():
	var reader = MidiFileReader.new()
	var data = PackedByteArray([0x4D, 0x54, 0x68, 0x64])
	reader.set_data(data)
	var result = reader.get_data()
	assert_array(result).has_size(4)
	assert_str(reader.get_extension()).is_equal("midstr")

func test_empty_data():
	var reader = MidiFileReader.new()
	var result = reader.get_data()
	assert_array(result).is_empty()

func test_get_extension():
	var reader = MidiFileReader.new()
	assert_str(reader.get_extension()).is_equal("midstr")

func test_get_data_returns_independent_copy():
	var reader = MidiFileReader.new()
	var data = PackedByteArray([0x01, 0x02, 0x03])
	reader.set_data(data)
	data.resize(0)
	var result = reader.get_data()
	assert_array(result).has_size(3)
