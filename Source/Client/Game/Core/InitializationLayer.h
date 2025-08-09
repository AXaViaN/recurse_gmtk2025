#ifndef _GAME_CORE_INITIALIZATIONLAYER
#define _GAME_CORE_INITIALIZATIONLAYER

#include <Veng/LayerSystem/Layer.h>

namespace Game::Core
{
	class InitializationLayer : public Veng::LayerSystem::Layer
	{
		VENG_ImplementLayer(InitializationLayer)
	public:
		virtual void OnStart() override;
	};
}

#endif // !_GAME_CORE_INITIALIZATIONLAYER
