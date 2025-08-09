#include "Menu/MainMenuLayer.h"

#include <Veng/LayerSystem/LayerManager.h>
#include "Core/ApplicationContext.h"
#include "Core/GameLayer.h"
#include "TextSystem/TextRenderer.h"

namespace Game::Menu
{
	void MainMenuLayer::OnStart()
	{
		auto& context = GetContext<Core::ApplicationContext>();

		context.CommonAssets.MenuMusic->Play();
		context.CommonAssets.MenuMusic->Fade(0.0f, 1.0f, 1.0f);
	}
	void MainMenuLayer::OnUpdate(float /*deltaTime*/)
	{
		auto& context = GetContext<Core::ApplicationContext>();

		auto text = std::string("recurse");
		auto textPosition = glm::vec2(-(text.size() / 2.0f) * 2.0f + 1.0f, 4.0f);
		TextSystem::TextRenderer::AddText(text, textPosition, 2.0f);

		text = std::string("press enter to play");
		textPosition = glm::vec2(-(text.size() / 2.0f) * 0.5, -2.0f);
		TextSystem::TextRenderer::AddText(text, textPosition, 0.5f);

		text = std::string("press escape to exit");
		textPosition = glm::vec2(-(text.size() / 2.0f) * 0.5, -4.0f);
		TextSystem::TextRenderer::AddText(text, textPosition, 0.5f);

		if (p_Context->Input.GetKeyDown(Veng::IO::Input::Key::Enter))
		{
			context.CommonAssets.MenuMusic->Fade(1.0f, 0.0f, 0.5f);

			p_LayerManager.AddLayer<Core::GameLayer>(Veng::LayerSystem::LayerManager::Placement::Top);
			p_LayerManager.RemoveLayer(*this);
		}
		if (p_Context->Input.GetKeyDown(Veng::IO::Input::Key::Escape))
		{
			p_Context->Window.Close();
		}
	}
}
