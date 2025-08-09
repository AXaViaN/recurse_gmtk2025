#ifndef _GAME_GAMEPLAY_REWINDCONTROLLER
#define _GAME_GAMEPLAY_REWINDCONTROLLER

#include "Core/ApplicationContext.h"
#include "Gameplay/PlayerEchoController.h"

namespace Game::Gameplay
{
	class RewindController
	{
	public:
		void Initialize(Core::ApplicationContext& context, PlayerEchoController& playerEchoController);

		void Update(Core::ApplicationContext& context, float deltaTime);

		inline auto IsFinished() const -> bool;

	private:
		PlayerEchoController* m_EchoController;

		float m_Time = 1.0f;
		float m_Timer = 0.0f;
	};

	/***** IMPL *****/

	inline auto RewindController::IsFinished() const -> bool
	{
		return (m_Timer == 0.0f);
	}
}

#endif // !_GAME_GAMEPLAY_REWINDCONTROLLER
