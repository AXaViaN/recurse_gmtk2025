#include <Veng/Asset/Texture.h>

#include <AX/Tool/PathManager.h>
#include <AX/Tool/Reporter.h>

#include <glax/gl.h>
#include <stb/stb_image.h>

namespace Veng::Asset
{
	auto Texture::CreateAsset(const std::string& path) -> AssetMeta<Texture>
	{
		return AssetManager::CreateAsset(path, Texture(path));
	}
	auto Texture::CreateAsset(const std::string& name, const glm::ivec2& size) -> AssetMeta<Texture>
	{
		return AssetManager::CreateAsset(name, Texture(size));
	}

	Texture::Texture(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(true);

		auto fullPath = AX::Tool::PathManager::GetFullPath(path);

		auto width = 0;
		auto height = 0;
		auto channels = 0;
		auto pixels = stbi_load(fullPath.c_str(), &width, &height, &channels, STBI_default);
		AX_Assert(
			pixels != nullptr,
			"Texture loading failed at \"" + fullPath + "\": " + stbi_failure_reason()
		);

		static GLenum formats[] = {
			GL_INVALID_ENUM, GL_RED, GL_RG, GL_RGB, GL_RGBA
		};

		glGenTextures(1, &m_RenderID);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		auto& format = formats[channels];
		glTexImage2D(
			GL_TEXTURE_2D, 0, format, width, height, 0,
			format, GL_UNSIGNED_BYTE, pixels
		);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(pixels);

		m_Size = { width, height };
	}
	Texture::Texture(const glm::ivec2& size) :
		m_Size(size)
	{
		glGenTextures(1, &m_RenderID);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);

		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA, m_Size.x, m_Size.y, 0,
			GL_RGB, GL_UNSIGNED_BYTE, nullptr
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		m_Pixels.resize(m_Size.y * m_Size.x);
	}

	void Texture::Use(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);
	}

	void Texture::SetPixel(const glm::ivec2& position, const glm::vec4& color)
	{
		m_Pixels[position.y * m_Size.x + position.x] = color * 255.0f;
	}
	void Texture::SetPixel(const glm::ivec2& position, const glm::u8vec3& color)
	{
		m_Pixels[position.y * m_Size.x + position.x] = glm::u8vec4(color, 1.0f);
	}
	auto Texture::GetPixel(const glm::ivec2& position) const -> const glm::u8vec4&
	{
		return m_Pixels[position.y * m_Size.x + position.x];
	}
	void Texture::UpdatePixels()
	{
		glBindTexture(GL_TEXTURE_2D, m_RenderID);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA, m_Size.x, m_Size.y, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, m_Pixels.data()
		);
	}

	Texture::~Texture() noexcept
	{
		if (m_RenderID != 0u)
		{
			glDeleteTextures(1, &m_RenderID);

			m_RenderID = 0u;
		}
	}
}
