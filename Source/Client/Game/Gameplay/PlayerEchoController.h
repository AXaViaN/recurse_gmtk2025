#ifndef _GAME_GAMEPLAY_PLAYERECHOCONTROLLER
#define _GAME_GAMEPLAY_PLAYERECHOCONTROLLER

#include "Core/ApplicationContext.h"
#include "Gameplay/PlayerRecord.h"
#include "SceneSystem/Object.h"

namespace Game::Gameplay
{
	class PlayerEchoController
	{
	public:
		void Reset();

		void AddEcho(const Gameplay::PlayerRecord& record);
		inline auto GetEchoes() -> const std::vector<AX::ID<SceneSystem::Object>>&;

		void Initialize(Core::ApplicationContext& context);

		void Update(Core::ApplicationContext& context, float deltaTime);

		void RewindInitialize(Core::ApplicationContext& context);
		void RewindFinalize(Core::ApplicationContext& context);
		void RewindUpdate(Core::ApplicationContext& context, float timeRemaining, float deltaTime);

	private:
		std::vector<AX::ID<SceneSystem::Object>> m_EchoObjects;
		std::vector<Gameplay::PlayerRecord> m_EchoRecords;

		std::array<bool, 3> m_IsEchoMovings;
	};

	/***** IMPL *****/

	inline auto PlayerEchoController::GetEchoes() -> const std::vector<AX::ID<SceneSystem::Object>>&
	{
		return m_EchoObjects;
	}
}

#endif // !_GAME_GAMEPLAY_PLAYERECHOCONTROLLER
