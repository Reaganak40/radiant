#pragma once

namespace rdt::core {
	class SoundData {
	private:
		ALenum m_format;
		uint32_t m_sampleRate;
		std::vector<uint8_t> m_soundData;
	public:
		SoundData();
		~SoundData();

		/*
			Loads a sound file from the provided filepath, returns
			true if the load was successful.
		*/
		bool LoadFile(const char* filepath);

		/*
			Gets the audio file data size.
		*/
		size_t GetSize();

		/*
			Returns the audio file's format.
		*/
		ALenum GetFormat();


		/*
			Returns the sample rate of the file.
		*/
		uint32_t GetSampleRate();

		/*
			Returns a pointer to the PCM data of the audio file.
		*/
		uint8_t* GetDataAt(size_t index);

	private:
		/*
			Uses the AudioFile data to determine its format.
		*/
		bool SetBufferFormat(unsigned int numChannels, unsigned int bitDepth);
	};
}