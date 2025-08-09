#include <Veng/Gfx/Framebuffer.h>

#include <Veng/Gfx/Renderer.h>

#include <glax/gl.h>

namespace Veng::Gfx
{
	Framebuffer::Framebuffer(const std::string& name, const glm::ivec2& size) :
		m_Size(size)
	{
		glGenFramebuffers(1, &m_FBO);
		Use();

		m_ColorTarget = Asset::AssetManager::CreateAsset(
			std::format("{}_ColorTarget", name), Asset::Texture(m_Size)
		);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTarget->GetRenderID(), 0);

		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		auto errorStatusMap = std::unordered_map<unsigned int, std::string>
		{
			{GL_FRAMEBUFFER_UNDEFINED, "GL_FRAMEBUFFER_UNDEFINED"},
			{GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"},
			{GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"},
			{GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER, "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"},
			{GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER, "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"},
			{GL_FRAMEBUFFER_UNSUPPORTED, "GL_FRAMEBUFFER_UNSUPPORTED"},
			{GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE, "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"},
			{GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS, "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"}
		};
		AX_Assert(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer creation error: " + errorStatusMap[status]);
	}

	void Framebuffer::Use() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}

	Framebuffer::~Framebuffer() noexcept
	{
		if (m_FBO != 0u)
		{
			glDeleteFramebuffers(1, &m_FBO);
			Veng::Asset::AssetManager::RemoveAsset(m_ColorTarget);

			m_FBO = 0u;
			m_Size = {};
			m_ColorTarget = {};
		}
	}
}
