#include "pch.h"
#include "SoundData.h"


namespace rdt::core {
	SoundData::SoundData()
	{
	}
	SoundData::~SoundData()
	{
	}

	bool SoundData::LoadFile(const char* filepath)
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

		return true;
	}

	unsigned int SoundData::GetSize()
	{
		return m_soundData.size();
	}

	ALenum SoundData::GetFormat()
	{
		return m_format;
	}

	uint32_t SoundData::GetSampleRate()
	{
		return m_sampleRate;
	}

	uint8_t* SoundData::GetDataAt(unsigned int index)
	{
		return &m_soundData[index];
	}
	
	bool SoundData::SetBufferFormat(unsigned int numChannels, unsigned int bitDepth)
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
}
