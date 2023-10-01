#pragma once
#include "Sound.h"

namespace rdt {

	/*
		Standard Sound class for the Radiant engine, that allows for easy implementation and
		usage of sound effects with the SoundEngine.
	*/
	class SoundEffect : public Sound {
	private:
	protected:

	public:
		SoundEffect();
		~SoundEffect();


		/*
			When this Sound is first created and added to the SoundEngine
			audio queue. Returns true if creation was successful and no errors
			found.

			Child SoundEffect classes should return this if it overrides OnCreate().
		*/
		virtual void OnCreate(core::SoundData* data) override;

		/*
			Allows for more dynamic logic with this sound during its Update Cylce.
		*/
		virtual void OnUpdate() override;

		/*
			Called when this sound starts playing its audio.
		*/
		virtual void OnStart() override;

		/*
			Called when this sound is stopped prematurely (i.e, paused)
		*/
		virtual void OnPause() override;

		/*
			Called when this sound reached the end of its audio play cylce.
		*/
		virtual void OnEndOfCycle() override;

		/*
			Called when this sound has finished playing and has exited the sound queue.
		*/
		virtual void OnStop() override;
	};
}