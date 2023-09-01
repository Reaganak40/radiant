#include "pch.h"
#include "Color.h"

namespace Radiant {
	Color::Color()
		: m_color(Vec4f(0.0f, 0.0f, 0.0f, 1.0f))
	{
	}
	Color::Color(float r, float g, float b, float a, bool normalized)
		: m_color(Vec4f(r, g, b, a))
	{
		if (!normalized) {
			m_color.a /= 255.0f;
			m_color.b /= 255.0f;
			m_color.c /= 255.0f;
			m_color.d /= 255.0f;
		}
	}

	Color::Color(ColorType color)
	{
		switch (color) {
		case ColorType::Blue:
			m_color = Vec4f(0.2f, 0.2f, 0.8f, 1.0f);
			break;
		case ColorType::Red:
			m_color = Vec4f(0.8f, 0.2f, 0.2f, 1.0f);
			break;
		case ColorType::Green:
			m_color = Vec4f(0.2f, 0.8f, 0.2f, 1.0f);
			break;
		default:
			break;
		}
	}

}
