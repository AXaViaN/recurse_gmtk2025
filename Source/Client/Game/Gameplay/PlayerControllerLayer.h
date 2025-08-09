#ifndef _GAME_GAMEPLAY_PLAYERCONTROLLERLAYER
#define _GAME_GAMEPLAY_PLAYERCONTROLLERLAYER

#include <Veng/LayerSystem/Layer.h>

namespace Game::Gameplay
{
	class PlayerControllerLayer : public Veng::LayerSystem::Layer
	{
		VENG_ImplementLayer(PlayerControllerLayer)
	public:
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnFixedUpdate() override;

	private:
		glm::vec2 m_PlayerMovement = glm::vec2(0.0f);

		bool m_IsFirstMove = true;
	};
}

#endif // !_GAME_GAMEPLAY_PLAYERCONTROLLERLAYER
