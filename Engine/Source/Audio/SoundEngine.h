#pragma once
#include "SoundSource.h"
#include "Sound.h"

namespace rdt {
	typedef unsigned int SoundID;

	class SoundEngine {
	private:
		SoundEngine();
		~SoundEngine();
		static SoundEngine* m_instance;
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
			Plays the sound referenced by the provided SoundID at the provided
			source location. Looping is set to false by default.
		*/
		static void PlaySound(const SoundID sID, const Vec3f& srcPos, bool looping = false) { m_instance->PlaySoundImpl(sID, srcPos, looping); }

	private:
		SoundID LoadAudioImpl(const std::string& audioFilepath);
		void PlaySoundImpl(const SoundID sID, const Vec3f& srcPos, bool looping);
		SoundID GetNextSoundID();

	};
}