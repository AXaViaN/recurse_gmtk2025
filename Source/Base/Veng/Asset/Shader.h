#ifndef _VENG_ASSET_SHADER
#define _VENG_ASSET_SHADER

#include <AX/Tool/Macro.h>
#include <Veng/Asset/Material.h>

#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

namespace Veng::Asset
{
	class Shader
	{
		friend class ShaderHelper;
	public:
		static auto CreateAsset(const std::string& path) -> AssetMeta<Shader>;

	public:
		Shader(const std::string& path);

		void Use() const;

		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, const AssetMeta<Texture>& value);
		void SetUniform(const std::string& name, const glm::vec2& value);
		void SetUniform(const std::string& name, const glm::vec3& value);
		void SetUniform(const std::string& name, const glm::vec4& value);
		void SetUniform(const std::string& name, const glm::mat4& value);

		inline auto GetMaterialProperties() -> const std::vector<Material::Property>&;

		static auto GetRenderBackendVersion() -> const std::string&;

	public:
		~Shader() noexcept;
		AX_SemanticsDeleteCopy(Shader)
		AX_SemanticsImplMove(Shader, m_ProgramID, m_Name, m_UniformLocations, m_MaterialProperties)

	private:
		unsigned int m_ProgramID = 0u;
		std::string m_Name;

		std::unordered_map<std::string, int> m_UniformLocations;
		std::vector<Material::Property> m_MaterialProperties;
	};

	/***** IMPL *****/

	inline auto Shader::GetMaterialProperties() -> const std::vector<Material::Property>&
	{
		return m_MaterialProperties;
	}
}

#endif // !_VENG_ASSET_SHADER
