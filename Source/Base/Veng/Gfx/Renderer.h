#ifndef _VENG_GFX_RENDERER
#define _VENG_GFX_RENDERER

#include <Veng/Gfx/CommandBuffer.h>
#include <Veng/Gfx/Context.h>

namespace Veng::Gfx
{
	class Renderer
	{
	public:
		static void Init();
		static void SetViewport(glm::ivec2 position, glm::ivec2 size);

		static void SetWireframeMode(bool isWireframeMode);
		static void SetWireframeWidth(float width);

		static auto CreateContext() -> Context;
		static auto CreateCommandBuffer() -> CommandBuffer;

		static void Render(const CommandBuffer& commandBuffer);
	};
}

#endif // !_VENG_GFX_RENDERER
