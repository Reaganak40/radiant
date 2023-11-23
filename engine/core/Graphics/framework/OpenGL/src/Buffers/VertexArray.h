#pragma once

namespace rdt::glCore {

	using VAO_ID = unsigned int;

	class VertexArray {
	private:
		static VAO_ID currently_bounded_VAO;
		VAO_ID m_ID;
	public:
		VertexArray();
		~VertexArray();

		VAO_ID GetID() const;
		void Bind() const;

		void DefineVertexBufferLayout();
	};

}