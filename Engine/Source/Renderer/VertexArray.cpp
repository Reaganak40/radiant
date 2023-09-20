#include "pch.h"
#include "VertexArray.h"
#include "Mesh.h"

namespace rdt {

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_ID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void VertexArray::Bind(VAO_ID id)
	{
		glBindVertexArray(id);
	}

	void VertexArray::DefineVertexBufferLayout()
	{
		// define position coordinates
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

		// define color coordinates
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

		//define texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoords));

		// define texture id (slot)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texIndex));

	}
}