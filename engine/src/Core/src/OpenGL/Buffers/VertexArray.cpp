#include "pch.h"
#include "VertexArray.h"
#include "Vertex.h"

namespace rdt::glCore {

	VAO_ID VertexArray::currently_bounded_VAO = 0;

	VertexArray::VertexArray()
		: m_ID(0)
	{
		glGenVertexArrays(1, &m_ID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void VertexArray::Bind() const
	{
		if (currently_bounded_VAO == m_ID) {
			return;
		}
		glBindVertexArray(m_ID);
	}

	VAO_ID VertexArray::GetID() const
	{
		return m_ID;
	}

	void VertexArray::DefineVertexBufferLayout()
	{
		// define position coordinates
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glVertex), (const void*)offsetof(glVertex, position));

		// define color coordinates
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glVertex), (const void*)offsetof(glVertex, color));

		//define texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glVertex), (const void*)offsetof(glVertex, texCoords));

		// define texture id (slot)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(glVertex), (const void*)offsetof(glVertex, texIndex));

	}
}