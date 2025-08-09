#ifndef _GAME_SCENESYSTEM_OBJECT
#define _GAME_SCENESYSTEM_OBJECT

#include <Veng/Asset/Material.h>

namespace Game::SceneSystem
{
	class Object
	{
	public:
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Scale = glm::vec2(1.0f);

		Veng::Asset::AssetMeta<Veng::Asset::Material> MaterialAsset;
		bool SimpleAlphaBlending = false;
		bool AdditiveAlphaBlending = false;
	};
}

#endif // !_GAME_SCENESYSTEM_OBJECT
