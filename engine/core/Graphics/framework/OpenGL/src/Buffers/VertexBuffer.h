#pragma once
#include "Vertex.h"

#define GL_CORE_NULL_VERTEX_BUFFER_ID 0
namespace rdt::glCore {
	using VBO_ID = unsigned int;

#define DEFAULT_VERTEX_BUFFER_LIMIT 64

	class VertexBuffer {
	private:
		VBO_ID m_ID;

		bool m_buffer_defined;

		std::vector<Vertex> m_buffer;
		unsigned int m_last_count;
		unsigned int m_current_count;

		int* refCount;

	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer& other);
		~VertexBuffer();

		/*
			Adds the list of vertices to the end of the buffer.
		*/
		void PushToBatch(Vertex* vertices, size_t vertexCount);

		/*
			Sends new vertex buffer data to GPU via opengl.
		*/
		void Update();

		/*
			Clears the vertex buffer of all vertices.
		*/
		void Flush();

		/*
			Returns the registered vertex buffer object id for this instance.
		*/
		VBO_ID GetID() const;

		/*
			Binds a vertex buffer to the OpenGL context.
		*/
		void Bind() const;

		/*
			Returns the current vertex buffer data
		*/
		std::vector<Vertex>& GetBufferData();

		/*
			Returns the number of vertices in the buffer 
		*/
		unsigned int GetVertexCount();

	private:
		void RegisterBufferData();

		void UpdateBuffer();
		void ExtendBuffer();
	};

}