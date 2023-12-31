#pragma once
#include "Utils/MathTypes.h"

namespace rdt::core {
	class SoundSource {
	private:
		ALuint m_sourceID;

	public:
		SoundSource();
		~SoundSource();

		/*
			Returns the ID for this Source.
		*/
		ALuint GetID() const { return m_sourceID; }

		void Init();
		void SetPosition(const Vec3f& nPos);
		void SetVelocity(const Vec3f& nVel);
		void AttachBuffer(ALuint bufferID);
	};
}