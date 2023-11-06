#pragma once
#include "Utils/MathTypes.h"

namespace rdt {

#define NOT_A_MODEL 0
	using ModelID = unsigned int;

	enum Common {
		RECT
	};

	// ==========================================================

	/*
		Models represents a renderable object, defining its
		vertices and indices.
	*/
	class Model {
	private:
		std::vector<Vec2f> m_vertices;
		std::vector<unsigned int> m_indices;

	public:
		Model();
		~Model();

		template<Common T>
		void DefineCommon() { }

		template<>
		void DefineCommon<RECT>() { DefineCommonRect(); }

	private:
		void Reset();
		void DefineCommonRect();
	};

	// ======================================================================

	class ModelManager {
	private:
		struct Impl;
		Impl* m_impl;

		ModelManager();
		~ModelManager();
		static ModelManager* m_instance;

	public:

		static void Initialize();
		static void Destroy();

		/*
			Adds a model to the ModelManager that can be accessed later using its name,
			or its return ModelID.
		*/
		static ModelID RegisterModel(const std::string& modelName, const Model& model);

		/*
			Gets the modelID for a model by its modelName
		*/
		static ModelID GetModelID(const std::string& modelName);

		/*
			Takes in a json file defining 1 or more models and registers them to the
			ModelManager.
		*/
		static void LoadModelJson(const std::string& file_json);
	};
}