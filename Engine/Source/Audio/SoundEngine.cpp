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
		m_shouldEndLoop = true;
		m_queue_index = 0;
	}

	SoundEngine::~SoundEngine()
	{
		
		for (auto& [id, sound] : m_sounds) {
			delete sound;
		}

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

	void SoundEngine::AudioLoopImpl()
	{
		m_shouldEndLoop = false;

		while (!m_shouldEndLoop) {
			RunCommandQueue();
			UpdateSounds();
		}
	}

	void SoundEngine::RunCommandQueue()
	{
		m_queue_index = m_queue_index == 1 ? 0 : 1;

		for (auto& command : m_command_queue[m_queue_index == 1 ? 0 : 1]) {
			switch (command.type) {
			case SCT_LoadResource:
				AddResourceToMap((LoadResourceData*)command.data);
				break;
			case SCT_CreateSound:
				AddSound((CreateSoundData*)command.data);
				break;
			case SCT_PlaySound:
				PlaySound((PlaySoundData*)command.data);
				break;
			}

			if (command.data != nullptr) {
				delete command.data;
			}
		}
		m_command_queue[m_queue_index == 1 ? 0 : 1].clear();
	}

	void SoundEngine::UpdateSounds()
	{
		for (auto& [sID, sound] : m_sounds) {
			sound->OnUpdate();
		}
	}

	void SoundEngine::LoadResourceImpl(const std::string& alias, const std::string& filepath)
	{
		SoundData* sData = new SoundData;
		if (!sData->LoadFile(filepath.c_str())) {
			delete sData;
			return;
		}

		LoadResourceData* command_data = new LoadResourceData;
		command_data->dataPtr = sData;
		command_data->soundAlias = alias;
		m_command_queue[m_queue_index].push_back({SCT_LoadResource, command_data});
	}

	void SoundEngine::AddResourceToMap(LoadResourceData* data)
	{
		m_data_map[data->soundAlias] = data->dataPtr;
	}

	void SoundEngine::AddSound(CreateSoundData* data)
	{
		if (m_data_map.find(data->resource) == m_data_map.end()) {
			return;
		}
		auto& res = m_data_map.at(data->resource);
		data->sPtr->OnCreate(res);
		m_sounds[data->sID] = data->sPtr;
	}

	void SoundEngine::PlaySound(PlaySoundData* data)
	{
		m_sounds.at(data->sID)->PlaySound(data->srcPos, data->looping);
	}

	SoundID SoundEngine::AddCreateSoundCommand(const std::string& resource, Sound* nSound)
	{
		CreateSoundData* command_data = new CreateSoundData;
		command_data->sID = GetNextSoundID();
		command_data->sPtr = nSound;
		command_data->resource = resource;

		m_command_queue[m_queue_index].push_back({SCT_CreateSound, command_data});
		return command_data->sID;
	}

	void SoundEngine::AddPlaySoundCommand(const SoundID sID, const Vec3f& srcPos, bool looping)
	{
		if (m_sounds.find(sID) == m_sounds.end()) {
			return;
		}

		PlaySoundData* command_data = new PlaySoundData;
		command_data->sID = sID;
		command_data->srcPos = srcPos;
		command_data->looping = looping;
		m_command_queue[m_queue_index].push_back({SCT_PlaySound, command_data});
	}

	SoundID SoundEngine::GetNextSoundID()
	{
		return ++m_SoundCounter;
	}

}