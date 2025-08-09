#include <Veng/Asset/Material.h>

#include <AX/Tool/Reporter.h>
#include <Veng/Asset/Shader.h>

namespace Veng::Asset
{
	auto Material::CreateAsset(const std::string& name, const AssetMeta<Shader>& shaderAsset) -> AssetMeta<Material>
	{
		return AssetManager::CreateAsset(name, Material(shaderAsset));
	}

	Material::Material(const AssetMeta<Shader>& shaderAsset) :
		m_ShaderAsset(shaderAsset),
		m_Properties(m_ShaderAsset->GetMaterialProperties())
	{
		auto idx = 0ull;
		for (auto& property : m_Properties)
		{
			m_PropertyNameToIdx.emplace(property.Name, idx++);

			property.Value = property.DefaultValue;
		}
	}

	void Material::Use() const
	{
		auto shader = *m_ShaderAsset;
		shader->Use();

		for (auto& property : m_Properties)
		{
			switch (property.Type)
			{
				case PropertyType::Float:
				{
					shader->SetUniform(property.Name, property.Value.AsFloat());
					break;
				}
				case PropertyType::Vec2:
				{
					shader->SetUniform(property.Name, property.Value.AsVec2());
					break;
				}
				case PropertyType::Vec3:
				{
					shader->SetUniform(property.Name, property.Value.AsVec3());
					break;
				}
				case PropertyType::Vec4:
				{
					shader->SetUniform(property.Name, property.Value.AsVec4());
					break;
				}
				case PropertyType::Texture:
				{
					auto& textureAsset = property.Value.AsTexture();
					if (textureAsset)
					{
						shader->SetUniform(property.Name, textureAsset);
					}
					break;
				}
			}
		}
	}

	auto Material::GetProperty(const std::string& name) const -> const Property&
	{
#ifndef AX_CONFIG_SHIPPING
		AX_Assert(m_PropertyNameToIdx.contains(name));
#endif // !AX_CONFIG_SHIPPING

		return m_Properties.at(m_PropertyNameToIdx.at(name));
	}
	void Material::SetProperty(const std::string& name, const PropertyValue& value)
	{
#ifndef AX_CONFIG_SHIPPING
		AX_Assert(m_PropertyNameToIdx.contains(name));
#endif // !AX_CONFIG_SHIPPING

		m_Properties.at(m_PropertyNameToIdx.at(name)).Value = value;
	}
}
