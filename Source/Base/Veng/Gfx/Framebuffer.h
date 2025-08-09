#ifndef _VENG_GFX_FRAMEBUFFER
#define _VENG_GFX_FRAMEBUFFER

#include <AX/Tool/Macro.h>
#include <Veng/Asset/Texture.h>

#include <glm/glm.hpp>

namespace Veng::Gfx
{
	class Framebuffer
	{
	public:
		Framebuffer() = default;
		Framebuffer(const std::string& name, const glm::ivec2& size);

		void Use() const;

		inline auto GetSize() const -> const glm::ivec2&;
		inline auto GetColorTarget() const -> const Asset::AssetMeta<Asset::Texture>&;

	public:
		~Framebuffer() noexcept;
		AX_SemanticsDeleteCopy(Framebuffer)
		AX_SemanticsImplMove(Framebuffer, m_FBO, m_Size, m_ColorTarget)

	private:
		unsigned int m_FBO = 0u;

		glm::ivec2 m_Size = {};
		Asset::AssetMeta<Asset::Texture> m_ColorTarget = {};
	};

	/***** IMPL *****/

	inline auto Framebuffer::GetSize() const -> const glm::ivec2&
	{
		return m_Size;
	}
	inline auto Framebuffer::GetColorTarget() const -> const Asset::AssetMeta<Asset::Texture>&
	{
		return m_ColorTarget;
	}
}

#endif // !_VENG_GFX_FRAMEBUFFER
