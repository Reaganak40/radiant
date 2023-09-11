#include "pch.h"
#include "VertexBuffer.h"

namespace rdt {

	VertexBuffer::VertexBuffer()
		: m_buffer_defined(false), m_current_count(0), m_last_count(0)
	{
		m_buffer.reserve(DEFAULT_VERTEX_BUFFER_LIMIT);
		glGenBuffers(1, &m_ID);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void VertexBuffer::PushToBatch(const std::vector<Vertex>& vertices)
	{
		if ((m_current_count + vertices.size()) >= m_buffer.capacity()) {
			ExtendBuffer();
		}

		for (const auto& vertex : vertices) {

			if (m_current_count == m_buffer.size()) {
				m_buffer.push_back(vertex);
			}
			else {
				m_buffer[m_current_count] = vertex;
			}
			m_current_count++;
		}
	}

	void VertexBuffer::Update()
	{
		if (!m_buffer_defined) {
			RegisterBufferData();
			m_buffer_defined = true;
		}

		UpdateBuffer();
	}

	void VertexBuffer::Flush()
	{
		m_current_count = 0;
	}

	void VertexBuffer::Bind(VBO_ID id)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void VertexBuffer::RegisterBufferData()
	{
		glBufferData(GL_ARRAY_BUFFER, m_buffer.capacity() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	}


	void VertexBuffer::UpdateBuffer()
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_current_count * sizeof(Vertex), m_buffer.data());
		m_last_count = m_current_count;
	}

	void VertexBuffer::ExtendBuffer()
	{
		m_buffer.reserve(m_buffer.capacity() * 2);
		m_buffer_defined = false;
	}
}