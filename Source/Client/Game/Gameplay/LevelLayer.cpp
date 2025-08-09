#include "Gameplay/LevelLayer.h"

#include <Veng/LayerSystem/LayerManager.h>
#include "Core/ApplicationContext.h"
#include "Gameplay/Events.h"
#include "Gameplay/PlayerControllerLayer.h"
#include "TextSystem/TextRenderer.h"

namespace Game::Gameplay
{
	class LevelLayerHelper
	{
	public:
		void Exit(LevelLayer* self, Core::ApplicationContext& context);
	};

	static LevelLayerHelper s_Helper;

	/***** CLASS IMPL *****/

	void LevelLayer::OnStart()
	{
		auto& context = GetContext<Core::ApplicationContext>();

		m_LoopTimer = context.CurrentLevelInfo->LevelTime;
		context.CurrentLevelInfo->LevelTimeRemaining = m_LoopTimer;

		AX::EventSystem::EventManager::StartListener<LevelStartedEvent>(
			[&](LevelStartedEvent& /*event*/)
			{
				m_IsLoopStarted = true;

				context.CommonAssets.ClockSfx->Play();
			},
			p_EventListenerGroup
		);
		AX::EventSystem::EventManager::StartListener<LevelRestartEvent>(
			[&](LevelRestartEvent& /*event*/)
			{
				s_Helper.Exit(this, context);
				AX::EventSystem::EventManager::SendEvent(LevelEndedEvent());
			},
			p_EventListenerGroup
		);
		AX::EventSystem::EventManager::StartListener<LevelCompletedEvent>(
			[&](LevelCompletedEvent& /*event*/)
			{
				s_Helper.Exit(this, context);
			},
			p_EventListenerGroup
		);

		m_PlayerControllerLayer = p_LayerManager.AddLayer<PlayerControllerLayer>(Veng::LayerSystem::LayerManager::Placement::Top);
	}
	void LevelLayer::OnUpdate(float deltaTime)
	{
		auto& context = GetContext<Core::ApplicationContext>();

		auto text = std::format("{:.1f}", m_LoopTimer);
		auto textPosition = glm::vec2(14.0f - text.size(), 6.75f);
		TextSystem::TextRenderer::AddText(text, textPosition);

		if (m_IsLoopStarted == false)
		{
			return;
		}

		if (context.CurrentLevelInfo->LevelTimeRemaining < context.CurrentLevelInfo->LevelTime * 0.4f)
		{
			if (m_IsLoopLastPhase == false)
			{
				auto head = context.CommonAssets.ClockSfx->GetHeadPosition();
				context.CommonAssets.ClockSfx->Stop();
				context.CommonAssets.ClockFastSfx->Play();
				context.CommonAssets.ClockFastSfx->MoveHead(head / 2.0f);
			}
			m_IsLoopLastPhase = true;

			text = "r to restart";
			textPosition = glm::vec2(14.0f - text.size() * 0.5f, 5.25f);
			TextSystem::TextRenderer::AddText(text, textPosition, 0.5f);
		}

		m_LoopTimer -= deltaTime;
		context.CurrentLevelInfo->LevelTimeRemaining = m_LoopTimer;
		if (m_LoopTimer < 0.0f)
		{
			s_Helper.Exit(this, context);
			AX::EventSystem::EventManager::SendEvent(LevelEndedEvent());
		}
	}

	/***** HELPER IMPL *****/

	void LevelLayerHelper::Exit(LevelLayer* self, Core::ApplicationContext& context)
	{
		context.CurrentLevelInfo->LevelTimeRemaining = 0.0f;

		context.CommonAssets.ClockSfx->Stop();
		context.CommonAssets.ClockFastSfx->Stop();

		self->p_LayerManager.RemoveLayer(self->m_PlayerControllerLayer);
		self->p_LayerManager.RemoveLayer(*self);
	}
}
