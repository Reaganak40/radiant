#include "pch.h"
#include "Layer.h"
#include "Texture/TextureManager.h"

namespace rdt {
	Layer::Layer()
	{
		m_default_shader = 0;
		m_updated_texture_slots = false;

	}

	Layer::~Layer()
	{
	}

	void Layer::SetDefaultShader(ShaderID nDefaultShaderID)
	{
		m_default_shader = nDefaultShaderID;
	}

	void Layer::AddMesh(Mesh& nMesh, const RenderType type)
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

	void Layer::CompileBatches()
	{
		for (auto& [tID, meshes] : m_textured_meshes) {
			if (meshes.size() == 0) {
				m_textured_meshes.erase(tID);
			}
		}

		// Reserve or resize vector based on number of batches
		int numBatches = m_textured_meshes.size() + 2;
		if (m_batches.size() < numBatches) {
			m_batches.reserve(numBatches);
		}
		else if (m_batches.size() > numBatches) {
			m_batches.resize(numBatches);
		}

		int unitIndex = 0;

		// Add batches for with textures
		for (auto& [tID, meshes] : m_textured_meshes) {
			
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
		if (m_batches.size() <= unitIndex) {
			PushRenderUnit();
		}
		for (auto& mesh : m_outline_meshes) {
			m_batches[unitIndex].m_VBO->PushToBatch(mesh->vertices);
			m_batches[unitIndex].m_IBO->PushToBatch(mesh->indices, mesh->vertices.size());
		}
		m_batches[unitIndex].type = DrawOutline;
		unitIndex++;

		// Add line batch
		if (m_batches.size() <= unitIndex) {
			PushRenderUnit();
		}
		for (auto& mesh : m_line_meshes) {
			m_batches[unitIndex].m_VBO->PushToBatch(mesh->vertices);
			m_batches[unitIndex].m_IBO->PushToBatch(mesh->indices, mesh->vertices.size());
		}
		m_batches[unitIndex].type = DrawLine;
	}

	bool Layer::TextureSlotsChanged()
	{
		return m_updated_texture_slots;
	}

	std::vector<glRenderUnit>& Layer::GetRenderUnits()
	{
		return m_batches;
	}

	void Layer::Flush()
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

	}

	void Layer::PushRenderUnit()
	{
		m_batches.push_back(glRenderUnit());
		m_batches.back().InitBuffers();
		m_batches.back().shaderID = m_default_shader;
	}
}