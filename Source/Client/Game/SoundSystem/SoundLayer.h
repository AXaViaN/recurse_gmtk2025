#ifndef _GAME_SOUNDSYSTEM_SOUNDLAYER
#define _GAME_SOUNDSYSTEM_SOUNDLAYER

#include <Veng/LayerSystem/Layer.h>

#include <functional>
#include <vector>

namespace Game::SoundSystem
{
	class SoundLayer : public Veng::LayerSystem::Layer
	{
		VENG_ImplementLayer(SoundLayer)
	public:
		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;

	private:
		std::vector<std::function<bool(float deltaTime)>> m_Operations;
		std::vector<float> m_OperationTimers;
	};
}

#endif // !_GAME_SOUNDSYSTEM_SOUNDLAYER
