#ifndef _GAME_MENU_TRANSITIONSCREENLAYER
#define _GAME_MENU_TRANSITIONSCREENLAYER

#include <Veng/Asset/Texture.h>
#include <Veng/LayerSystem/Layer.h>

namespace Game::Menu
{
	class TransitionScreenLayer : public Veng::LayerSystem::Layer
	{
		VENG_ImplementLayer(TransitionScreenLayer)
	public:
		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;

	private:
		Veng::Asset::AssetMeta<Veng::Asset::Texture> m_Texture;

		float m_Time = 1.25f;
		float m_Timer = m_Time;
	};
}

#endif // !_GAME_MENU_TRANSITIONSCREENLAYER
