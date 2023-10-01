#include "pch.h"
#include "IndexBuffer.h"

namespace rdt::core {

    IndexBuffer::IndexBuffer()
        : m_vertex_count(0), m_should_update(false), m_current_count(0), m_ID(0), m_last_count(0)
    {
        glGenBuffers(1, &m_ID);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void IndexBuffer::PushToBatch(const std::vector<unsigned int>& indices, size_t vertexCount)
    {
        for (const auto& index : indices) {

            if (m_current_count == m_buffer.size()) {
                m_buffer.push_back((unsigned int)m_vertex_count + index);
                m_should_update = true;
            }
            else {
                if ((m_vertex_count + index) != m_buffer[m_current_count]) {
                    m_buffer[m_current_count] = (unsigned int)m_vertex_count + index;
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

    void IndexBuffer::Bind(IBO_ID id)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

}