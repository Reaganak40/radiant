#include "pch.h"
#include "RenderLayer.h"
#include "Texture/TextureManager.h"

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

	void RenderLayer::AddMesh(Mesh& nMesh, const RenderType type)
	{
		switch (type) {
		case DrawFilled:
		{
			TextureID tID = nMesh.texture == nullptr ? 0 : nMesh.texture->GetID();
			if (m_textured_meshes.find(tID) == m_textured_meshes.end()) {
				m_textured_meshes[tID] = {};
			}
			m_textured_meshes.at(tID).push_back(&nMesh);
		}
			break;
		case DrawOutline:
			m_outline_meshes.push_back(&nMesh);
			break;
		case DrawLine:
			m_line_meshes.push_back(&nMesh);
			break;
		}
	}

	void RenderLayer::CompileBatches()
	{

		int unitIndex = 0;

		// Add batches for with textures
		for (auto& [tID, meshes] : m_textured_meshes) {
			if (meshes.size() == 0) {
				continue;
			}

			if (m_batches.size() <= unitIndex) {
				PushRenderUnit();
			}

			for (auto& mesh : meshes) {
				if (TextureManager::ApplyTextureAtlas(mesh->texture, mesh->texAtlasCoords, mesh->vertices)) {
					m_updated_texture_slots = true;
				}
				m_batches[unitIndex].m_VBO->PushToBatch(mesh->vertices);
				m_batches[unitIndex].m_IBO->PushToBatch(mesh->indices, mesh->vertices.size());
			}
			m_batches[unitIndex].type = DrawFilled;
			unitIndex++;
		}
		
		// Add polygon outline batch
		if (m_outline_meshes.size() > 0) {
			if (m_batches.size() <= unitIndex) {
				PushRenderUnit();
			}
			for (auto& mesh : m_outline_meshes) {
				m_batches[unitIndex].m_VBO->PushToBatch(mesh->vertices);
				m_batches[unitIndex].m_IBO->PushToBatch(mesh->indices, mesh->vertices.size());
			}
			m_batches[unitIndex].type = DrawOutline;
			unitIndex++;
		}

		// Add line batch
		if (m_line_meshes.size() > 0) {
			if (m_batches.size() <= unitIndex) {
				PushRenderUnit();
			}
			for (auto& mesh : m_line_meshes) {
				m_batches[unitIndex].m_VBO->PushToBatch(mesh->vertices);
				m_batches[unitIndex].m_IBO->PushToBatch(mesh->indices, mesh->vertices.size());
			}
			m_batches[unitIndex].type = DrawLine;
			unitIndex++;
		}

		m_batchCount = unitIndex;
	}

	bool RenderLayer::TextureSlotsChanged()
	{
		return m_updated_texture_slots;
	}

	unsigned int RenderLayer::GetBatchCount()
	{
		return m_batchCount;
	}

	std::vector<glRenderUnit>& RenderLayer::GetRenderUnits()
	{
		return m_batches;
	}

	void RenderLayer::Flush()
	{
		m_updated_texture_slots = false;

		for (auto& [tID, meshes] : m_textured_meshes) {
			m_textured_meshes.at(tID).clear();
		}
		m_outline_meshes.clear();
		m_line_meshes.clear();

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
}