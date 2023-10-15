#pragma once
#include "Core.h"
#include "MathTypes.h"

namespace rdt {
	enum ColorType {
		BLUE,
		RED,
		GREEN,
		ORANGE,
		BLACK,
		WHITE,
		POWERSHELL_BLUE,
		YELLOW,
		PINK
	};

	class RADIANT_API Color {
	private:
		Vec4f m_color;

	public:
		Color();
		Color(float r, float g, float b, float a, bool normalized=true);
		Color(ColorType color);
		~Color() {}

		const Vec4f& SetColor(float r, float g, float b, float a, bool normalized = true);
		const Vec4f& SetColor(const ColorType& nColor);


		const Vec4f& GetColor() const { return m_color; }

		inline Color operator=(const ColorType& nColor) {
			SetColor(nColor);
			return (*this);
		}

		const Vec4f& SetRed(float r, bool normalized=true);
		const Vec4f& SetGreen(float g, bool normalized = true);
		const Vec4f& SetBlue(float b, bool normalized = true);
		const Vec4f& SetAlpha(float a, bool normalized = true);

	};
}