/*******************************************************************
*	Module:  Graphics (core)
*	File:    DrawCall.h
*
*	Author: Reagan Kelley
*
*	Contains the buffers and data for a single opengl draw call.
*******************************************************************/
#pragma once
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

//Forward Delcarations
namespace rdt::core {
	using TextureID = unsigned int;
	class Mesh;
}

// Required Definitions for Struct/Class Members
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace rdt::core {

	class glDrawCall {
	private:
		VBO_ID vboID;
		IBO_ID iboID;
		ShaderID shaderID;
		std::shared_ptr<VertexBuffer> m_VBO;
		std::shared_ptr<IndexBuffer> m_IBO;

		TextureID m_texture;
		TextureSlot m_slot;
	public:
		glDrawCall();
		~glDrawCall();

		/*
			Creates a new instance of the index and vertex buffers
		*/
		void InitBuffers();

		/*
			Calls update on the vertex and index buffers
		*/
		void UpdatBuffers();

		/*
			Flushes the data from the vertex and index buffers
		*/
		void Flush();

		/*
			Returns true if there are no meshes in this draw call
		*/
		bool IsEmpty();

		/*
			Sets the shader to be used for this draw call
		*/
		void SetShader(ShaderID nShader);

		/*
			Assigns the texture this draw call is responsible for
		*/
		void AssignTexture(TextureID texture);

		/*
			Returns the texture this draw call is responsible for
		*/
		TextureID GetAssignedTexture();

		/*
			Adds a mesh to this draw call
		*/
		void AddMesh(const Mesh& mesh);

		/*
			Updates the vertex data to associated the given
			texture slot with its assigned texture.
		*/
		void UpdateTextureSlotIndex(TextureSlot textureSlot);

		friend class RendererGL;
	};

	struct glDrawCallBucket {
		std::vector<glDrawCall> m_draw_calls;

		/*
			Adds a new needed draw call with the requested textureID assigned
		*/
		glDrawCall& AddDrawCall(TextureID textureID);
	};
	class DrawCallAllocator {
	private:
		std::map<unsigned int, glDrawCallBucket> m_layers;
		std::vector<glDrawCall*> m_draw_order;
		ShaderID m_default_shader;
	public:

		DrawCallAllocator();

		/*
			Adds a mesh to the glDrawCall queue
		*/
		void AddMesh(const Mesh& mesh);

		/*
			Returns the draw calls in order
		*/
		const std::vector<glDrawCall*>& GetDrawCalls();

		/*
			Resets the DrawCallAllocator draw calls
		*/
		void Flush();

		/*
			Sets the default shader to be assigned to newly instantiated draw calls
		*/
		void SetDefaultShader(ShaderID defaultShader);

	private:

		/*
			Returns true if the given layer exists
		*/
		bool HasLayer(unsigned int layer);
	};
}
