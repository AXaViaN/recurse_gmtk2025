#ifndef _GAME_GAMEPLAY_TOGGLEOBJECT
#define _GAME_GAMEPLAY_TOGGLEOBJECT

#include <AX/ID.h>
#include "Core/ApplicationContext.h"

namespace Game::Gameplay
{
	class ToggleObject
	{
	public:
		ToggleObject(
			Core::ApplicationContext& context,
			const std::string& sprite, const std::string& toggledSprite,
			const Veng::Asset::AssetMeta<SoundSystem::Sound>& sfx,
			const Veng::Asset::AssetMeta<SoundSystem::Sound>& toggledSfx,
			const glm::vec2& spawnPosition
		);

		void SetToggled(bool isToggled);

		auto IsColliding(const AX::ID<SceneSystem::Object>& otherObjectID) -> bool;

	private:
		Core::ApplicationContext& m_Context;

		AX::ID<SceneSystem::Object> m_ObjectID;

		std::string m_Sprite;
		std::string m_ToggledSprite;
		Veng::Asset::AssetMeta<SoundSystem::Sound> m_Sfx;
		Veng::Asset::AssetMeta<SoundSystem::Sound> m_ToggledSfx;

		bool m_IsToggled = false;
	};
}

#endif // !_GAME_GAMEPLAY_TOGGLEOBJECT
