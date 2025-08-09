#include "Core/RenderLayer.h"

#include <Veng/Gfx/Renderer.h>
#include "Core/ApplicationContext.h"
#include "TextSystem/TextRenderer.h"

using namespace Veng::Gfx;

namespace Game::Core
{
	class RenderLayerHelper
	{
	public:
		void RenderSceneObjects(Context& renderContext, CommandBuffer& commandBuffer, const std::vector<SceneSystem::Object>& sceneObjects);
	};

	static RenderLayerHelper s_Helper;

	/***** CLASS IMPL *****/

	void RenderLayer::OnStart()
	{
		static auto setProjectionMatrix = [&]()
		{
			auto windowAspectRatio = p_Context->Window.GetAspectRatio();
			auto height = 2.0f;
			auto width = height * windowAspectRatio;

			m_ProjectionMatrix = glm::ortho(-width / 2, width / 2, -height / 2, height / 2);
			TextSystem::TextRenderer::SetProjectionMatrix(m_ProjectionMatrix);
		};

		setProjectionMatrix();

		AX::EventSystem::EventManager::StartListener<Veng::IO::Window::WindowResizeEvent>(
			[&](Veng::IO::Window::WindowResizeEvent& event)
			{
				Renderer::SetViewport({ 0, 0 }, { event.Width, event.Height });
				setProjectionMatrix();
			}
		);
	}
	void RenderLayer::OnRender()
	{
		auto& context = GetContext<ApplicationContext>();

		auto renderContext = Renderer::CreateContext();
		auto commandBuffer = Renderer::CreateCommandBuffer();
		
		renderContext.ProjectionMatrix = m_ProjectionMatrix;
		renderContext.ViewMatrix = glm::mat4(1.0f);
		renderContext.MeshAsset = context.CommonAssets.QuadMeshAsset;

		commandBuffer.Clear({ 0.2f, 0.2f, 0.2f });

		s_Helper.RenderSceneObjects(renderContext, commandBuffer, context.Scene.GetBackgroundObjects());
		s_Helper.RenderSceneObjects(renderContext, commandBuffer, context.Scene.GetForegroundObjects());
		s_Helper.RenderSceneObjects(renderContext, commandBuffer, context.Scene.GetDynamicObjects());
		s_Helper.RenderSceneObjects(renderContext, commandBuffer, context.Scene.GetOverlayObjects());

		Renderer::Render(commandBuffer);

		TextSystem::TextRenderer::RenderAll();

		commandBuffer.Reset();
		s_Helper.RenderSceneObjects(renderContext, commandBuffer, context.Scene.GetScreenOverlayObjects());
		Renderer::Render(commandBuffer);
	}

	/***** HELPER IMPL *****/

	void RenderLayerHelper::RenderSceneObjects(Context& renderContext, CommandBuffer& commandBuffer, const std::vector<SceneSystem::Object>& sceneObjects)
	{
		for (auto& sceneObject : sceneObjects)
		{
			renderContext.ModelMatrix = glm::translate(
				glm::mat4(1.0f), glm::vec3(sceneObject.Position, 0.0f)
			);
			renderContext.ModelMatrix = glm::scale(
				renderContext.ModelMatrix, glm::vec3(sceneObject.Scale, 1.0f)
			);

			renderContext.MaterialAsset = sceneObject.MaterialAsset;

			if (sceneObject.SimpleAlphaBlending)
			{
				commandBuffer.EnableSimpleAlphaBlending();
			}
			else if (sceneObject.AdditiveAlphaBlending)
			{
				commandBuffer.EnableAdditiveAlphaBlending();
			}

			commandBuffer.Draw(renderContext);

			if (sceneObject.SimpleAlphaBlending || sceneObject.AdditiveAlphaBlending)
			{
				commandBuffer.DisableAlphaBlending();
			}
		}
	}
}
