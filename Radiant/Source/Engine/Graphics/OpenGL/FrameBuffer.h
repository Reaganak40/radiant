#pragma once

namespace rdt::core {

	typedef unsigned int FBO_ID;

	class FrameBuffer {
	private:
		FBO_ID m_ID;
		unsigned int m_texture;
		unsigned int m_RBO;

		int m_window_width;
		int m_window_height;

	public:
		FrameBuffer();
		~FrameBuffer();

		/* Gets the associated frame buffer object id. */
		FBO_ID GetID();

		/* Returns the texture that belongs to the frame. */
		unsigned int GetTexture();

		/* Initializes the FrameBuffer object with the OpenGL context. */
		void Init();

		/* Binds the framebuffer object. */
		static void Bind(FBO_ID fbo);

		/* Unbinds the framebuffer object. */
		static void Unbind();

		/* Rescales the framebuffer window. */
		void Rescale(int nWindowWidth, int nWindowHeight);
	};
}