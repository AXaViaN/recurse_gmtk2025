#ifndef _GAME_CORE_RENDERLAYER
#define _GAME_CORE_RENDERLAYER

#include <Veng/LayerSystem/Layer.h>

namespace Game::Core
{
	class RenderLayer : public Veng::LayerSystem::Layer
	{
		VENG_ImplementLayer(RenderLayer)
	public:
		virtual void OnStart() override;
		virtual void OnRender() override;

	private:
		glm::mat4 m_ProjectionMatrix;
	};
}

#endif // !_GAME_CORE_RENDERLAYER
