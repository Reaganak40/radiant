#pragma once
#include "SoundBuffer.h"
#include "SoundSource.h"
namespace rdt {
	typedef unsigned int SoundID;

	struct Sound {
		SoundBuffer buffer;
		SoundSource source;
	};

	class SoundManager {
	private:
		SoundManager();
		~SoundManager();
		static SoundManager* m_instance;
		SoundID m_SoundCounter;

		ALCdevice* m_ALCdevice;
		ALCcontext* m_context;

		std::unordered_map<SoundID, Sound> m_sounds;

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
			Loads an audio file contents into the the SoundManager for use.
			Returns the SoundID to access it later.
		*/
		static SoundID LoadAudio(const std::string& audioFilepath) {
			return m_instance->LoadAudioImpl(audioFilepath);
		}

		/*
			Plays the sound referenced by the provided SoundID.
		*/
		static void PlaySound(const SoundID sID) { m_instance->PlaySoundImpl(sID); }

	private:
		SoundID LoadAudioImpl(const std::string& audioFilepath);
		void PlaySoundImpl(const SoundID sID);
		SoundID GetNextSoundID();

	};
}