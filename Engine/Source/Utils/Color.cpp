#include "pch.h"
#include "Color.h"

namespace Radiant {
	Color::Color()
		: m_color(Vec4f(0.0f, 0.0f, 0.0f, 1.0f))
	{
	}
	Color::Color(float r, float g, float b, float a, bool normalized)
	{
		SetColor(r, g, b, a, normalized);
	}

	Color::Color(ColorType color)
	{
		SetColor(color);
	}

	const Vec4f& Color::SetColor(float r, float g, float b, float a, bool normalized)
	{
		m_color = Vec4f(r, g, b, a);
		if (!normalized) {
			m_color.x1 /= 255.0f;
			m_color.x2 /= 255.0f;
			m_color.x3 /= 255.0f;
			m_color.x4 /= 255.0f;
		}

		return m_color;
	}

	const Vec4f& Color::SetColor(const ColorType& nColor)
	{
		switch (nColor) {
		case ColorType::BLUE:
			m_color = Vec4f(0.2f, 0.2f, 0.8f, 1.0f);
			break;
		case ColorType::RED:
			m_color = Vec4f(0.8f, 0.2f, 0.2f, 1.0f);
			break;
		case ColorType::GREEN:
			m_color = Vec4f(0.2f, 0.8f, 0.2f, 1.0f);
			break;
		default:
			m_color = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
			break;
		}

		return m_color;
	}

	const Vec4f& Color::SetRed(float r, bool normalized)
	{
		if (!normalized) {
			r /= 255.0f;
		}
		m_color.x1 = r;

		return m_color;
	}

	const Vec4f& Color::SetGreen(float g, bool normalized)
	{
		if (!normalized) {
			g /= 255.0f;
		}
		m_color.x2 = g;

		return m_color;
	}

	const Vec4f& Color::SetBlue(float b, bool normalized)
	{
		if (!normalized) {
			b /= 255.0f;
		}
		m_color.x3 = b;

		return m_color;
	}

	const Vec4f& Color::SetAlpha(float a, bool normalized)
	{
		if (!normalized) {
			a /= 255.0f;
		}
		m_color.x4 = a;

		return m_color;
	}

}
