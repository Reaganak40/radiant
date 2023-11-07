#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Graphics/RenderTypes.h"

// Forward Declarations
namespace rdt::core {
	struct Mesh;
}

namespace rdt::core {
	
	// Opengl render unit contains all needed data for 1 draw call.
	struct glRenderUnit {
		VBO_ID vboID;
		IBO_ID iboID;
		ShaderID shaderID;

		VertexBuffer* m_VBO;
		IndexBuffer* m_IBO;

		RenderType type;

		glRenderUnit() {
			m_VBO = nullptr;
			m_IBO = nullptr;
			vboID = 0;
			iboID = 0;
			shaderID = 0;
			type = DrawFilled;
		}

		void InitBuffers() {
			if (m_VBO != nullptr) {
				return;
			}
			m_VBO = new VertexBuffer;
			m_IBO = new IndexBuffer;
			vboID = m_VBO->GetID();
			iboID = m_IBO->GetID();
		}

		void DeleteBuffers() {
			if (m_VBO != nullptr) {
				delete m_VBO;
			}

			if (m_IBO != nullptr) {
				delete m_IBO;
			}
		}
	};

	class RenderLayer {
	private:
		ShaderID m_default_shader;
		std::vector<glRenderUnit> m_batches;
		unsigned int m_batchCount;

		std::unordered_map<TextureID, std::vector<Mesh>> m_meshes;

		bool m_updated_texture_slots;
	public:
		RenderLayer();
		~RenderLayer();

		/*
			Sets the default shader that is used for most draw calls in the layer.
		*/
		void SetDefaultShader(ShaderID nDefaultShaderID);

		/*
			Adds a mesh that is intended to be draw at this layer.
		*/
		void AddMesh(Mesh& nMesh);

		/*
			Sorts the meshes and compiles them into opengl render batches.
		*/
		void CompileBatches();

		/*
			Returns true if the texture slots changed when compiling the batches.
		*/
		bool TextureSlotsChanged();

		/*
			Returns the current number of batched render units.
		*/
		unsigned int GetBatchCount();

		/*
			Gets the compiled render units.
		*/
		std::vector<glRenderUnit>& GetRenderUnits();

		/*
			Resets the layer for next draw cycle.
		*/
		void Flush();

	private:
		void PushRenderUnit();

		/*
			Uses the transform data in the mesh to position, scale, and rotation the vertices
		*/
		void ApplyTransform(const Mesh& mesh, std::vector<Vec2f>& vertices);
	};
}