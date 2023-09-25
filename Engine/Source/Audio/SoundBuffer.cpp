#include "pch.h"
#include "SoundBuffer.h"

namespace rdt {
	SoundBuffer::SoundBuffer()
	{
	}
	SoundBuffer::~SoundBuffer()
	{
	}

	bool SoundBuffer::LoadFile(const char* filepath)
	{
		return m_file.load(filepath);
	}
}
