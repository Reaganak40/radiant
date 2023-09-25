#include "pch.h"
#include "SoundManager.h"

namespace rdt {
	
	SoundManager* SoundManager::m_instance = nullptr;
	
	SoundManager::SoundManager()
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

	SoundManager::~SoundManager()
	{
		
		alcDestroyContext(m_context);
		alcCloseDevice(m_ALCdevice);
	}

	void SoundManager::Initialize()
	{
		if (m_instance != nullptr) {
			Destroy();
		}

		m_instance = new SoundManager;
	}

	void SoundManager::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	SoundID SoundManager::LoadAudioImpl(const std::string& audioFilepath)
	{
		SoundID nSID = GetNextSoundID();
		m_sounds[nSID];

		if (!m_sounds.at(nSID).buffer.LoadFile(audioFilepath.c_str())) {
			m_sounds.erase(nSID);
			return 0;
		}

		auto& nSound = m_sounds.at(nSID);
		nSound.source.Init();
		nSound.source.SetPosition({ 0, 0, 0 });
		nSound.source.SetVelocity({ 0, 0, 0 });
		nSound.source.AttachBuffer(nSound.buffer.GetID());

		return nSID;
	}

	void SoundManager::PlaySoundImpl(const SoundID sID)
	{
		if (m_sounds.find(sID) == m_sounds.end()) {
			return;
		}

		alSourcePlay(m_sounds.at(sID).source.GetID());

		ALint state = AL_PLAYING;
		while (state == AL_PLAYING)
		{
			alGetSourcei(m_sounds.at(sID).source.GetID(), AL_SOURCE_STATE, &state);
		}
		printf("Done\n");
	}

	SoundID SoundManager::GetNextSoundID()
	{
		return ++m_SoundCounter;
	}

}