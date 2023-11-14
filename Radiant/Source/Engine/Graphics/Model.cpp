#include "pch.h"
#include "Model.h"
#include "Logging/Log.h"

#include "ECS/CommonComponents.h"
#include "Utils/Utils.h"

namespace rdt {
	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	void Model::DefineCommonRect()
	{
		m_vertices.push_back({ 0.0f, 0.0f });
		m_vertices.push_back({ 1.0f, 0.0f });
		m_vertices.push_back({ 1.0f, 1.0f });
		m_vertices.push_back({ 0.0f, 1.0f });

		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);
		
		m_indices.push_back(2);
		m_indices.push_back(3);
		m_indices.push_back(0);
	}

	std::vector<Vec2f>& Model::GetVertices()
	{
		return m_vertices;
	}

	std::vector<unsigned int>& Model::GetIndices()
	{
		return m_indices;
	}

	void Model::ApplyTransform(const Transform& transform, std::vector<Vec2f>& vertices)
	{
		for (const auto& vertex : m_vertices) {
			vertices.push_back(Utils::Scale(Vec2f::Zero(), vertex, transform.scale));
			Utils::RotatePoint(Vec2f::Zero(), vertices.back(), transform.rotation);
			Utils::Translate(vertices.back(), transform.position);
		}
	}

	void Model::Reset()
	{
		m_vertices.clear();
		m_indices.clear();
	}

	// =============================================================================
	ModelManager* ModelManager::m_instance = nullptr;

	struct ModelManager::Impl {

		ModelID idGenerator = 0;
		std::unordered_map<std::string, ModelID> aliasToId;
		std::unordered_map<ModelID, Model> m_models;

		Impl() {}
		~Impl() {}

		ModelID Register(const std::string& name)
		{
			if (aliasToId.find(name) != aliasToId.end()) {
				RDT_CORE_WARN("ModelManager - Could not register model '{}', already exists.", name);
				return RDT_NULL_MODEL_ID;
			}

			ModelID nID = ++idGenerator;
			aliasToId[name] = nID;
			m_models[nID];
			return nID;
		}

		ModelID GetModelID(const std::string& name) {
			if (aliasToId.find(name) == aliasToId.end()) {
				RDT_CORE_WARN("ModelManager - Could not find model '{}'", name);
				return RDT_NULL_MODEL_ID;
			}

			return aliasToId.at(name);
		}
		
		bool ModelExists(ModelID mID) {
			return m_models.find(mID) != m_models.end();
		}

		std::vector<Vec2f>& GetVertices(ModelID model)
		{
			return m_models.at(model).GetVertices();
		}

		std::vector<unsigned int>& GetIndices(ModelID model)
		{
			return m_models.at(model).GetIndices();
		}

		const char* GetModelAlias(ModelID model)
		{
			for (auto& [name, id] : aliasToId) {
				if (model == id) {
					return name.c_str();
				}
			}

			return "NoNameFound";
		}
	};

	ModelManager::ModelManager()
		: m_impl(new ModelManager::Impl)
	{
	}

	ModelManager::~ModelManager()
	{
		delete m_impl;
	}

	void ModelManager::Initialize()
	{
		Destroy();
		m_instance = new ModelManager();
	}

	void ModelManager::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	ModelID ModelManager::RegisterModel(const std::string& modelName)
	{
		return m_instance->m_impl->Register(modelName);
	}
	ModelID ModelManager::GetModelID(const std::string& modelName)
	{
		return m_instance->m_impl->GetModelID(modelName);
	}

	bool ModelManager::ModelExists(ModelID model)
	{
		return m_instance->m_impl->ModelExists(model);
	}

	void ModelManager::LoadModelJson(const std::string& file_json)
	{
		// TODO
	}
	std::vector<Vec2f>& ModelManager::GetVertices(ModelID model)
	{
		if (!ModelExists(model)) {
			assert(false);
		}
		return m_instance->m_impl->GetVertices(model);
	}
	std::vector<unsigned int>& ModelManager::GetIndices(ModelID model)
	{
		if (!ModelExists(model)) {
			assert(false);
		}
		return m_instance->m_impl->GetIndices(model);
	}
	void ModelManager::ApplyTransform(ModelID model, const Transform& transform, std::vector<Vec2f>& vertices)
	{
		if (!ModelExists(model)) {
			return;
		}

		GetModel(model).ApplyTransform(transform, vertices);
	}

	const char* ModelManager::GetModelAlias(ModelID model)
	{
		return m_instance->m_impl->GetModelAlias(model);
	}

	Model& ModelManager::GetModel(ModelID model)
	{
		return m_instance->m_impl->m_models.at(model);
	}
}
