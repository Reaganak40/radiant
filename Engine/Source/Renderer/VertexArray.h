#pragma once

namespace rdt {

	typedef unsigned int VAO_ID;

	class VertexArray {
	private:
		VAO_ID m_ID;
	public:
		VertexArray();
		~VertexArray();

		static void Bind(VAO_ID id);
		inline VAO_ID GetID() { return m_ID; }

		void DefineVertexBufferLayout();

	};

}