#ifndef _GAME_MENU_GAMEOVERLAYER
#define _GAME_MENU_GAMEOVERLAYER

#include <Veng/LayerSystem/Layer.h>

namespace Game::Menu
{
	class GameOverLayer : public Veng::LayerSystem::Layer
	{
		VENG_ImplementLayer(GameOverLayer)
	public:
		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;
	};
}

#endif // !_GAME_MENU_GAMEOVERLAYER
