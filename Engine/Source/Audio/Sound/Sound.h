#pragma once
#include "Audio/SoundData.h"
#include "Audio/SoundSource.h"
namespace rdt {

// 32kb of data in each buffer
#define AUDIO_BUFFER_SIZE 65536
#define NUM_AUDIO_BUFFERS 4

	enum SoundState {
		NO_BINDING,
		NOT_PLAYING,
		IS_PLAYING,
	};

	class Sound {
	private:
		
	protected:
		SoundState m_state;
		ALuint m_buffers[NUM_AUDIO_BUFFERS];
		std::vector<ALuint> m_unused_buffers;
		std::size_t m_cursor;
		SoundData* m_data;
		SoundSource m_source;
		bool m_shouldLoop;

		/*
			Configures the OpenAL buffers with the provided SoundData.
			This should be called by OnCreate(). Also preloads the buffers
			with the SoundData.
		*/
		void InitBuffers(SoundData* nData);

		/*
			Loads the starting data from the audio file into the buffers.
		*/
		void PreLoadBuffers();

		/*
			Function to be called every frame that the sound is playing.
		*/
		void OnPlaying();

	public:
		Sound();
		~Sound();

		/*
			Plays the loaded sound from the provided source.
		*/
		void PlaySound(const Vec3f& srcPos, bool loop);

		/*
			Stops playing the sound if it is currently running.
		*/
		void StopSound();

		/*
			When this Sound is first created and added to the SoundEngine
			audio queue. 
		*/
		virtual void OnCreate(SoundData* data) {}

		/*
			Allows for more dynamic logic with this sound during its Update Cylce.
		*/
		virtual void OnUpdate() {}

		/*
			Called when this sound starts playing its audio.
		*/
		virtual void OnStart() {}

		/*
			Called when this sound is stopped prematurely (i.e, paused)
		*/
		virtual void OnPause() {}

		/*
			Called when this sound reached the end of its audio play cylce.
		*/
		virtual void OnEndOfCycle() {}

		/*
			Called when this sound has finished playing and has exited the sound queue.
		*/
		virtual void OnStop() {}

		/*
			Returns the current state of the audio sound.
		*/
		SoundState GetState() { return m_state; }
		
	private:
		/*
			Called in the case the file data is smaller than the full
			buffer space, and a loop is requested: fill the remaining buffers.
		*/
		void UseUnusedBuffers();

		void UpdateStream(const ALuint source, bool loop);
	};
}
