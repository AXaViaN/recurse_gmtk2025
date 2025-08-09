#include "Gameplay/RewindController.h"

#include "TextSystem/TextRenderer.h"

namespace Game::Gameplay
{
	void RewindController::Initialize(Core::ApplicationContext& context, PlayerEchoController& playerEchoController)
	{
		m_EchoController = &playerEchoController;
		m_Timer = m_Time;

		m_EchoController->RewindInitialize(context);

		context.CommonAssets.RewindSfx->Play();
	}

	void RewindController::Update(Core::ApplicationContext& context, float deltaTime)
	{
		auto alpha = m_Timer / m_Time;
		auto timeRemaining = context.CurrentLevelInfo->LevelTime * (1.0f - alpha);

		m_Timer -= deltaTime;
		auto nextAlpha = m_Timer / m_Time;
		auto nextTimeRemaining = context.CurrentLevelInfo->LevelTime * (1.0f - nextAlpha);

		auto delta = nextTimeRemaining - timeRemaining;
		m_EchoController->RewindUpdate(context, timeRemaining, delta);

		if (m_Timer < 0)
		{
			m_EchoController->RewindFinalize(context);
			m_Timer = 0.0f;
		}

		auto text = std::format("{:.1f}", timeRemaining);
		auto textPosition = glm::vec2(-12.5f, 6.75f);
		TextSystem::TextRenderer::AddText(text, textPosition);
	}
}
