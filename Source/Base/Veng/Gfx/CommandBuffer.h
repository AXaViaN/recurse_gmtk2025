#ifndef _VENG_GFX_COMMANDBUFFER
#define _VENG_GFX_COMMANDBUFFER

#include <glm/glm.hpp>

#include <functional>
#include <vector>

namespace Veng::Gfx
{
	class Context;
	class Framebuffer;

	class CommandBuffer
	{
		friend class Renderer;
	public:
		void Reset();

		void Clear();
		void Clear(const glm::vec3& color);

		void UseFramebuffer(const Framebuffer& framebuffer);
		void UseDefaultFramebuffer();

		void EnableSimpleAlphaBlending();
		void EnableAdditiveAlphaBlending();
		void DisableAlphaBlending();

		void Draw(const Context& context);

	private:
		std::vector<std::function<void()>> m_Commands;
	};
}

#endif // !_VENG_GFX_COMMANDBUFFER
