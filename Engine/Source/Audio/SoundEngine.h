#pragma once
#include "SoundData.h"
#include "SoundSource.h"
#include "Sound/Sound.h"


namespace rdt {
	typedef unsigned int SoundID;

	class SoundEngine {
	private:
		SoundEngine();
		~SoundEngine();
		static SoundEngine* m_instance;

		/* ***********************************************
		*
		*		   Sound Engine Data Structures
		*
		* ***********************************************/
		enum SoundCommandType {
			SCT_LoadResource,
			SCT_CreateSound,
			SCT_PlaySound,
		};

		struct SoundCommand {
			SoundCommandType type;
			void* data;

			SoundCommand(SoundCommandType nType, void* nData)
				: type(nType), data(nData) {}
		};

		struct CreateSoundData {
			SoundID sID;
			Sound* sPtr;
			std::string resource;
		};

		struct PlaySoundData {
			SoundID sID;
			Vec3f srcPos;
			bool looping;
		};

		struct LoadResourceData {
			std::string soundAlias;
			SoundData* dataPtr;
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
		std::unordered_map<std::string, SoundData*> m_data_map;

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
		static void PlaySound(const SoundID sID, const Vec3f& srcPos, bool looping = false) { m_instance->AddPlaySoundCommand(sID, srcPos, looping); }

		/*
			Adds a new sound object to the audio queue, which will be processed by the
			audio loop. This Sound object is now owned by the SoundEngine and will be
			freed by it.
		*/
		static SoundID CreateNewSound(const std::string& resource, Sound* nSound) { return m_instance->AddCreateSoundCommand(resource, nSound); }

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

		void LoadResourceImpl(const std::string& alias, const std::string& filepath);
		void AddResourceToMap(LoadResourceData* data);
		void AddSound(CreateSoundData* data);
		void PlaySound(PlaySoundData* data);

		SoundID AddCreateSoundCommand(const std::string& resource, Sound* nSound);
		void AddPlaySoundCommand(const SoundID sID, const Vec3f& srcPos, bool looping);
		SoundID GetNextSoundID();

	};
}