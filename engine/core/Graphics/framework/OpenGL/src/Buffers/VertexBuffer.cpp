#include "pch.h"
#include "VertexBuffer.h"
#include "Internal/State.h"

namespace rdt::glCore {

	VertexBuffer::VertexBuffer()
		: m_buffer_defined(false), m_current_count(0), m_last_count(0), m_ID(0)
	{
		m_buffer.reserve(DEFAULT_VERTEX_BUFFER_LIMIT);
		glGenBuffers(1, &m_ID);

		refCount = new int(1);
	}

	VertexBuffer::VertexBuffer(const VertexBuffer& other)
	{
		m_ID = other.m_ID;
		m_buffer_defined = other.m_buffer_defined;
		m_current_count = other.m_current_count;
		m_last_count = other.m_last_count;
		m_buffer = other.m_buffer;

		refCount = other.refCount;
		(*refCount)++;
	}

	VertexBuffer::~VertexBuffer()
	{
		if (--(*refCount) == 0) {
			glDeleteBuffers(1, &m_ID);
			delete refCount;
		}
	}

	void VertexBuffer::PushToBatch(Vertex* vertices, size_t vertexCount)
	{
		if ((m_current_count + vertexCount) >= m_buffer.capacity()) {
			ExtendBuffer();
		}

		for (size_t i = 0; i < vertexCount; i++) {
			if (m_current_count == m_buffer.size()) {
				m_buffer.push_back(vertices[i]);
			}
			else {
				m_buffer[m_current_count] = vertices[i];
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

	VBO_ID VertexBuffer::GetID() const
	{
		return m_ID;
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	std::vector<Vertex>& VertexBuffer::GetBufferData()
	{
		return m_buffer;
	}

	unsigned int VertexBuffer::GetVertexCount()
	{
		return m_current_count;
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