#pragma once
#include "SoundData.h"
#include "SoundSource.h"
#include "Sound/Sound.h"
#include "Messaging/Message.h"

namespace rdt {
	typedef unsigned int SoundID;

	class SoundEngine {
	private:
		SoundEngine();
		~SoundEngine();
		static SoundEngine* m_instance;
		MessageID m_broadcast;
		std::mutex m_mutex;

		/* ***********************************************
		*
		*		   Sound Engine Data Structures
		*
		* ***********************************************/
		enum SoundCommandType {
			SCT_LoadResource,
			SCT_CreateSound,
			SCT_PlaySound,
			SCT_StopSound,
		};

		struct SoundCommand {
			SoundCommandType type;
			void* data;

			SoundCommand(SoundCommandType nType, void* nData)
				: type(nType), data(nData) {}
		};

		struct LoadResourceData {
			std::string soundAlias;
			std::string filepath;
		};

		struct CreateSoundData {
			SoundID sID = 0;
			Sound* sPtr = nullptr;
			std::string resource;
		};

		struct PlaySoundData {
			SoundID sID = 0;
			Vec3f srcPos;
			bool looping = false;
		};

		struct StopSoundData {
			SoundID sID = 0;
		};

		/************************************************
		* 
		*			   Sound Engine Internals
		* 
		* ***********************************************/
		SoundID m_SoundCounter;
		ALCdevice* m_ALCdevice;
		ALCcontext* m_context;
		
		std::unordered_map<SoundID, Sound*> m_sounds;
		std::unordered_map<std::string, core::SoundData*> m_data_map;

		std::vector<SoundCommand> m_command_queue[2];
		unsigned int m_queue_index;
		bool m_shouldEndLoop;

	public:
		/*
			Creates the OpenAL context for game audio.
		*/
		static void Initialize();

		/*
			Destroys the OpenAL context and frees the singleton.
		*/
		static void Destroy();

		/*
			Starts the Audio Loop for the SoundEngine to continually
			process sounds.
		*/
		static void StartAudioLoop() { m_instance->AudioLoopImpl(); }

		/*
			Stops the Audio Loop for the SoundEngine.
		*/
		static void StopAudioLoop() { m_instance->m_shouldEndLoop = true; }

		/*
			Loads an audio source file into the sound engine, so it can be used
			by sound objects.
		*/
		static void LoadResource(const std::string& alias, const std::string& filepath) { m_instance->LoadResourceImpl(alias, filepath); }
		
		/*
			Plays the sound referenced by the provided SoundID at the provided
			source location. Looping is set to false by default.
		*/
		static void PlaySound(const SoundID sID, const Vec3f& srcPos = Vec3f::Zero(), bool looping = false) { m_instance->PushPlaySoundCommand(sID, srcPos, looping); }

		/*
			Adds a new sound object to the audio queue, which will be processed by the
			audio loop. This Sound object is now owned by the SoundEngine and will be
			freed by it.
		*/
		static SoundID CreateNewSound(const std::string& resource, Sound* nSound) { return m_instance->PushCreateSoundCommand(resource, nSound); }


		/*
			Stops a sound referenced by the provided SoundID if it is currently playing.
		*/
		static void StopSound(const SoundID sID) { m_instance->PushStopSoundCommand(sID); }

	private:

		void AudioLoopImpl();

		/*
			Updates the engine internals by running the command queue.
		*/
		void RunCommandQueue();

		/*
			Calls OnUpdate on all active sounds
		*/
		void UpdateSounds();

		/*
			Unbinds and stops all audio instances.
		*/
		void Teardown();

		/*
			Removes all allocated memory in the command queue.
		*/
		void DeleteCommandQueue(unsigned int queueIndex);

		void AddToCommandQueue(const SoundCommand& command);
		void LoadResourceImpl(const std::string& alias, const std::string& filepath);
		void AddResourceToMap(LoadResourceData* data);
		void AddSound(CreateSoundData* data);
		void PlaySound(PlaySoundData* data);
		void StopSound(StopSoundData* data);

		SoundID PushCreateSoundCommand(const std::string& resource, Sound* nSound);
		void PushPlaySoundCommand(const SoundID sID, const Vec3f& srcPos, bool looping);
		void PushStopSoundCommand(const SoundID sID);
		SoundID GetNextSoundID();

	};
}