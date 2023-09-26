#include "pch.h"
#include "Sound.h"

namespace rdt {
	Sound::Sound()
	{
		m_state = NO_BINDING;
		
		for (int i = 0; i < NUM_AUDIO_BUFFERS; i++) {
			m_buffers[i] = 0;
		}

		m_format = 0;
		m_sampleRate = 0;
		m_cursor = 0;
	}
	Sound::~Sound()
	{
		if (m_state != NO_BINDING) {
			alDeleteBuffers(NUM_AUDIO_BUFFERS, &m_buffers[0]);
		}
	}

	bool Sound::LoadFile(const char* filepath)
	{
		AudioFile<float> afile;

		if (!afile.load(filepath)) {
			return false;
		}

		if (!afile.savePCMToBuffer(m_soundData)) {
			return false;
		}

		if (!SetBufferFormat(afile.getNumChannels(), afile.getBitDepth())) {
			return false;
		}

		m_sampleRate = afile.getSampleRate();

		alGenBuffers(NUM_AUDIO_BUFFERS, &m_buffers[0]);
		PreLoadBuffers();

		m_state = NOT_PLAYING;
		return true;
	}

	void Sound::PlaySound(const ALuint source)
	{
		alSourceQueueBuffers(source, NUM_AUDIO_BUFFERS, m_buffers);
		alSourcePlay(source);

		ALint playState = AL_PLAYING;
		while (playState == AL_PLAYING) {
			UpdateStream(source);
			alGetSourcei(source, AL_SOURCE_STATE, &playState);
		}
	}

	bool Sound::SetBufferFormat(unsigned int numChannels, unsigned int bitDepth)
	{
		if (numChannels == 1 && bitDepth == 8) {
			m_format = AL_FORMAT_MONO8; 
			return true;
		}
		if (numChannels == 1 && bitDepth == 16) {
			m_format = AL_FORMAT_MONO16;
			return true;
		}
		if (numChannels == 2 && bitDepth == 8) {
			m_format = AL_FORMAT_STEREO8;
			return true;
		}
		if (numChannels == 2 && bitDepth == 16) {
			m_format = AL_FORMAT_STEREO16;
			return true;
		}

		printf("SoundBuffer: Unrecognized WAV format: %d channels and %d bps\n",
			numChannels, bitDepth);

		return false;
	}
	void Sound::PreLoadBuffers()
	{
		std::size_t i = 0;
		for (; i < NUM_AUDIO_BUFFERS; ++i)
		{
			alBufferData(m_buffers[i], m_format, &m_soundData[i * AUDIO_BUFFER_SIZE], AUDIO_BUFFER_SIZE, m_sampleRate);
		}

		m_cursor = i * AUDIO_BUFFER_SIZE;
	}

	void Sound::UpdateStream(const ALuint source)
	{
		ALint buffersProcessed;
		alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);

		if (buffersProcessed <= 0) {
			return;
		}

		char* data = new char[AUDIO_BUFFER_SIZE];
		while (buffersProcessed--) {
			ALuint buffer;
			alSourceUnqueueBuffers(source, 1, &buffer);

			std::memset(data, 0, AUDIO_BUFFER_SIZE);

			std::size_t dataSizeToCopy = AUDIO_BUFFER_SIZE;
			if (m_cursor + AUDIO_BUFFER_SIZE > m_soundData.size()) {
				dataSizeToCopy = m_soundData.size() - m_cursor;
			}

			std::memcpy(data, &m_soundData[m_cursor], dataSizeToCopy);
			m_cursor += dataSizeToCopy;

			if (dataSizeToCopy < AUDIO_BUFFER_SIZE)
			{
				m_cursor = 0;
				std::memcpy(&data[dataSizeToCopy], &m_soundData[m_cursor], AUDIO_BUFFER_SIZE - dataSizeToCopy);
				m_cursor = AUDIO_BUFFER_SIZE - dataSizeToCopy;
			}

			alBufferData(buffer, m_format, data, AUDIO_BUFFER_SIZE, m_sampleRate);
			alSourceQueueBuffers(source, 1, &buffer);
		}

		delete[] data;
	}
}
