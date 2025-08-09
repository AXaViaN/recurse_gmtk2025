#include <Veng/Gfx/Renderer.h>

#include <glax/gl.h>

namespace Veng::Gfx
{
	void Renderer::Init()
	{
		glEnable(GL_CULL_FACE);
		
		glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
	}
	void Renderer::SetViewport(glm::ivec2 position, glm::ivec2 size)
	{
		glViewport(position.x, position.y, size.x, size.y);
	}

	void Renderer::SetWireframeMode(bool isWireframeMode)
	{
		auto mode = GL_FILL;
		if (isWireframeMode)
		{
			mode = GL_LINE;
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
		}

		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}
	void Renderer::SetWireframeWidth(float width)
	{
		glLineWidth(width);
	}

	auto Renderer::CreateContext() -> Context
	{
		return Context();
	}
	auto Renderer::CreateCommandBuffer() -> CommandBuffer
	{
		return CommandBuffer();
	}

	void Renderer::Render(const CommandBuffer& commandBuffer)
	{
		for (auto& command : commandBuffer.m_Commands)
		{
			command();
		}
	}
}
