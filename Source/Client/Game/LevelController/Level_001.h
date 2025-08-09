#ifndef _GAME_LEVELCONTROLLER_LEVEL001
#define _GAME_LEVELCONTROLLER_LEVEL001

#include "LevelController/LevelBase.h"
#include "Gameplay/ToggleObject.h"

namespace Game::LevelController
{
	class Level_001 : public LevelBase
	{
	public:
		virtual void Initialize(Core::ApplicationContext& context) override;

		virtual void Update(Core::ApplicationContext& context, float deltaTime, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects) override;

	private:
		std::optional<Gameplay::ToggleObject> m_Switch;
	};
}

#endif // !_GAME_LEVELCONTROLLER_LEVEL001
