#include "Menu/TransitionScreenLayer.h"

#include <Veng/Gfx/Renderer.h>
#include <Veng/LayerSystem/LayerManager.h>
#include "Core/ApplicationContext.h"
#include "Gameplay/Events.h"

namespace Game::Menu
{
	void TransitionScreenLayer::OnStart()
	{
		auto& context = GetContext<Core::ApplicationContext>();

		m_Texture = context.CommonAssets.TransitionScreenMaterialAsset->GetProperty("Texture").Value.AsTexture();

		auto overlayID = context.Scene.CreateScreenOverlayObject();
		auto overlay = context.Scene.GetObject(overlayID);
		overlay->Position = glm::vec2(0.0f);
		overlay->Scale = glm::vec2(10.0f);
		overlay->MaterialAsset = context.CommonAssets.TransitionScreenMaterialAsset;
		overlay->SimpleAlphaBlending = true;
	}
	void TransitionScreenLayer::OnUpdate(float deltaTime)
	{
		auto alpha = m_Timer / m_Time;
		m_Texture->SetPixel({ 0, 0 }, glm::vec4(0.0f, 0.0f, 0.0f, alpha));
		m_Texture->UpdatePixels();

		m_Timer -= deltaTime;
		if (m_Timer < 0)
		{
			m_Texture->SetPixel({ 0, 0 }, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
			m_Texture->UpdatePixels();

			AX::EventSystem::EventManager::SendEvent(Gameplay::TransitionScreenCompletedEvent());
			p_LayerManager.RemoveLayer(*this);
		}
	}
}
