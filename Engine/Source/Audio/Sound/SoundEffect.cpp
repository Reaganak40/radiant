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
		if (m_state == IS_PLAYING) {
			OnPlaying();
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
	}
}
