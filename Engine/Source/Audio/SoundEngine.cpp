#include "pch.h"
#include "SoundEngine.h"
#include "Messaging/MessageBus.h"
#include "Messaging/MessageTypes.h"

namespace rdt {
	
	using namespace core;

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

		m_broadcast = MessageBus::CreateBroadcast("SoundEngine", new Broadcast);
	}

	SoundEngine::~SoundEngine()
	{
		
		for (auto& [id, sound] : m_sounds) {
			delete sound;
		}

		for (int i = 0; i < 2; i++) {
			DeleteCommandQueue(i);
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

		Teardown();
	}

	void SoundEngine::RunCommandQueue()
	{
		m_mutex.lock();
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
			case SCT_StopSound:
				StopSound((StopSoundData*)command.data);
				break;
			}

			if (command.data != nullptr) {
				delete command.data;
			}
		}
		m_command_queue[m_queue_index == 1 ? 0 : 1].clear();
		m_mutex.unlock();
	}

	void SoundEngine::UpdateSounds()
	{
		for (auto& [sID, sound] : m_sounds) {
			sound->OnUpdate();

			if (sound->JustStopped()) {
				MessageBus::AddToBroadcast(m_broadcast, MT_SoundStopped, new SoundStoppedData(sID));
			}
		}
	}

	void SoundEngine::Teardown()
	{
		for (auto& [sID, sound] : m_sounds) {
			sound->StopSound();
		}
	}

	void SoundEngine::DeleteCommandQueue(unsigned int queueIndex)
	{
		for (auto& command : m_command_queue[queueIndex]) {

			if (command.data != nullptr) {
				switch (command.type) {
				case SCT_LoadResource:
					break;
				case SCT_CreateSound:
					if (((CreateSoundData*)command.data)->sPtr) {
						delete ((CreateSoundData*)command.data)->sPtr;
					}
					break;
				case SCT_PlaySound:
					break;
				}

				delete command.data;
			}
		}
	}

	void SoundEngine::AddToCommandQueue(const SoundCommand& command)
	{
		m_mutex.lock();
		m_command_queue[m_queue_index].push_back(command);
		m_mutex.unlock();
	}

	void SoundEngine::LoadResourceImpl(const std::string& alias, const std::string& filepath)
	{
		LoadResourceData* command_data = new LoadResourceData;
		command_data->filepath = filepath;
		command_data->soundAlias = alias;
		AddToCommandQueue({ SCT_LoadResource, command_data });
	}

	void SoundEngine::AddResourceToMap(LoadResourceData* data)
	{
		SoundData* nData = new SoundData;
		
		if (!nData->LoadFile(data->filepath.c_str())) {
			printf("SoundEngine: Could not located resource at [%s]\n", data->filepath.c_str());
			return;
		}
		m_data_map[data->soundAlias] = nData;
	}

	void SoundEngine::AddSound(CreateSoundData* data)
	{
		if (m_data_map.find(data->resource) == m_data_map.end()) {
			printf("SoundEngine: Could not create sound. [Resource '%s' was not found]\n", data->resource.c_str());
			return;
		}

		data->sPtr->OnCreate(m_data_map.at(data->resource));
		m_sounds[data->sID] = data->sPtr;
	}

	void SoundEngine::PlaySound(PlaySoundData* data)
	{
		if (m_sounds.find(data->sID) == m_sounds.end()) {
			return;
		}

		m_sounds.at(data->sID)->PlaySound(data->srcPos, data->looping);
	}

	void SoundEngine::StopSound(StopSoundData* data)
	{
		if (m_sounds.find(data->sID) == m_sounds.end()) {
			return;
		}

		m_sounds.at(data->sID)->StopSound();
		
		if (m_sounds.at(data->sID)->JustStopped()) {
			MessageBus::AddToBroadcast(m_broadcast, MT_SoundStopped, new SoundStoppedData(data->sID));
		}
	}

	SoundID SoundEngine::PushCreateSoundCommand(const std::string& resource, Sound* nSound)
	{
		CreateSoundData* command_data = new CreateSoundData;
		command_data->sID = GetNextSoundID();
		command_data->sPtr = nSound;
		command_data->resource = resource;

		AddToCommandQueue({ SCT_CreateSound, command_data });
		return command_data->sID;
	}

	void SoundEngine::PushPlaySoundCommand(const SoundID sID, const Vec3f& srcPos, bool looping)
	{
		PlaySoundData* command_data = new PlaySoundData;
		command_data->sID = sID;
		command_data->srcPos = srcPos;
		command_data->looping = looping;

		AddToCommandQueue({ SCT_PlaySound, command_data });
	}

	void SoundEngine::PushStopSoundCommand(const SoundID sID)
	{
		StopSoundData* command_data = new StopSoundData;
		command_data->sID = sID;

		AddToCommandQueue({ SCT_StopSound, command_data });
	}

	SoundID SoundEngine::GetNextSoundID()
	{
		return ++m_SoundCounter;
	}

}