#include "pch.h"
#include "Animation.h"

#include "Texture/TextureAtlas.h"
#include "Texture/TextureManager.h"
#include "Logging/Log.h"

namespace rdt {
	Animation::Animation(TextureID textureWithAtlas)
		: m_atlas{TextureManager::GetTextureAtlas(textureWithAtlas)}
	{
	}

	Animation::~Animation()
	{
	}

	void Animation::PushFrame(unsigned int atlasX, unsigned int atlasY)
	{
		m_frames.push_back(m_atlas->GetProfile(atlasX, atlasY));
	}

	void Animation::PushFrame(int spriteIndicator)
	{
		m_frames.push_back(m_atlas->GetProfile(spriteIndicator));
	}

	AtlasProfile Animation::GetFrameAt(AnimationIndex index)
	{
		if (index >= m_frames.size()) {
			return TextureManager::NOT_USING_ATLAS();
		}

		return m_frames[index];
	}

	size_t Animation::FrameCount()
	{
		return m_frames.size();
	}

	TextureID Animation::GetTextureID()
	{
		return m_atlas->GetTextureID();
	}

	// ========================================================
	struct AnimationManager::Impl {
		
		std::unordered_map<std::string, AnimationID> aliasToID;
		std::unordered_map<AnimationID, std::shared_ptr<Animation>> m_animations;
		AnimationID idCounter = 0;

		Impl()
		{
		}

		~Impl()
		{
		}

		AnimationID GenerateID() {
			return ++idCounter;
		}

		bool AnimationExists(const std::string& name) {
			return aliasToID.find(name) != aliasToID.end();
		}

		bool AnimationExists(AnimationID aID) {
			return m_animations.find(aID) != m_animations.end();
		}

		AnimationID CreateNewAnimation(const std::string& name, TextureID texture)
		{
			if (AnimationExists(name)) {
				RDT_WARN("AnimationManager - Tried to create duplicate animation '{}', original returned.", name);
				return aliasToID.at(name);
			}

			if (TextureManager::GetTextureAtlas(texture) == nullptr) {
				RDT_WARN("AnimationManager - Could not create animation '{}', no defined texture atlas for texture [{}].", name, TextureManager::GetAlias(texture));
				return RDT_NULL_ANIMATION_ID;
			}

			AnimationID nID = GenerateID();
			aliasToID[name] = nID;
			m_animations[nID] = std::make_shared<Animation>(texture);

			nID;
		}

		AnimationID GetAnimationID(const std::string& name) {
			if (!AnimationExists(name)) {
				return RDT_NULL_ANIMATION_ID;
			}

			return aliasToID.at(name);
		}

		std::shared_ptr<Animation> GetAnimation(AnimationID aID)
		{
			if (!AnimationExists(aID)) {
				return nullptr;
			}

			return m_animations.at(aID);
		}

	};
	// ========================================================
	AnimationManager::Impl* AnimationManager::m_impl = nullptr;

	void AnimationManager::Initialize()
	{
		Destroy();
		m_impl = new AnimationManager::Impl;
	}

	void AnimationManager::Destroy()
	{
		if (m_impl != nullptr) {
			delete m_impl;
			m_impl = nullptr;
		}
	}
	AnimationID AnimationManager::CreateNewAnimation(const std::string& name, TextureID texture)
	{
		return m_impl->CreateNewAnimation(name, texture);
	}

	AnimationID AnimationManager::GetAnimationID(const std::string& name)
	{
		return m_impl->GetAnimationID(name);
	}

	std::shared_ptr<Animation> AnimationManager::GetAnimation(AnimationID aID)
	{
		return m_impl->GetAnimation(aID);
	}
	TextureID AnimationManager::GetAnimationTexture(AnimationID aID)
	{
		auto animation = GetAnimation(aID);
		if (animation == nullptr) {
			return RDT_NULL_TEXTURE_ID;
		}

		return animation->GetTextureID();
	}
	AtlasProfile AnimationManager::GetFrame(AnimationID aID, AnimationIndex aIndex)
	{
		auto animation = GetAnimation(aID);
		if (animation == nullptr) {
			return TextureManager::NOT_USING_ATLAS();
		}

		return animation->GetFrameAt(aIndex);
	}
}
