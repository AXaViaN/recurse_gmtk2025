#ifndef _GAME_MENU_MAINMENULAYER
#define _GAME_MENU_MAINMENULAYER

#include <Veng/LayerSystem/Layer.h>

namespace Game::Menu
{
	class MainMenuLayer : public Veng::LayerSystem::Layer
	{
		VENG_ImplementLayer(MainMenuLayer)
	public:
		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;
	};
}

#endif // !_GAME_MENU_MAINMENULAYER
