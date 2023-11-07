#include "pch.h"
#include "RenderLayer.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/Texture/TextureManager.h"
#include "Graphics/Model.h"
#include "Utils/Utils.h"

namespace rdt::core {
	RenderLayer::RenderLayer()
	{
		m_batchCount = 0;
		m_default_shader = 0;
		m_updated_texture_slots = false;
	}

	RenderLayer::~RenderLayer()
	{
	}

	void RenderLayer::SetDefaultShader(ShaderID nDefaultShaderID)
	{
		m_default_shader = nDefaultShaderID;
	}

	void RenderLayer::AddMesh(Mesh& nMesh)
	{
		if (m_meshes.find(nMesh.textureID) == m_meshes.end()) {
			m_meshes[nMesh.textureID];
		}

		m_meshes.at(nMesh.textureID).push_back(nMesh);
	}

	void RenderLayer::CompileBatches()
	{
		m_updated_texture_slots = true;

		auto getTexCoords = [](int tci, const AtlasProfile& profile) {

			Vec2f res;
			switch (tci) {
			case 0:
				res.x = profile.normalizedStartX;
				res.y = profile.normalizedStartY + profile.normalizedHeight;
				break;
			case 1:
				res.x = profile.normalizedStartX + profile.normalizedWidth;
				res.y = profile.normalizedStartY + profile.normalizedHeight;
				break;
			case 2:
				res.x = profile.normalizedStartX + profile.normalizedWidth;
				res.y = profile.normalizedStartY;
				break;
			case 4:
				res.x = profile.normalizedStartX;
				res.y = profile.normalizedStartY;
				break;
			}

			return res;

			};

		int unitIndex = 0;
		for (auto& [tID, meshes] : m_meshes) {
			if (meshes.size() == 0) {
				continue;
			}

			if (m_batches.size() <= unitIndex) {
				PushRenderUnit();
			}

			for (auto& mesh : meshes) {

				std::vector<Vec2f> vertex_positions(ModelManager::ModelExists(mesh.modelID) ? ModelManager::GetVertices(mesh.modelID) : std::vector<Vec2f>());
				std::vector<unsigned int> indices(ModelManager::ModelExists(mesh.modelID) ? ModelManager::GetIndices(mesh.modelID) : std::vector<unsigned int>());
				ApplyTransform(mesh, vertex_positions);

				std::vector<Vertex> vertices;
				int texture_coordinate_index = 0;
				for (auto& position : vertex_positions) {

					Vec2f texCoords = getTexCoords(texture_coordinate_index, mesh.atlasProfile);
					texture_coordinate_index = (texture_coordinate_index + 1) % 4;
					float texIndex = (float)TextureManager::GetTexture(tID).CurrentTextureSlot();
					
					vertices.push_back(Vertex(Vec3f(position.x, position.y), mesh.fillColor, texCoords, texIndex));
				}

				m_batches[unitIndex].m_VBO->PushToBatch(vertices);
				m_batches[unitIndex].m_IBO->PushToBatch(indices, vertices.size());
			}
			m_batches[unitIndex].type = DrawFilled;
			unitIndex++;
		}
	}

	bool RenderLayer::TextureSlotsChanged()
	{
		return m_updated_texture_slots;
	}

	unsigned int RenderLayer::GetBatchCount()
	{
		return m_batches.size();
	}

	std::vector<glRenderUnit>& RenderLayer::GetRenderUnits()
	{
		return m_batches;
	}

	void RenderLayer::Flush()
	{
		m_updated_texture_slots = false;

		m_meshes.clear();

		for (auto& unit : m_batches) {
			unit.m_IBO->Flush();
			unit.m_VBO->Flush();
		}

		m_batchCount = 0;
	}

	void RenderLayer::PushRenderUnit()
	{
		m_batches.push_back(glRenderUnit());
		m_batches.back().InitBuffers();
		m_batches.back().shaderID = m_default_shader;
	}

	void RenderLayer::ApplyTransform(const Mesh& mesh, std::vector<Vec2f>& vertices)
	{
		// Scale, Rotate, Translate
		for (auto& vertex : vertices) {
			vertex = Utils::Scale(Vec2f::Zero(), vertex, mesh.scale);

			if (mesh.rotation != 0.0f) {
				Utils::RotatePoint(Vec2f::Zero(), vertex, mesh.rotation);
			}

			Utils::Translate(vertex, mesh.position);
		}
	}
}