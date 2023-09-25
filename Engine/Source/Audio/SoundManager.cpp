#include "pch.h"
#include "SoundManager.h"
#include "AudioError.h"

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

	}

	SoundManager::~SoundManager()
	{
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

}