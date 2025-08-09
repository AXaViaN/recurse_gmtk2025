#ifndef _GAME_LEVELCONTROLLER_LEVEL003
#define _GAME_LEVELCONTROLLER_LEVEL003

#include "LevelController/LevelBase.h"
#include "Gameplay/ToggleObject.h"
#include "Gameplay/PushObject.h"

namespace Game::LevelController
{
	class Level_003 : public LevelBase
	{
	public:
		virtual void Initialize(Core::ApplicationContext& context) override;

		virtual void Update(Core::ApplicationContext& context, float deltaTime, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects) override;
		virtual void FixedUpdate(Core::ApplicationContext& context, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects) override;

	private:
		std::optional<Gameplay::ToggleObject> m_RedButton;
		std::optional<Gameplay::PushObject> m_RedFlask;
	};
}

#endif // !_GAME_LEVELCONTROLLER_LEVEL003
