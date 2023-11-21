#include "pch.h"
#include "Vertex.h"

rdt::glCore::Vertex::Vertex()
{
	float zeros[4] = { 0 };

	memcpy_s(position, 3, zeros, 3);
	memcpy_s(color, 4, zeros, 4);
	memcpy_s(texCoords, 2, zeros, 2);
	texIndex = 0.0f;
}

void rdt::glCore::Vertex::SetPosition(const float nPosition[3])
{
	memcpy_s(position, sizeof(position), nPosition, sizeof(nPosition));
}

void rdt::glCore::Vertex::SetPosition(const float x, const float y, const float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

void rdt::glCore::Vertex::SetColor(const float nColor[4])
{
	memcpy_s(color, sizeof(color), nColor, sizeof(nColor));
}

void rdt::glCore::Vertex::SetColor(const float r, const float g, const float b, const float a)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}

void rdt::glCore::Vertex::SetTexCoords(const float nTexCoords[2])
{
	memcpy_s(texCoords, sizeof(texCoords), nTexCoords, sizeof(nTexCoords));
}

void rdt::glCore::Vertex::SetTexCoords(const float x, const float y)
{
	texCoords[0] = x;
	texCoords[0] = y;
}

void rdt::glCore::Vertex::SetTexIndex(float nIndex)
{
	texIndex = nIndex;
}
