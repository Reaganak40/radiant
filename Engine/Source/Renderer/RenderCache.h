#pragma once
#include "Mesh.h"
#include "Utils/MathTypes.h"
#include "Utils/UniqueID.h"

#include "Polygon/Polygon.h"
#include "Polygon/Rect.h"
#include "Polygon/Line.h"


namespace Radiant {
	class RenderCache {
	private:
		std::unordered_map<UniqueID, Mesh> m_mesh_cache;
		std::vector<std::shared_ptr<Rect>> m_rect_cache;
		std::vector<std::shared_ptr<Line>> m_line_cache;

		unsigned int m_rect_index;
		unsigned int m_line_index;

	public:
		RenderCache();
		~RenderCache();

		/*
			Gets the mesh marked by this UniqueID, or creates a new Mesh if it was not found.
		*/
		Mesh& GetMesh(UniqueID meshIdentifier);
		
		/*
			Gets an available rect for rendering purposes.
		*/
		std::shared_ptr<Rect> GetFreeRect();

		/*
			Adds a rect to the cache.
		*/
		void AddRectToCache(std::shared_ptr<Rect> rect);

		/*
			Gets an available Line for rendering purposes.
		*/
		std::shared_ptr<Line> GetFreeLine();

		/*
			Adds a line to the cache.
		*/
		void AddLineToCache(std::shared_ptr<Line> line);

		/*
			Removes the mesh from the cache that has this identifier.
		*/
		void Flush(const UniqueID meshIdentifier);

		/*
			Prepares cache for next frame.
		*/
		void OnEndFrame();
	};
}