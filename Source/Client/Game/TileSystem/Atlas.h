#ifndef _GAME_TILESYSTEM_ATLAS
#define _GAME_TILESYSTEM_ATLAS

#include <Veng/Asset/Material.h>

#include <glm/glm.hpp>

#include <string>

namespace Game::TileSystem
{
	class Atlas
	{
	public:
		Atlas(const std::string& path, const glm::vec2& size, Veng::Asset::AssetMeta<Veng::Asset::Shader> shaderAsset);

		void AddSprite(const std::string& name, const glm::vec2& position);
		
		auto GetSpriteMaterial(const std::string& name) -> Veng::Asset::AssetMeta<Veng::Asset::Material>;

	private:
		Veng::Asset::AssetMeta<Veng::Asset::Shader> m_ShaderAsset;
		Veng::Asset::AssetMeta<Veng::Asset::Texture> m_TextureAsset;
		glm::vec2 m_UVScale;

		std::unordered_map<std::string, Veng::Asset::AssetMeta<Veng::Asset::Material>> m_SpriteMap;
	};
}

#endif // !_GAME_TILESYSTEM_ATLAS
