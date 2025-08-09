#include <Veng/Gfx/CommandBuffer.h>

#include <AX/EventSystem/EventManager.h>
#include <Veng/Asset/Shader.h>
#include <Veng/Gfx/Context.h>
#include <Veng/Gfx/Framebuffer.h>
#include <Veng/Gfx/Renderer.h>
#include <Veng/IO/Window.h>

#include <glax/gl.h>

namespace Veng::Gfx
{
	class CommandBufferHelper
	{
	public:
		CommandBufferHelper();

	public:
		glm::ivec2 WindowSize = {};
	};

	static CommandBufferHelper s_Helper;

	/***** CLASS IMPL *****/

	void CommandBuffer::Reset()
	{
		m_Commands.clear();
	}

	void CommandBuffer::Clear()
	{
		m_Commands.emplace_back([]()
		{
			glClear(GL_COLOR_BUFFER_BIT);
		});
	}
	void CommandBuffer::Clear(const glm::vec3& color)
	{
		m_Commands.emplace_back([color]()
		{
			glClearColor(color.x, color.y, color.z, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		});
	}

	void CommandBuffer::UseFramebuffer(const Framebuffer& framebuffer)
	{
		m_Commands.emplace_back([&framebuffer]()
		{
			framebuffer.Use();
			Renderer::SetViewport({ 0, 0 }, framebuffer.GetSize());
		});
	}
	void CommandBuffer::UseDefaultFramebuffer()
	{
		m_Commands.emplace_back([]()
		{
			Framebuffer().Use();
			Renderer::SetViewport({ 0, 0 }, s_Helper.WindowSize);
		});
	}

	void CommandBuffer::EnableSimpleAlphaBlending()
	{
		m_Commands.emplace_back([]()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		});
	}
	void CommandBuffer::EnableAdditiveAlphaBlending()
	{
		m_Commands.emplace_back([]()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
		});
	}
	void CommandBuffer::DisableAlphaBlending()
	{
		m_Commands.emplace_back([]()
		{
			glDisable(GL_BLEND);
		});
	}

	void CommandBuffer::Draw(const Context& context)
	{
		m_Commands.emplace_back([context]()
		{
			auto mvp = (
				context.ProjectionMatrix *
				context.ViewMatrix *
				context.ModelMatrix
			);

			auto mesh = *(context.MeshAsset);
			auto material = *(context.MaterialAsset);
			auto shader = *(material->GetShaderAsset());

			material->Use();
			shader->SetUniform("MVP", mvp);

			mesh->Use();
			glDrawElements(mesh->GetDrawMode(), mesh->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
		});
	}

	/***** HELPER IMPL *****/

	CommandBufferHelper::CommandBufferHelper()
	{
		AX::EventSystem::EventManager::StartListener<IO::Window::WindowResizeEvent>(
			[&](IO::Window::WindowResizeEvent& event)
			{
				WindowSize = { event.Width, event.Height };
			}
		);
	}
}
