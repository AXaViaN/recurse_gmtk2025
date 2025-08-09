#ifndef _GAME_GAMEPLAY_PLAYERRECORDERLAYER
#define _GAME_GAMEPLAY_PLAYERRECORDERLAYER

#include <Veng/LayerSystem/Layer.h>
#include "Gameplay/PlayerRecord.h"

namespace Game::Gameplay
{
	class PlayerRecorderLayer : public Veng::LayerSystem::Layer
	{
		VENG_ImplementLayer(PlayerRecorderLayer)
	public:
		virtual void OnStart() override;

	private:
		PlayerRecord m_PlayerRecord;
	};
}

#endif // !_GAME_GAMEPLAY_PLAYERRECORDERLAYER
