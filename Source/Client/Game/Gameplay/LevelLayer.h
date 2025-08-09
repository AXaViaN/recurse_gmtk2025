#ifndef _GAME_GAMEPLAY_LEVELLAYER
#define _GAME_GAMEPLAY_LEVELLAYER

#include <Veng/LayerSystem/Layer.h>

namespace Game::Gameplay
{
	class PlayerControllerLayer;

	class LevelLayer : public Veng::LayerSystem::Layer
	{
		VENG_ImplementLayer(LevelLayer)
		friend class LevelLayerHelper;
	public:
		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;

	private:
		float m_LoopTimer = 10.0f;
		bool m_IsLoopStarted = false;
		bool m_IsLoopLastPhase = false;

		std::shared_ptr<PlayerControllerLayer> m_PlayerControllerLayer;
	};
}

#endif // !_GAME_GAMEPLAY_LEVELLAYER
