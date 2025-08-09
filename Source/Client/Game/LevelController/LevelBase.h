#ifndef _GAME_LEVELCONTROLLER_LEVELBASE
#define _GAME_LEVELCONTROLLER_LEVELBASE

#include "Core/ApplicationContext.h"

namespace Game::LevelController
{
	class LevelBase
	{
	public:
		LevelBase() = default;

		virtual void Initialize(Core::ApplicationContext& context) = 0;
		virtual void Update(Core::ApplicationContext& context, float deltaTime, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects) = 0;
		inline virtual void FixedUpdate(Core::ApplicationContext& context, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects);

		static auto CreateLevelList() -> std::vector<std::unique_ptr<LevelController::LevelBase>>;

	public:
		virtual ~LevelBase() noexcept = default;
		AX_SemanticsDeleteAll(LevelBase)

	protected:
		void SpawnPlayer(Core::ApplicationContext& context);
		void SetDoorState(Core::ApplicationContext& context, bool isOpen);

	private:
		bool m_IsDoorOpen = false;
	};

	/***** IMPL *****/

	inline void LevelBase::FixedUpdate(Core::ApplicationContext& /*context*/, const std::vector<AX::ID<SceneSystem::Object>>& /*echoObjects*/)
	{
	}
}

#endif // !_GAME_LEVELCONTROLLER_LEVELBASE
