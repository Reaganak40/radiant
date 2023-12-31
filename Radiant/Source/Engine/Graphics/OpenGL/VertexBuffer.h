#pragma once
#include "pch.h"
#include "Graphics/Mesh.h"

#define RDT_NULL_VERTEX_BUFFER_ID 0
namespace rdt::core {
	using VBO_ID = unsigned int;

#define DEFAULT_VERTEX_BUFFER_LIMIT 64

	class VertexBuffer {
	private:
		VBO_ID m_ID;
		bool m_buffer_defined;

		std::vector<Vertex> m_buffer;
		unsigned int m_last_count;
		unsigned int m_current_count;

	public:
		VertexBuffer();
		~VertexBuffer();

		/*
			Adds the list of vertices to the end of the buffer.
		*/
		void PushToBatch(const std::vector<Vertex>& vertices);

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
		inline VBO_ID GetID() { return m_ID; }

		/*
			Statically binds a vertex buffer with the provided ID.
		*/
		static void Bind(VBO_ID id);

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