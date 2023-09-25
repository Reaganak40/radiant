#pragma once

namespace rdt {
	class SoundManager {
	private:
		SoundManager();
		~SoundManager();
		static SoundManager* m_instance;

		ALCdevice* m_ALCdevice;
		ALCcontext* m_context;

	public:
		/*
			Creates the OpenAL context for game audio.
		*/
		static void Initialize();

		/*
			Destroys the OpenAL context and frees the singleton.
		*/
		static void Destroy();
	};
}