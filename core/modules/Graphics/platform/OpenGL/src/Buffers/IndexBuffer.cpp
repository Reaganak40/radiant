#include "pch.h"
#include "IndexBuffer.h"

namespace rdt::glCore {

    IndexBuffer::IndexBuffer()
        : m_vertex_count(0), m_should_update(false), m_current_count(0), m_ID(0), m_last_count(0)
    {
        glGenBuffers(1, &m_ID);
        refCount = new int(1);
    }

    IndexBuffer::IndexBuffer(const IndexBuffer& other)
    {
        m_ID = other.m_ID;
        m_should_update = other.m_should_update;
        m_current_count = other.m_current_count;
        m_vertex_count = other.m_vertex_count;
        m_last_count = other.m_last_count;
        m_buffer = other.m_buffer;

        refCount = other.refCount;
        (*refCount)++;
    }

    IndexBuffer::~IndexBuffer()
    {
        if (--(*refCount) == 0) {
            glDeleteBuffers(1, &m_ID);
            delete refCount;
        }
    }

    void IndexBuffer::PushToBatch(unsigned int* indices, size_t indexCount, size_t vertexCount)
    {
        for (size_t i = 0; i < indexCount; i++) {
            if (m_current_count == m_buffer.size()) {
                m_buffer.push_back((unsigned int)m_vertex_count + indices[i]);
                m_should_update = true;
            }
            else {
                if ((m_vertex_count + indices[i]) != m_buffer[m_current_count]) {
                    m_buffer[m_current_count] = (unsigned int)m_vertex_count + indices[i];
                    m_should_update = true;
                }
            }

            m_current_count++;
        }
        m_vertex_count += vertexCount;
    }

    void IndexBuffer::Update()
    {
        if (m_should_update || (m_last_count != m_current_count)) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_current_count * sizeof(unsigned int), m_buffer.data(), GL_STATIC_DRAW);
            m_should_update = false;
        }
        m_last_count = m_current_count;
    }

    void IndexBuffer::Flush()
    {
        m_current_count = 0;
        m_vertex_count = 0;
    }

    IBO_ID IndexBuffer::GetID() const
    {
        return m_ID;
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }
}