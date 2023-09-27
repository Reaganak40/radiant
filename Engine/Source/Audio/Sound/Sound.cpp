#include "pch.h"
#include "Sound.h"

namespace rdt {
	void Sound::InitBuffers(SoundData* nData)
	{
		m_data = nData;
		alGenBuffers(NUM_AUDIO_BUFFERS, &m_buffers[0]);
		PreLoadBuffers();

		m_state = NOT_PLAYING;
	}

	void Sound::OnPlaying()
	{
		ALint playState;
		alGetSourcei(m_source.GetID(), AL_SOURCE_STATE, &playState);
		UpdateStream(m_source.GetID(), m_shouldLoop);

		if (playState != AL_PLAYING) {
			m_state = NOT_PLAYING;
			PreLoadBuffers();
			OnStop();
		}
	}

	Sound::Sound()
	{
		m_state = NO_BINDING;
		m_shouldLoop = false;
		m_data = nullptr;

		for (int i = 0; i < NUM_AUDIO_BUFFERS; i++) {
			m_buffers[i] = 0;
		}
		m_cursor = 0;
		m_source.Init();
	}
	Sound::~Sound()
	{
		if (m_state != NO_BINDING) {
			alDeleteBuffers(NUM_AUDIO_BUFFERS, &m_buffers[0]);
		}
	}

	void Sound::PlaySound(const Vec3f& srcPos, bool loop)
	{
		if (m_state == IS_PLAYING) {
			return;
		}

		m_source.SetPosition(srcPos);
		m_source.SetVelocity(Vec3f::Zero());

		ALsizei numBuffers = NUM_AUDIO_BUFFERS;
		if (loop && m_unused_buffers.size()) {
			UseUnusedBuffers();
		}
		else {
			numBuffers -= m_unused_buffers.size();
		}

		alSourceQueueBuffers(m_source.GetID(), numBuffers, m_buffers);
		alSourcePlay(m_source.GetID());
		m_state = IS_PLAYING;
		m_shouldLoop = loop;

		OnStart();
	}

	void Sound::PreLoadBuffers()
	{
		m_cursor = 0;
		unsigned int i = 0;

		std::size_t dataSize = m_data->GetSize();
		for (; i < NUM_AUDIO_BUFFERS; ++i)
		{
			if (m_cursor == dataSize) {
				break;
			}

			std::size_t dataSizeToCopy = AUDIO_BUFFER_SIZE;
			if (m_cursor + AUDIO_BUFFER_SIZE > dataSize) {
				dataSizeToCopy = dataSize - m_cursor;
			}

			alBufferData(m_buffers[i], m_data->GetFormat(), m_data->GetDataAt(m_cursor), dataSizeToCopy, m_data->GetSampleRate());
			m_cursor += dataSizeToCopy;
		}

		for (; i < NUM_AUDIO_BUFFERS; ++i)
		{
			m_unused_buffers.push_back(m_buffers[i]);
		}
	}

	void Sound::UseUnusedBuffers()
	{
		m_cursor = 0;
		
		char* data = new char[AUDIO_BUFFER_SIZE];
		std::size_t dataSize = m_data->GetSize();
		for (auto& buffer : m_unused_buffers) {

			std::size_t dataSizeToCopy = AUDIO_BUFFER_SIZE;
			if (m_cursor + AUDIO_BUFFER_SIZE > dataSize) {
				dataSizeToCopy = dataSize - m_cursor;
			}

			std::memcpy(data, m_data->GetDataAt(m_cursor), dataSizeToCopy);
			m_cursor += dataSizeToCopy;

			if (dataSizeToCopy < AUDIO_BUFFER_SIZE)
			{
				m_cursor = 0;
				std::memcpy(&data[dataSizeToCopy], m_data->GetDataAt(m_cursor), AUDIO_BUFFER_SIZE - dataSizeToCopy);
				m_cursor = AUDIO_BUFFER_SIZE - dataSizeToCopy;
				dataSizeToCopy = AUDIO_BUFFER_SIZE;
			}

			alBufferData(buffer, m_data->GetFormat(), m_data->GetDataAt(m_cursor), dataSizeToCopy, m_data->GetSampleRate());
		}

		m_unused_buffers.clear();
		delete[] data;
	}

	void Sound::UpdateStream(const ALuint source, bool loop)
	{
		ALint buffersProcessed;
		alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);

		if (buffersProcessed <= 0) {
			return;
		}

		char* data = new char[AUDIO_BUFFER_SIZE];
		std::size_t dataSize = m_data->GetSize();
		while (buffersProcessed--) {
			ALuint buffer;

			alSourceUnqueueBuffers(source, 1, &buffer);

			std::memset(data, 0, AUDIO_BUFFER_SIZE);

			std::size_t dataSizeToCopy = AUDIO_BUFFER_SIZE;
			if (m_cursor + AUDIO_BUFFER_SIZE > dataSize) {
				dataSizeToCopy = dataSize - m_cursor;
			}

			if (dataSizeToCopy == 0) {
				continue;
			}

			std::memcpy(data, m_data->GetDataAt(m_cursor), dataSizeToCopy);
			m_cursor += dataSizeToCopy;

			if (dataSizeToCopy < AUDIO_BUFFER_SIZE && loop)
			{
				OnEndOfCycle();
				m_cursor = 0;
				std::memcpy(&data[dataSizeToCopy], m_data->GetDataAt(m_cursor), AUDIO_BUFFER_SIZE - dataSizeToCopy);
				m_cursor = AUDIO_BUFFER_SIZE - dataSizeToCopy;
				dataSizeToCopy = AUDIO_BUFFER_SIZE;
			}

			alBufferData(buffer, m_data->GetFormat(), data, dataSizeToCopy, m_data->GetSampleRate());
			alSourceQueueBuffers(source, 1, &buffer);
		}

		delete[] data;
	}
}
