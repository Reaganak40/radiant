#include "pch.h"
#include "Model.h"
#include "Logging/Log.h"

namespace rdt {
	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	void Model::DefineCommonRect()
	{
		m_vertices.push_back({ -0.5f, -0.5f });
		m_vertices.push_back({  0.5f, -0.5f });
		m_vertices.push_back({  0.5f,  0.5f });
		m_vertices.push_back({ -0.5f,  0.5f });

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

	void Model::Reset()
	{
		m_vertices.clear();
		m_indices.clear();
	}

	// =============================================================================
	ModelManager* ModelManager::m_instance = nullptr;

	struct ModelManager::Impl {

		ModelID idGenerator = 0;
		std::unordered_map<std::string, ModelID> nameToModelID;
		std::unordered_map<ModelID, Model> m_models;

		Impl() {}
		~Impl() {}

		ModelID Register(const std::string& name)
		{
			if (nameToModelID.find(name) != nameToModelID.end()) {
				RDT_CORE_WARN("ModelManager - Could not register model '{}', already exists.", name);
				return RDT_NULL_MODEL_ID;
			}

			ModelID nID = ++idGenerator;
			nameToModelID[name] = nID;
			m_models[nID];
			return nID;
		}

		ModelID GetModelID(const std::string& name) {
			if (nameToModelID.find(name) == nameToModelID.end()) {
				RDT_CORE_WARN("ModelManager - Could not find model '{}'", name);
				return RDT_NULL_MODEL_ID;
			}

			return nameToModelID.at(name);
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
	Model& ModelManager::GetModel(ModelID model)
	{
		return m_instance->m_impl->m_models.at(model);
	}
}
