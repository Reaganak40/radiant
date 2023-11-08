/*******************************************************************
*	Module:  Graphics
*	File:    Model.h
*
*	Author: Reagan Kelley
*
*   Models describe the features of an entity, its geometry before
*	any transformations. Models are accesses and maintained by the
*	ModelManager.
*******************************************************************/
#pragma once
#include "Utils/MathTypes.h"

// Forward Declarations
namespace rdt {
	struct Transform;
}

namespace rdt {

#define RDT_NULL_MODEL_ID 0
	using ModelID = unsigned int;

	enum ModelShape {
		MS_RECT
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

		template<ModelShape T>
		void AddShape() { }

		template<>
		void AddShape<MS_RECT>() { DefineCommonRect(); }

		std::vector<Vec2f>& GetVertices();
		std::vector<unsigned int>& GetIndices();

		/*
			Takes a transform, and an out reference, and returns the model
			vertices according to that transform.
		*/
		void ApplyTransform(const Transform& transform, std::vector<Vec2f>& vertices);

	private:
		void Reset();
		void DefineCommonRect();
	};

	// ======================================================================

	class RADIANT_API ModelManager {
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
		static ModelID RegisterModel(const std::string& modelName);

		/*
			Gets the modelID for a model by its modelName
		*/
		static ModelID GetModelID(const std::string& modelName);


		/*
			Returns true if a model is registered with the given modelID
		*/
		static bool ModelExists(ModelID model);

		/*
			Takes in a json file defining 1 or more models and registers them to the
			ModelManager.
		*/
		static void LoadModelJson(const std::string& file_json);

		/*
			Gets the specified model and copies its vertex data to the out reference
		*/
		static std::vector<Vec2f>& GetVertices(ModelID model);

		/*
			Gets the specified model and copies its index data to the out reference
		*/
		static std::vector<unsigned int>& GetIndices(ModelID model);

		/*
			Takes a transform, and an out reference, and returns the model
			vertices according to that transform.
		*/
		static void ApplyTransform(ModelID model, const Transform& transform, std::vector<Vec2f>& vertices);

		friend class ResourceManager;
	private:

		/*
			Returns a reference to the model specified by the modelID
		*/
		static Model& GetModel(ModelID model);
	};
}