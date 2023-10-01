#include "pch.h"
#include "SoundSource.h"

namespace rdt::core {
	SoundSource::SoundSource()
	{
		m_sourceID = 0;
	}
	SoundSource::~SoundSource()
	{
		if (m_sourceID) {
			alDeleteSources(1, &m_sourceID);
		}
	}
	void SoundSource::Init()
	{
		alGenSources(1, &m_sourceID);
		alSourcef(m_sourceID, AL_PITCH, 1);
		alSourcef(m_sourceID, AL_GAIN, 1.0f);
	}

	void SoundSource::SetPosition(const Vec3f& nPos)
	{
		alSource3f(m_sourceID, AL_POSITION, nPos.x, nPos.y, nPos.z);
	}

	void SoundSource::SetVelocity(const Vec3f& nVel)
	{
		alSource3f(m_sourceID, AL_VELOCITY, nVel.x, nVel.y, nVel.z);
	}

	void SoundSource::AttachBuffer(ALuint bufferID)
	{
		alSourcei(m_sourceID, AL_LOOPING, AL_FALSE);
		alSourcei(m_sourceID, AL_BUFFER, bufferID);
	}
}