#include "SoundSystem/SoundLayer.h"

#include "SoundSystem/Events.h"

namespace Game::SoundSystem
{
	class SoundLayerHelper
	{
	public:
		static bool OnFadeEvent(FadeEvent event, float& timer, float deltaTime);
	};

	static SoundLayerHelper s_Helper;

	/***** CLASS IMPL *****/

	void SoundLayer::OnStart()
	{
		AX::EventSystem::EventManager::StartListener<FadeEvent>(
			[&](FadeEvent& event)
			{
				auto& timer = m_OperationTimers.emplace_back(0.0f);
				m_Operations.emplace_back(
					std::bind(&SoundLayerHelper::OnFadeEvent, event, timer, std::placeholders::_1)
				);

				event.Consume();
			},
			p_EventListenerGroup
		);
	}
	void SoundLayer::OnUpdate(float deltaTime)
	{
		auto finishedOperations = std::vector<size_t>();
		for (auto operationIdx = 0ull; operationIdx < m_Operations.size(); ++operationIdx)
		{
			auto& operation = m_Operations[operationIdx];

			auto isRunning = operation(deltaTime);
			if (isRunning == false)
			{
				finishedOperations.emplace_back(operationIdx);
			}
		}

		for (auto i = 0ull; i < finishedOperations.size(); ++i)
		{
			auto operationIdx = finishedOperations[i] - i;
			m_Operations.erase(m_Operations.begin() + operationIdx);
			m_OperationTimers.erase(m_OperationTimers.begin() + operationIdx);
		}
	}

	/***** HELPER IMPL *****/

	bool SoundLayerHelper::OnFadeEvent(FadeEvent event, float& timer, float deltaTime)
	{
		timer += deltaTime;
		if (timer >= event.Time)
		{
			event.Sound->SetVolume(event.EndVolume);
			if (event.EndVolume == 0.0f)
			{
				event.Sound->Stop();
			}

			return false;
		}

		auto t = timer / event.Time;
		auto volume = glm::mix(event.StartVolume, event.EndVolume, t);
		event.Sound->SetVolume(volume);

		return true;
	}
}
