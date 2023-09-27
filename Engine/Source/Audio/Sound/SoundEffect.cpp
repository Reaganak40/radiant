#include "pch.h"
#include "SoundEffect.h"

namespace rdt {
	SoundEffect::SoundEffect()
	{
	}
	SoundEffect::~SoundEffect()
	{
	}

	void SoundEffect::OnCreate(SoundData* data)
	{
		InitBuffers(data);
	}

	void SoundEffect::OnUpdate()
	{
		m_justStopped = false;
		if (m_state == IS_PLAYING) {
			OnPlaying();

			if (m_state != IS_PLAYING) {
				m_justStopped = true;
			}
		}
	}

	void SoundEffect::OnStart()
	{
	}

	void SoundEffect::OnPause()
	{
	}

	void SoundEffect::OnEndOfCycle()
	{
	}

	void SoundEffect::OnStop()
	{
		m_justStopped = true;
	}
}
