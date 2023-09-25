#pragma once

namespace rdt {
	class SoundBuffer {
	private:
		AudioFile<float> m_file;
	public:
		SoundBuffer();
		~SoundBuffer();

		/*
			Loads a sound file from the provided filepath, returns
			true if the load was successful.
		*/
		bool LoadFile(const char* filepath);
	};
}