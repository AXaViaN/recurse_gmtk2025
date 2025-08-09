#include "TileSystem/Atlas.h"

namespace Game::TileSystem
{
	Atlas::Atlas(const std::string& path, const glm::vec2& size, Veng::Asset::AssetMeta<Veng::Asset::Shader> shaderAsset) :
		m_ShaderAsset(shaderAsset),
		m_TextureAsset(Veng::Asset::Texture::CreateAsset(path)),
		m_UVScale(glm::vec2(1.0f) / size)
	{
	}

	void Atlas::AddSprite(const std::string& name, const glm::vec2& position)
	{
		auto materialAsset = Veng::Asset::Material::CreateAsset(name, m_ShaderAsset);
		materialAsset->SetProperty("Texture", m_TextureAsset);
		materialAsset->SetProperty("UVScale", m_UVScale);
		materialAsset->SetProperty("UVOffset", position);

		m_SpriteMap.emplace(name, materialAsset);
	}

	auto Atlas::GetSpriteMaterial(const std::string& name) -> Veng::Asset::AssetMeta<Veng::Asset::Material>
	{
		auto spriteIt = m_SpriteMap.find(name);
		AX_Assert(spriteIt != m_SpriteMap.end());

		return spriteIt->second;
	}
}
