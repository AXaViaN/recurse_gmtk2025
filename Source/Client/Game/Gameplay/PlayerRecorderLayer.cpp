#include "Gameplay/PlayerRecorderLayer.h"

#include "Core/ApplicationContext.h"
#include "Gameplay/Events.h"

namespace Game::Gameplay
{
	class PlayerRecorderLayerHelper
	{
	public:
		void ResetPlayerRecord(Core::ApplicationContext& context, PlayerRecord& playerRecord);
		void RecordMovement(Core::ApplicationContext& context, PlayerRecord& playerRecord, const glm::vec2& movement);
	};

	static PlayerRecorderLayerHelper s_Helper;

	/***** CLASS IMPL *****/

	void PlayerRecorderLayer::OnStart()
	{
		auto& context = GetContext<Core::ApplicationContext>();

		AX::EventSystem::EventManager::StartListener<LevelStartedEvent>(
			[&](LevelStartedEvent& /*event*/)
			{
				s_Helper.ResetPlayerRecord(context, m_PlayerRecord);
			},
			p_EventListenerGroup
		);
		AX::EventSystem::EventManager::StartListener<LevelEndedEvent>(
			[&](LevelEndedEvent& /*event*/)
			{
				AX::EventSystem::EventManager::SendEvent(PlayerRecordedEvent(m_PlayerRecord));
				s_Helper.ResetPlayerRecord(context, m_PlayerRecord);
			},
			p_EventListenerGroup
		);
		AX::EventSystem::EventManager::StartListener<PlayerMovementEvent>(
			[&](PlayerMovementEvent& event)
			{
				s_Helper.RecordMovement(context, m_PlayerRecord, event.Movement);
			},
			p_EventListenerGroup
		);
	}

	/***** HELPER IMPL *****/

	void PlayerRecorderLayerHelper::ResetPlayerRecord(Core::ApplicationContext& context, PlayerRecord& record)
	{
		record = {};
		record.CurrentStep = 0;
		record.Steps.emplace_back(PlayerRecordStep
		{
			.Time = context.CurrentLevelInfo->LevelTime,
			.Movement = glm::vec2(0.0f)
		});
	}
	void PlayerRecorderLayerHelper::RecordMovement(Core::ApplicationContext& context, PlayerRecord& playerRecord, const glm::vec2& movement)
	{
		auto shouldRecord = true;
		if (playerRecord.Steps.size() > 0)
		{
			auto& lastStep = playerRecord.Steps.back();
			shouldRecord = (lastStep.Movement != movement);
		}

		if (shouldRecord)
		{
			auto leap = std::fmod(
				context.CurrentLevelInfo->LevelTimeRemaining, Veng::LayerSystem::Layer::FixedDeltaTime
			);
			playerRecord.Steps.emplace_back(PlayerRecordStep
			{
				.Time = context.CurrentLevelInfo->LevelTimeRemaining + leap,
				.Movement = movement
			});
		}
	}
}
