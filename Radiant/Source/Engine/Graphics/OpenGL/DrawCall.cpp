#include "pch.h"
#include "DrawCall.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/Texture/TextureAtlas.h"
#include "Graphics/Texture/TextureManager.h"
#include "Graphics/Model.h"
#include "Utils/MathTypes.h"

rdt::core::glDrawCall::glDrawCall()
{
	vboID = RDT_NULL_VERTEX_BUFFER_ID;
	iboID = RDT_NULL_INDEX_BUFFER_ID;
	shaderID = RDT_NULL_SHADER_ID;

	m_VBO = nullptr;
	m_IBO = nullptr;

	m_texture = RDT_NULL_TEXTURE_ID;
}
rdt::core::glDrawCall::~glDrawCall()
{
	if (m_VBO != nullptr) {
		delete m_VBO;
		delete m_IBO;
	}
}

void rdt::core::glDrawCall::InitBuffers()
{
	if (m_VBO != nullptr) {
		return;
	}
	m_VBO = new VertexBuffer;
	m_IBO = new IndexBuffer;
	vboID = m_VBO->GetID();
	iboID = m_IBO->GetID();
}

void rdt::core::glDrawCall::UpdatBuffers()
{
	m_VBO->Update();
	m_IBO->Update();
}

void rdt::core::glDrawCall::Flush()
{
	m_VBO->Flush();
	m_IBO->Flush();
}

bool rdt::core::glDrawCall::IsEmpty()
{
	return m_VBO->GetVertexCount() == 0;
}

void rdt::core::glDrawCall::SetShader(ShaderID nShader)
{
	shaderID = nShader;
}

void rdt::core::glDrawCall::AssignTexture(TextureID texture)
{
	m_slot = TextureManager::GetTexture(texture).CurrentTextureSlot();
	m_texture = texture;
}

rdt::TextureID rdt::core::glDrawCall::GetAssignedTexture()
{
	return m_texture;
}

void rdt::core::glDrawCall::AddMesh(const Mesh& mesh)
{
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

	// Get normalized vertex positions from Model
	std::vector<Vec2f> vertex_positions;
	ModelManager::ApplyTransform(mesh.modelID, mesh.transform, vertex_positions);
	std::vector<unsigned int> indices(ModelManager::ModelExists(mesh.modelID) ? ModelManager::GetIndices(mesh.modelID) : std::vector<unsigned int>());

	std::vector<Vertex> vertices;
	int texture_coordinate_index = 0;
	for (auto& position : vertex_positions) {

		Vec2f texCoords = getTexCoords(texture_coordinate_index, mesh.atlasProfile);
		texture_coordinate_index = (texture_coordinate_index + 1) % 4;
		float texIndex = (float)m_slot;

		vertices.push_back(Vertex(Vec3f(position.x, position.y), mesh.fillColor, texCoords, texIndex));
	}

	m_VBO->PushToBatch(vertices);
	m_IBO->PushToBatch(indices, vertices.size());
}
void rdt::core::glDrawCall::UpdateTextureSlotIndex(TextureSlot textureSlot)
{
	// If no change, no need to update the vertex data
	if (m_slot == textureSlot) {
		return;
	}

	m_slot = textureSlot;
	for (auto& vertex : m_VBO->GetBufferData()) {
		vertex.texIndex = (float)m_slot;
	}
}
// ================================================================================================
rdt::core::glDrawCall& rdt::core::glDrawCallBucket::AddDrawCall(TextureID textureID)
{
	m_draw_calls.push_back({});
	m_draw_calls.back().InitBuffers();
	m_draw_calls.back().AssignTexture(textureID);
	return m_draw_calls.back();
}
// ================================================================================================
rdt::core::DrawCallAllocator::DrawCallAllocator()
{
	m_default_shader = RDT_NULL_SHADER_ID;
}
void rdt::core::DrawCallAllocator::AddMesh(const Mesh& mesh)
{
	if (!HasLayer(mesh.layer)) {
		m_layers[mesh.layer];
	}

	// Add mesh to layer.texture draw call
	for (auto& drawCall: m_layers.at(mesh.layer).m_draw_calls) {
		if (drawCall.GetAssignedTexture() == mesh.layer) {
			drawCall.AddMesh(mesh);
			return;
		}
	}

	// Add mesh if layer has no texture for that draw call
	m_layers.at(mesh.layer).AddDrawCall(mesh.textureID).AddMesh(mesh);

	// Add default shader to new draw call
	m_layers.at(mesh.layer).m_draw_calls.back().SetShader(m_default_shader);
}

const std::vector<rdt::core::glDrawCall*>& rdt::core::DrawCallAllocator::GetDrawCalls()
{
	if (m_draw_order.size() == 0) {
		for (auto& [layer, bucket] : m_layers) {
			for (auto& drawCall : bucket.m_draw_calls) {
				if (!drawCall.IsEmpty()) {
					m_draw_order.push_back(&drawCall);
				}
			}
		}
	}

	return m_draw_order;
}

void rdt::core::DrawCallAllocator::Flush()
{
	m_draw_order.clear();
	for (auto& [layer, bucket] : m_layers) {
		for (auto& drawCall : bucket.m_draw_calls) {
			drawCall.Flush();
		}
	}
}

void rdt::core::DrawCallAllocator::SetDefaultShader(ShaderID defaultShader)
{
	m_default_shader = defaultShader;
}

bool rdt::core::DrawCallAllocator::HasLayer(unsigned int layer)
{
	return m_layers.find(layer) != m_layers.end();
}
