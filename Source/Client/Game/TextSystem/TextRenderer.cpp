#include "TextSystem/TextRenderer.h"

#include <Veng/Gfx/Renderer.h>

namespace Game::TextSystem
{
	class TextRendererHelper
	{
	public:
		struct TextInfo
		{
			std::string Text;
			glm::vec2 Position;
			float Scale;
		};

	public:
		static constexpr glm::vec2 UVScale = glm::vec2(1.0f / 13.0f, 1.0f / 7.0f);

	public:
		Veng::Gfx::Context RenderContext;
		std::unordered_map<char, glm::vec2> BitmapInfo;

		std::vector<TextInfo> TextInfos;
	};

	static TextRendererHelper s_Helper;

	/***** CLASS IMPL *****/

	void TextRenderer::Initialize(Veng::Asset::AssetMeta<Veng::Asset::Mesh> quadMeshAsset, Veng::Asset::AssetMeta<Veng::Asset::Shader> shaderAsset)
	{
		s_Helper.RenderContext = Veng::Gfx::Renderer::CreateContext();
		s_Helper.RenderContext.ViewMatrix = glm::mat4(1.0f);
		s_Helper.RenderContext.ModelMatrix = glm::mat4(1.0f);
		s_Helper.RenderContext.MeshAsset = quadMeshAsset;
		s_Helper.RenderContext.MaterialAsset = Veng::Asset::Material::CreateAsset("FontMaterial", shaderAsset);

		auto fontBitmapAsset = Veng::Asset::Texture::CreateAsset("Data/Font/PixelBitmapFonts/fonts/thick_8x8.png");
		s_Helper.RenderContext.MaterialAsset->SetProperty("Texture", fontBitmapAsset);
		s_Helper.RenderContext.MaterialAsset->SetProperty("UVScale", s_Helper.UVScale);
		s_Helper.RenderContext.MaterialAsset->SetProperty("UVOffset", glm::vec2(0.0f));

		s_Helper.BitmapInfo =
		{
			{ 'A', { 0, 6 } },
			{ 'B', { 1, 6 } },
			{ 'C', { 2, 6 } },
			{ 'D', { 3, 6 } },
			{ 'E', { 4, 6 } },
			{ 'F', { 5, 6 } },
			{ 'G', { 6, 6 } },
			{ 'H', { 7, 6 } },
			{ 'I', { 8, 6 } },
			{ 'J', { 9, 6 } },
			{ 'K', { 10, 6 } },
			{ 'L', { 11, 6 } },
			{ 'M', { 12, 6 } },
			{ 'N', { 0, 5 } },
			{ 'O', { 1, 5 } },
			{ 'P', { 2, 5 } },
			{ 'Q', { 3, 5 } },
			{ 'R', { 4, 5 } },
			{ 'S', { 5, 5 } },
			{ 'T', { 6, 5 } },
			{ 'U', { 7, 5 } },
			{ 'V', { 8, 5 } },
			{ 'W', { 9, 5 } },
			{ 'X', { 10, 5 } },
			{ 'Y', { 11, 5 } },
			{ 'Z', { 12, 5 } },
			{ 'a', { 0, 4 } },
			{ 'b', { 1, 4 } },
			{ 'c', { 2, 4 } },
			{ 'd', { 3, 4 } },
			{ 'e', { 4, 4 } },
			{ 'f', { 5, 4 } },
			{ 'g', { 6, 4 } },
			{ 'h', { 7, 4 } },
			{ 'i', { 8, 4 } },
			{ 'j', { 9, 4 } },
			{ 'k', { 10, 4 } },
			{ 'l', { 11, 4 } },
			{ 'm', { 12, 4 } },
			{ 'n', { 0, 3 } },
			{ 'o', { 1, 3 } },
			{ 'p', { 2, 3 } },
			{ 'q', { 3, 3 } },
			{ 'r', { 4, 3 } },
			{ 's', { 5, 3 } },
			{ 't', { 6, 3 } },
			{ 'u', { 7, 3 } },
			{ 'v', { 8, 3 } },
			{ 'w', { 9, 3 } },
			{ 'x', { 10, 3 } },
			{ 'y', { 11, 3 } },
			{ 'z', { 12, 3 } },
			{ '0', { 0, 2 } },
			{ '1', { 1, 2 } },
			{ '2', { 2, 2 } },
			{ '3', { 3, 2 } },
			{ '4', { 4, 2 } },
			{ '5', { 5, 2 } },
			{ '6', { 6, 2 } },
			{ '7', { 7, 2 } },
			{ '8', { 8, 2 } },
			{ '9', { 9, 2 } },
			{ '+', { 10, 2 } },
			{ '-', { 11, 2 } },
			{ '=', { 12, 2 } },
			{ '(', { 0, 1 } },
			{ ')', { 1, 1 } },
			{ '[', { 2, 1 } },
			{ ']', { 3, 1 } },
			{ '{', { 4, 1 } },
			{ '}', { 5, 1 } },
			{ '<', { 6, 1 } },
			{ '>', { 7, 1 } },
			{ '/', { 8, 1 } },
			{ '*', { 9, 1 } },
			{ ':', { 10, 1 } },
			{ '#', { 11, 1 } },
			{ '%', { 12, 1 } },
			{ '!', { 0, 0 } },
			{ '?', { 1, 0 } },
			{ '.', { 2, 0 } },
			{ ',', { 3, 0 } },
			{ '\'', { 4, 0 } },
			{ '\"', { 5, 0 } },
			{ '@', { 6, 0 } },
			{ '&', { 7, 0 } },
			{ '$', { 8, 0 } },
			{ ' ', { 9, 0 } }
		};
	}
	void TextRenderer::SetProjectionMatrix(const glm::mat4& projectionMatrix)
	{
		s_Helper.RenderContext.ProjectionMatrix = projectionMatrix;
	}

	void TextRenderer::AddText(const std::string& text, const glm::vec2& position, float scale/*=1.0f*/)
	{
		s_Helper.TextInfos.emplace_back(TextRendererHelper::TextInfo
		{
			.Text = text,
			.Position = position,
			.Scale = scale
		});
	}

	void TextRenderer::RenderAll()
	{
		for (auto& textInfo : s_Helper.TextInfos)
		{
			constexpr auto scale = glm::vec2(0.125f);
			s_Helper.RenderContext.ModelMatrix = glm::translate(
				glm::mat4(1.0f), glm::vec3(textInfo.Position * scale, 0.0f)
			);
			s_Helper.RenderContext.ModelMatrix = glm::scale(
				s_Helper.RenderContext.ModelMatrix, glm::vec3(scale * textInfo.Scale, 1.0f)
			);

			for (auto& character : textInfo.Text)
			{
				auto charIt = s_Helper.BitmapInfo.find(character);
				AX_Assert(charIt != s_Helper.BitmapInfo.end());

				auto& characterOffset = charIt->second;
				s_Helper.RenderContext.MaterialAsset->SetProperty("UVOffset", characterOffset);

				// TODO_ASC: Command buffer can't handle material property changes
				auto commandBuffer = Veng::Gfx::Renderer::CreateCommandBuffer();
				commandBuffer.Draw(s_Helper.RenderContext);
				Veng::Gfx::Renderer::Render(commandBuffer);

				s_Helper.RenderContext.ModelMatrix = glm::translate(
					s_Helper.RenderContext.ModelMatrix, { 1, 0, 0 }
				);
			}
		}

		s_Helper.TextInfos.clear();
	}
}
