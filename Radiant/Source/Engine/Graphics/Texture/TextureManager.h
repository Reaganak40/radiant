#pragma once
#include "Core.h"
#include "Texture.h"
#include "../Mesh.h"

namespace rdt::core {
	class RendererGL;
	class RenderLayer;
}

namespace rdt {

	class RADIANT_API TextureManager {
	private:
		struct Impl;
		Impl* m_impl;
		
		TextureManager();
		~TextureManager();
		static TextureManager* m_instance;

	public:
		/*
			Creates a new instance of the TextureManager.
		*/
		static void Initialize();

		/*
			Destroys the current instance of the TextureManger.
		*/
		static void Destroy();

		/*
			Loads a texture from a png file. Returns the texture reference for
			more specifications or calling it in the renderer API.
		*/
		static Texture& LoadTextureFromPNG(const std::string& name, const std::string& filepath) {
			return m_instance->LoadTextureFromPNGImpl(name, filepath);
		}

		static Texture* GetTexture(const std::string& name) { return m_instance->GetTextureImpl(name); }

		friend class core::RendererGL;
		friend class core::RenderLayer;

	private:
		Texture& LoadTextureFromPNGImpl(const std::string& name, const std::string& filepath);
		Texture* GetTextureImpl(const std::string& name);
		
		/*
			Applies the provided texture to the vertices of the provided rectangle. If the texture
			has a texture atlas, the atlasCoords will be applied to get the correct sprite in the
			tile map.
			Returns true if the texture slots changed.
		*/
		static bool ApplyTextureAtlas(Texture* texture, const Vec2i& atlasCoords, std::vector<core::Vertex>& rectVertices, bool flipHorizontal = false);
		static std::array<unsigned int, MAX_TEXTURES>& GetTextureSlots();

		void AddNoneTexture();

		TextureSlot GetNextSlot();

	};
}