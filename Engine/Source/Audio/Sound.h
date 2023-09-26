#pragma once
#include "SoundSource.h"

namespace rdt {

// 32kb of data in each buffer
#define AUDIO_BUFFER_SIZE 65536
#define NUM_AUDIO_BUFFERS 4

	enum SoundState {
		NO_BINDING,
		NOT_PLAYING,
		IS_PLAYING,
	};

	class Sound {
	private:
		SoundState m_state;

		ALenum m_format;
		uint32_t m_sampleRate;
		std::vector<uint8_t> m_soundData;

		ALuint m_buffers[NUM_AUDIO_BUFFERS];
		std::size_t m_cursor;

	public:
		Sound();
		~Sound();

		/*
			Loads a sound file from the provided filepath, returns
			true if the load was successful.
		*/
		bool LoadFile(const char* filepath);

		/*
			Plays the loaded sound from the provided source.
		*/
		void PlaySound(const ALuint source);
		
	private:
		/*
			Uses the AudioFile data to determine its format.
		*/
		bool SetBufferFormat(unsigned int numChannels, unsigned int bitDepth);

		/*
			Loads the starting data from the audio file into the buffers.
		*/
		void PreLoadBuffers();

		void UpdateStream(const ALuint source);
	};
}
