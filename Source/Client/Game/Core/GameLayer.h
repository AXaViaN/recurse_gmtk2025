#ifndef _GAME_CORE_GAMELAYER
#define _GAME_CORE_GAMELAYER

#include <Veng/LayerSystem/Layer.h>
#include "Gameplay/PlayerRecorderLayer.h"

namespace Game::Core
{
	class GameLayer : public Veng::LayerSystem::Layer
	{
		VENG_ImplementLayer(GameLayer)
		friend class GameLayerHelper;
	public:
		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnFixedUpdate() override;

	private:
		std::shared_ptr<Gameplay::PlayerRecorderLayer> m_PlayerRecorderLayer;

		bool m_IsLevelStarted = false;
	};
}

#endif // !_GAME_CORE_GAMELAYER
