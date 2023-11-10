#pragma once
#include "pch.h"

#define RDT_NULL_INDEX_BUFFER_ID 0

namespace rdt::core {
	using IBO_ID = unsigned int;

	class IndexBuffer {
	private:
		IBO_ID m_ID;

		size_t m_vertex_count;
		bool m_should_update;

		std::vector<unsigned int> m_buffer;
		unsigned int m_current_count;
		unsigned int m_last_count;

	public:
		IndexBuffer();
		~IndexBuffer();


		void PushToBatch(const std::vector<unsigned int>& indices, size_t vertexCount);

		/*
			Sends new index buffer data to GPU via opengl.
		*/
		void Update();

		/*
			Clears the index buffer of all index data.
		*/
		void Flush();

		/*
			Gets the IBO indentifier for this instace.
		*/
		inline IBO_ID GetID() { return m_ID; }

		/*
			Gets the number of indices in the buffer.
		*/
		inline unsigned int GetIndexCount() { return m_current_count; }


		/*
			Statically binds an index buffer through its provided ID.
		*/
		static void Bind(IBO_ID id);
	};

}
