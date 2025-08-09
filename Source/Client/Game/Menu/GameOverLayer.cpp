#include "Menu/GameOverLayer.h"

#include "Core/ApplicationContext.h"
#include "TextSystem/TextRenderer.h"

namespace Game::Menu
{
	void GameOverLayer::OnStart()
	{
		auto& context = GetContext<Core::ApplicationContext>();

		context.CommonAssets.MenuMusic->Play();
		context.CommonAssets.MenuMusic->Fade(0.0f, 1.0f, 0.5f);
	}
	void GameOverLayer::OnUpdate(float /*deltaTime*/)
	{
		auto text = std::string("thanks for playing!");
		auto textPosition = glm::vec2(-(text.size() / 2.0f) + 1.0f, 4.0f);
		TextSystem::TextRenderer::AddText(text, textPosition);

		text = std::string("made by AXaViaN");
		textPosition = glm::vec2(-(text.size() / 2.0f) * 0.5, 0.0f);
		TextSystem::TextRenderer::AddText(text, textPosition, 0.5f);

		text = std::string("sprites: 0x72");
		textPosition = glm::vec2(-(text.size() / 2.0f) * 0.5, -2.0f);
		TextSystem::TextRenderer::AddText(text, textPosition, 0.5f);

		text = std::string("font: frostyfreeze");
		textPosition = glm::vec2(-(text.size() / 2.0f) * 0.5, -3.0f);
		TextSystem::TextRenderer::AddText(text, textPosition, 0.5f);

		text = std::string("press escape to exit");
		textPosition = glm::vec2(-(text.size() / 2.0f) * 0.5, -6.0f);
		TextSystem::TextRenderer::AddText(text, textPosition, 0.5f);

		if (p_Context->Input.GetKeyDown(Veng::IO::Input::Key::Escape))
		{
			p_Context->Window.Close();
		}
	}
}
