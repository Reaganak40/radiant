#pragma once
#include "MathTypes.h"

namespace Radiant {
	enum ColorType {
		Blue,
		Red,
		Green,
	};

	class Color {
	private:
		Vec4f m_color;

	public:
		Color();
		Color(float r, float g, float b, float a, bool normalized=true);
		Color(ColorType color);
		~Color() {}

		const Vec4f& GetColor() const { return m_color; }
	};
}