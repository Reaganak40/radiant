#pragma once

namespace rdt {
	class SoundBuffer {
	private:
		AudioFile<float> m_file;
		ALuint m_bufferID;
	public:
		SoundBuffer();
		~SoundBuffer();

		/*
			Loads a sound file from the provided filepath, returns
			true if the load was successful.
		*/
		bool LoadFile(const char* filepath);

		/*
			Returns the OpenAL bufferID.
		*/
		ALuint GetID() { return m_bufferID; }

	private:
		static ALenum GetBufferFormat(unsigned int numChannels, unsigned int bitDepth);
	};
}

