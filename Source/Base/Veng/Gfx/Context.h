#ifndef _VENG_GFX_CONTEXT
#define _VENG_GFX_CONTEXT

#include <Veng/Asset/Material.h>
#include <Veng/Asset/Mesh.h>

#include <glm/glm.hpp>

namespace Veng::Gfx
{
	class Context
	{
	public:
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ModelMatrix;

		Asset::AssetMeta<Asset::Mesh> MeshAsset;
		Asset::AssetMeta<Asset::Material> MaterialAsset;
	};
}

#endif // !_VENG_GFX_CONTEXT
