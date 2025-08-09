#include "Gameplay/ToggleObject.h"

namespace Game::Gameplay
{
	ToggleObject::ToggleObject(
		Core::ApplicationContext& context,
		const std::string& sprite, const std::string& toggledSprite,
		const Veng::Asset::AssetMeta<SoundSystem::Sound>& sfx,
		const Veng::Asset::AssetMeta<SoundSystem::Sound>& toggledSfx,
		const glm::vec2& spawnPosition
	) :
		m_Context(context),
		m_ObjectID(context.Scene.CreateForegroundObject()),
		m_Sprite(sprite),
		m_ToggledSprite(toggledSprite),
		m_Sfx(sfx),
		m_ToggledSfx(toggledSfx)
	{
		auto toggleObject = m_Context.Scene.GetObject(m_ObjectID);
		toggleObject->Scale = glm::vec2(m_Context.CurrentLevelInfo->TileSize);
		toggleObject->Position = m_Context.CurrentLevelInfo->GetTilePositionInScene(spawnPosition);
		toggleObject->MaterialAsset = m_Context.CommonAssets.DungeonTileAtlas->GetSpriteMaterial(sprite);
	}

	void ToggleObject::SetToggled(bool isToggled)
	{
		if (m_IsToggled == isToggled)
		{
			return;
		}

		m_IsToggled = isToggled;
		auto toggleObject = m_Context.Scene.GetObject(m_ObjectID);
		toggleObject->MaterialAsset = m_Context.CommonAssets.DungeonTileAtlas->GetSpriteMaterial(
			m_IsToggled ? m_ToggledSprite : m_Sprite
		);

		auto sfx = m_Sfx;
		if (m_IsToggled)
		{
			sfx = m_ToggledSfx;
		}
		sfx->Play();
	}

	auto ToggleObject::IsColliding(const AX::ID<SceneSystem::Object>& otherObjectID) -> bool
	{
		auto toggleObject = m_Context.Scene.GetObject(m_ObjectID);
		auto otherObject = m_Context.Scene.GetObject(otherObjectID);

		auto distance = glm::distance(toggleObject->Position, otherObject->Position);
		auto minDistance = (0.75f * m_Context.CurrentLevelInfo->TileSize);
		return (distance < minDistance);
	}
}
