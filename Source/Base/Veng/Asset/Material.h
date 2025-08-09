#ifndef _VENG_ASSET_MATERIAL
#define _VENG_ASSET_MATERIAL

#include <Veng/Asset/Texture.h>

#include <glm/glm.hpp>

#include <unordered_map>
#include <vector>
#include <string>
#include <variant>

namespace Veng::Asset
{
	class Shader;

	class Material
	{
	public:
		enum class PropertyType;
		class PropertyValue
		{
		public:
			inline PropertyValue() = default;
			inline PropertyValue(const float& value);
			inline PropertyValue(const glm::vec2& value);
			inline PropertyValue(const glm::vec3& value);
			inline PropertyValue(const glm::vec4& value);
			inline PropertyValue(const AssetMeta<Texture>& value);

			inline auto AsFloat(const float& value) -> PropertyValue&;
			inline auto AsVec2(const glm::vec2& value) -> PropertyValue&;
			inline auto AsVec3(const glm::vec3& value) -> PropertyValue&;
			inline auto AsVec4(const glm::vec4& value) -> PropertyValue&;
			inline auto AsTexture(const AssetMeta<Texture>& value) -> PropertyValue&;

			inline auto AsFloat() -> float&;
			inline auto AsVec2() -> glm::vec2&;
			inline auto AsVec3() -> glm::vec3&;
			inline auto AsVec4() -> glm::vec4&;
			inline auto AsTexture() -> AssetMeta<Texture>&;

			inline auto AsFloat() const -> const float&;
			inline auto AsVec2() const -> const glm::vec2&;
			inline auto AsVec3() const -> const glm::vec3&;
			inline auto AsVec4() const -> const glm::vec4&;
			inline auto AsTexture() const -> const AssetMeta<Texture>&;

		private:
			std::variant<float, glm::vec2, glm::vec3, glm::vec4, AssetMeta<Texture>> m_Value;
		};

		struct Property
		{
			std::string Name;
			PropertyType Type;

			PropertyValue Value;
			PropertyValue DefaultValue;
			PropertyValue MinValue;
			PropertyValue MaxValue;
		};

	public:
		static auto CreateAsset(const std::string& name, const AssetMeta<Shader>& shaderAsset) -> AssetMeta<Material>;

	public:
		Material(const AssetMeta<Shader>& shaderAsset);

		void Use() const;

		auto GetProperty(const std::string& name) const -> const Property&;
		void SetProperty(const std::string& name, const PropertyValue& value);

		inline auto GetShaderAsset() const -> const AssetMeta<Shader>&;
		inline auto GetProperties() const -> const std::vector<Property>&;

	private:
		AssetMeta<Shader> m_ShaderAsset;
		std::vector<Property> m_Properties;
		std::unordered_map<std::string, size_t> m_PropertyNameToIdx;
	};

	/***** PropertyType IMPL *****/

	enum class Material::PropertyType
	{
		Float,
		Vec2,
		Vec3,
		Vec4,
		Texture
	};

	/***** PropertyValue IMPL *****/

	inline Material::PropertyValue::PropertyValue(const float& value) :
		m_Value(value)
	{
	}
	inline Material::PropertyValue::PropertyValue(const glm::vec2& value) :
		m_Value(value)
	{
	}
	inline Material::PropertyValue::PropertyValue(const glm::vec3& value) :
		m_Value(value)
	{
	}
	inline Material::PropertyValue::PropertyValue(const glm::vec4& value) :
		m_Value(value)
	{
	}
	inline Material::PropertyValue::PropertyValue(const AssetMeta<Texture>& value) :
		m_Value(value)
	{
	}

	inline auto Material::PropertyValue::AsFloat(const float& value) -> PropertyValue&
	{
		m_Value = value;
		return *this;
	}
	inline auto Material::PropertyValue::AsVec2(const glm::vec2& value) -> PropertyValue&
	{
		m_Value = value;
		return *this;
	}
	inline auto Material::PropertyValue::AsVec3(const glm::vec3& value) -> PropertyValue&
	{
		m_Value = value;
		return *this;
	}
	inline auto Material::PropertyValue::AsVec4(const glm::vec4& value) -> PropertyValue&
	{
		m_Value = value;
		return *this;
	}
	inline auto Material::PropertyValue::AsTexture(const AssetMeta<Texture>& value) -> PropertyValue&
	{
		m_Value = value;
		return *this;
	}

	inline auto Material::PropertyValue::AsFloat() -> float&
	{
		return std::get<float>(m_Value);
	}
	inline auto Material::PropertyValue::AsVec2() -> glm::vec2&
	{
		return std::get<glm::vec2>(m_Value);
	}
	inline auto Material::PropertyValue::AsVec3() -> glm::vec3&
	{
		return std::get<glm::vec3>(m_Value);
	}
	inline auto Material::PropertyValue::AsVec4() -> glm::vec4&
	{
		return std::get<glm::vec4>(m_Value);
	}
	inline auto Material::PropertyValue::AsTexture() -> AssetMeta<Texture>&
	{
		return std::get<AssetMeta<Texture>>(m_Value);
	}

	inline auto Material::PropertyValue::AsFloat() const -> const float&
	{
		return std::get<float>(m_Value);
	}
	inline auto Material::PropertyValue::AsVec2() const -> const glm::vec2&
	{
		return std::get<glm::vec2>(m_Value);
	}
	inline auto Material::PropertyValue::AsVec3() const -> const glm::vec3&
	{
		return std::get<glm::vec3>(m_Value);
	}
	inline auto Material::PropertyValue::AsVec4() const -> const glm::vec4&
	{
		return std::get<glm::vec4>(m_Value);
	}
	inline auto Material::PropertyValue::AsTexture() const -> const AssetMeta<Texture>&
	{
		return std::get<AssetMeta<Texture>>(m_Value);
	}

	/***** Material IMPL *****/

	inline auto Material::GetShaderAsset() const -> const AssetMeta<Shader>&
	{
		return m_ShaderAsset;
	}
	inline auto Material::GetProperties() const -> const std::vector<Property>&
	{
		return m_Properties;
	}
}

#endif // !_VENG_ASSET_MATERIAL
