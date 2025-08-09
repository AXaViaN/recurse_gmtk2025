#ifndef _GAME_LEVELCONTROLLER_LEVEL002
#define _GAME_LEVELCONTROLLER_LEVEL002

#include "LevelController/LevelBase.h"
#include "Gameplay/ToggleObject.h"

namespace Game::LevelController
{
	class Level_002 : public LevelBase
	{
	public:
		virtual void Initialize(Core::ApplicationContext& context) override;

		virtual void Update(Core::ApplicationContext& context, float deltaTime, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects) override;

	private:
		std::optional<Gameplay::ToggleObject> m_Switch1;
		std::optional<Gameplay::ToggleObject> m_Switch2;
	};
}

#endif // !_GAME_LEVELCONTROLLER_LEVEL002
