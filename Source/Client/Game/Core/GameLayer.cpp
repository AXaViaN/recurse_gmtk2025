#include "Core/GameLayer.h"

#include <Veng/LayerSystem/LayerManager.h>
#include "Core/ApplicationContext.h"
#include "Gameplay/Events.h"
#include "Gameplay/LevelLayer.h"
#include "Gameplay/PlayerEchoController.h"
#include "Gameplay/RewindController.h"
#include "LevelController/LevelBase.h"
#include "Menu/GameOverLayer.h"
#include "Menu/TransitionScreenLayer.h"
#include "TextSystem/TextRenderer.h"

namespace Game::Core
{
	class GameLayerHelper
	{
	public:
		void InitializeLevel(GameLayer* self, ApplicationContext& context);
		void StartLevel(GameLayer* self, ApplicationContext& context);

	public:
		Gameplay::PlayerEchoController PlayerEchoController;
		Gameplay::RewindController RewindController;

		std::vector<std::unique_ptr<LevelController::LevelBase>> Levels;
		size_t CurrentLevel = 0;
	};

	static GameLayerHelper s_Helper;

	/***** CLASS IMPL *****/

	void GameLayer::OnStart()
	{
		auto& context = GetContext<ApplicationContext>();

		m_PlayerRecorderLayer = p_LayerManager.AddLayer<Gameplay::PlayerRecorderLayer>(
			Veng::LayerSystem::LayerManager::Placement::Above, *this
		);
		AX::EventSystem::EventManager::StartListener<Gameplay::PlayerRecordedEvent>(
			[&](Gameplay::PlayerRecordedEvent& event)
			{
				s_Helper.PlayerEchoController.AddEcho(event.Record);
			},
			p_EventListenerGroup
		);

		AX::EventSystem::EventManager::StartListener<Gameplay::LevelStartedEvent>(
			[&](Gameplay::LevelStartedEvent& /*event*/)
			{
				m_IsLevelStarted = true;
			},
			p_EventListenerGroup
		);
		AX::EventSystem::EventManager::StartListener<Gameplay::LevelEndedEvent>(
			[&](Gameplay::LevelEndedEvent& /*event*/)
			{
				s_Helper.InitializeLevel(this, context);
				s_Helper.RewindController.Initialize(context, s_Helper.PlayerEchoController);
			},
			p_EventListenerGroup
		);
		AX::EventSystem::EventManager::StartListener<Gameplay::LevelCompletedEvent>(
			[&](Gameplay::LevelCompletedEvent& /*event*/)
			{
				context.CommonAssets.LevelMusic->Fade(1.0f, 0.25f, 0.5f);
				context.CommonAssets.LevelCompleteSfx->Play();

				s_Helper.PlayerEchoController.Reset();

				++s_Helper.CurrentLevel;
				if (s_Helper.CurrentLevel < s_Helper.Levels.size())
				{
					s_Helper.InitializeLevel(this, context);
				}
				else
				{
					m_IsLevelStarted = false;
					context.Scene.Reset();

					context.CommonAssets.LevelMusic->Fade(1.0f, 0.0f, 0.5f);
					p_LayerManager.AddLayer<Menu::GameOverLayer>(Veng::LayerSystem::LayerManager::Placement::Top);
				}

				p_LayerManager.AddLayer<Menu::TransitionScreenLayer>(Veng::LayerSystem::LayerManager::Placement::Top);
			},
			p_EventListenerGroup
		);
		AX::EventSystem::EventManager::StartListener<Gameplay::TransitionScreenCompletedEvent>(
			[&](Gameplay::TransitionScreenCompletedEvent& /*event*/)
			{
				if (s_Helper.CurrentLevel < s_Helper.Levels.size())
				{
					context.CommonAssets.LevelMusic->Fade(0.25f, 1.0f, 0.5f);
					s_Helper.StartLevel(this, context);
				}
				else
				{
					p_LayerManager.RemoveLayer(m_PlayerRecorderLayer);
					p_LayerManager.RemoveLayer(*this);
				}
			},
			p_EventListenerGroup
		);

		s_Helper.Levels = LevelController::LevelBase::CreateLevelList();
		s_Helper.InitializeLevel(this, context);
		s_Helper.StartLevel(this, context);

		context.CommonAssets.LevelMusic->Play();
	}
	void GameLayer::OnUpdate(float deltaTime)
	{
		auto& context = GetContext<ApplicationContext>();

		static auto isTutorial = true;
		if (isTutorial)
		{
			TextSystem::TextRenderer::AddText("W", { 9.5f, 0.5f }, 0.5f);
			TextSystem::TextRenderer::AddText("A S D", { 8.5f, -0.5f }, 0.5f);
		}

		if (s_Helper.RewindController.IsFinished() == false)
		{
			s_Helper.RewindController.Update(context, deltaTime);
			s_Helper.Levels[s_Helper.CurrentLevel]->Update(context, deltaTime, s_Helper.PlayerEchoController.GetEchoes());
			if (s_Helper.RewindController.IsFinished())
			{
				s_Helper.InitializeLevel(this, context);
				s_Helper.StartLevel(this, context);
			}
		}

		if (m_IsLevelStarted == false)
		{
			return;
		}

		isTutorial = false;

		s_Helper.Levels[s_Helper.CurrentLevel]->Update(context, deltaTime, s_Helper.PlayerEchoController.GetEchoes());
	}
	void GameLayer::OnFixedUpdate()
	{
		auto& context = GetContext<ApplicationContext>();

		if (m_IsLevelStarted == false)
		{
			return;
		}

		s_Helper.PlayerEchoController.Update(context, FixedDeltaTime);
		s_Helper.Levels[s_Helper.CurrentLevel]->FixedUpdate(context, s_Helper.PlayerEchoController.GetEchoes());
	}

	/***** HELPER IMPL *****/

	void GameLayerHelper::InitializeLevel(GameLayer* self, ApplicationContext& context)
	{
		self->m_IsLevelStarted = false;
		context.Scene.Reset();

		PlayerEchoController.Initialize(context);

		Levels[s_Helper.CurrentLevel]->Initialize(context);
		context.CurrentLevelInfo->SpawnMap();
	}
	void GameLayerHelper::StartLevel(GameLayer* self, ApplicationContext& /*context*/)
	{
		self->p_LayerManager.AddLayer<Gameplay::LevelLayer>(Veng::LayerSystem::LayerManager::Placement::Top);
	}
}
