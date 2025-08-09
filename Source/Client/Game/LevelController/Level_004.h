#ifndef _GAME_LEVELCONTROLLER_LEVEL004
#define _GAME_LEVELCONTROLLER_LEVEL004

#include "LevelController/LevelBase.h"
#include "Gameplay/ToggleObject.h"
#include "Gameplay/PushObject.h"

namespace Game::LevelController
{
	class Level_004 : public LevelBase
	{
	public:
		virtual void Initialize(Core::ApplicationContext& context) override;

		virtual void Update(Core::ApplicationContext& context, float deltaTime, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects) override;
		virtual void FixedUpdate(Core::ApplicationContext& context, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects) override;

	private:
		std::optional<Gameplay::ToggleObject> m_RedButton;
		std::optional<Gameplay::PushObject> m_RedFlask;

		std::optional<Gameplay::ToggleObject> m_BlueButton;
		std::optional<Gameplay::PushObject> m_BlueFlask;
	};
}

#endif // !_GAME_LEVELCONTROLLER_LEVEL004
