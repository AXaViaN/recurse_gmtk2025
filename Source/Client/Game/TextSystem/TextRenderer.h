#ifndef _GAME_TEXTSYSTEM_TEXTRENDERER
#define _GAME_TEXTSYSTEM_TEXTRENDERER

#include <Veng/Asset/Mesh.h>
#include <Veng/Asset/Shader.h>

#include <string>

namespace Game::TextSystem
{
	class TextRenderer
	{
	public:
		static void Initialize(Veng::Asset::AssetMeta<Veng::Asset::Mesh> quadMeshAsset, Veng::Asset::AssetMeta<Veng::Asset::Shader> shaderAsset);
		static void SetProjectionMatrix(const glm::mat4& projectionMatrix);

		static void AddText(const std::string& text, const glm::vec2& position, float scale=1.0f);

		static void RenderAll();
	};
}

#endif // !_GAME_TEXTSYSTEM_TEXTRENDERER
