#ifndef _VENG_ASSET_TEXTURE
#define _VENG_ASSET_TEXTURE

#include <AX/Tool/Macro.h>
#include <Veng/Asset/AssetManager.h>

#include <glm/glm.hpp>

#include <vector>

namespace Veng::Asset
{
	class Texture
	{
	public:
		static auto CreateAsset(const std::string& path) -> AssetMeta<Texture>;
		static auto CreateAsset(const std::string& name, const glm::ivec2& size) -> AssetMeta<Texture>;

	public:
		Texture(const std::string& path);
		Texture(const glm::ivec2& size);

		void Use(unsigned int slot) const;

		void SetPixel(const glm::ivec2& position, const glm::vec4& color);
		void SetPixel(const glm::ivec2& position, const glm::u8vec3& color);
		auto GetPixel(const glm::ivec2& position) const -> const glm::u8vec4&;
		void UpdatePixels();

		inline auto GetRenderID() const -> unsigned int;
		inline auto GetSize() const -> const glm::ivec2&;

	public:
		~Texture() noexcept;
		AX_SemanticsDeleteCopy(Texture)
		AX_SemanticsImplMove(Texture, m_RenderID, m_Size, m_Pixels)

	private:
		unsigned int m_RenderID = 0u;

		glm::ivec2 m_Size = {};
		std::vector<glm::u8vec4> m_Pixels;
	};

	/***** IMPL *****/

	inline auto Texture::GetRenderID() const -> unsigned int
	{
		return m_RenderID;
	}
	inline auto Texture::GetSize() const -> const glm::ivec2&
	{
		return m_Size;
	}
}

#endif // !_VENG_ASSET_TEXTURE
