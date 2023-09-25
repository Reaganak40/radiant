#include "pch.h"
#include "SoundBuffer.h"
#include "AudioError.h"

namespace rdt {
	SoundBuffer::SoundBuffer()
	{
		m_bufferID = 0;
	}
	SoundBuffer::~SoundBuffer()
	{
		if (m_bufferID) {
			alDeleteBuffers(1, &m_bufferID);
		}
	}

	bool SoundBuffer::LoadFile(const char* filepath)
	{
		if (!m_file.load(filepath)) {
			return false;
		 }
		
		alCall(alGenBuffers, 1, &m_bufferID);

		std::vector<uint8_t> pcmData;
		if (!m_file.savePCMToBuffer(pcmData)) {
			return false;
		}

		alBufferData(m_bufferID, GetBufferFormat(m_file.getNumChannels(), m_file.getBitDepth()),
			reinterpret_cast<ALvoid*>(pcmData.data()), pcmData.size(), m_file.getSampleRate());

		return true;
	}
	ALenum SoundBuffer::GetBufferFormat(unsigned int numChannels, unsigned int bitDepth)
	{
		if (numChannels == 1 && bitDepth == 8)  { return AL_FORMAT_MONO8; }
		if (numChannels == 1 && bitDepth == 16) { return AL_FORMAT_MONO16; }
		if (numChannels == 2 && bitDepth == 8)  { return AL_FORMAT_STEREO8; }
		if (numChannels == 2 && bitDepth == 16) { return AL_FORMAT_STEREO16; }
		
		printf("SoundBuffer: Unrecognized WAV format: %d channels and %d bps\n",
			numChannels, bitDepth);

		return 0;
	}
}
