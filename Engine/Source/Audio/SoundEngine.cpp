#include "pch.h"
#include "SoundEngine.h"

namespace rdt {
	
	SoundEngine* SoundEngine::m_instance = nullptr;
	
	SoundEngine::SoundEngine()
	{
		m_ALCdevice = alcOpenDevice(nullptr);
		if (!m_ALCdevice) {
			printf("Sound Manager: Failed to open ALC device!\n");
			exit(-1);
		}

		m_context = alcCreateContext(m_ALCdevice, nullptr);
		if (!m_context) {
			printf("Sound Manager: Failed to create OpenAL context!\n");
			exit(-1);
		}

		if (!alcMakeContextCurrent(m_context)) {
			printf("Sound Manager: Could not make audio context current!\n");
			exit(-1);
		}

		m_SoundCounter = 0;
	}

	SoundEngine::~SoundEngine()
	{
		
		alcDestroyContext(m_context);
		alcCloseDevice(m_ALCdevice);
	}

	void SoundEngine::Initialize()
	{
		if (m_instance != nullptr) {
			Destroy();
		}

		m_instance = new SoundEngine;
	}

	void SoundEngine::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	SoundID SoundEngine::LoadAudioImpl(const std::string& audioFilepath)
	{
		SoundID nSID = GetNextSoundID();
		m_sounds[nSID];

		if (!m_sounds.at(nSID).LoadFile(audioFilepath.c_str())) {
			m_sounds.erase(nSID);
			return 0;
		}

		return nSID;
	}

	void SoundEngine::PlaySoundImpl(const SoundID sID, const Vec3f& srcPos)
	{
		if (m_sounds.find(sID) == m_sounds.end()) {
			return;
		}

		SoundSource source;
		source.Init();
		source.SetPosition(srcPos);
		source.SetVelocity(Vec3f::Zero());

		m_sounds.at(sID).PlaySound(source.GetID());

		printf("Done\n");
	}

	SoundID SoundEngine::GetNextSoundID()
	{
		return ++m_SoundCounter;
	}

}