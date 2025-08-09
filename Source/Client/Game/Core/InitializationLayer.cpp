#include "Core/InitializationLayer.h"

#include <Veng/LayerSystem/LayerManager.h>
#include "Core/ApplicationContext.h"
#include "Core/RenderLayer.h"
#include "Menu/MainMenuLayer.h"
#include "SoundSystem/SoundLayer.h"
#include "SoundSystem/SoundManager.h"
#include "TextSystem/TextRenderer.h"

namespace Game::Core
{
	class InitializationLayerHelper
	{
	public:
		void InitializeCommonAssets(ApplicationContext& context);

	private:
		auto CreateQuadMeshAsset() -> Veng::Asset::AssetMeta<Veng::Asset::Mesh>;
		void CreateDungeonTileAtlas(ApplicationContext& context);
	};

	static InitializationLayerHelper s_Helper;

	/***** CLASS IMPL *****/

	void InitializationLayer::OnStart()
	{
		auto& context = GetContext<ApplicationContext>();

		SoundSystem::SoundManager::Initialize();

		s_Helper.InitializeCommonAssets(context);

		TextSystem::TextRenderer::Initialize(
			context.CommonAssets.QuadMeshAsset, context.CommonAssets.TileShaderAsset
		);

		p_LayerManager.AddLayer<SoundSystem::SoundLayer>(Veng::LayerSystem::LayerManager::Placement::Bottom);
		p_LayerManager.AddLayer<Menu::MainMenuLayer>(Veng::LayerSystem::LayerManager::Placement::Top);
		p_LayerManager.AddLayer<RenderLayer>(Veng::LayerSystem::LayerManager::Placement::Bottom);

		p_LayerManager.RemoveLayer(*this);
	}

	/***** HELPER IMPL *****/

	void InitializationLayerHelper::InitializeCommonAssets(ApplicationContext& context)
	{
		context.CommonAssets.QuadMeshAsset = CreateQuadMeshAsset();
		context.CommonAssets.SimpleShaderAsset = Veng::Asset::Shader::CreateAsset("Data/Shader/Simple2D.glsl");
		context.CommonAssets.TileShaderAsset = Veng::Asset::Shader::CreateAsset("Data/Shader/Tile2D.glsl");

		context.CommonAssets.CharacterShaderAsset = Veng::Asset::Shader::CreateAsset("Data/Shader/Character.glsl");
		context.CommonAssets.PlayerMaterialAsset = Veng::Asset::Material::CreateAsset(
			"PlayerMaterial", context.CommonAssets.CharacterShaderAsset
		);
		context.CommonAssets.PlayerMaterialAsset->SetProperty("Color", glm::vec3(0.25, 0.75, 0.5));

		auto echoColors = std::array<glm::vec3, 3>
		{
			glm::vec3(0.25, 0.5, 0.75),
			glm::vec3(0.5, 0.25, 0.5),
			glm::vec3(0.75, 0.25, 0.25)
		};
		for (auto i = 0; i < context.CommonAssets.PlayerEchoMaterialAssets.size(); ++i)
		{
			context.CommonAssets.PlayerEchoMaterialAssets[i] = Veng::Asset::Material::CreateAsset(
				std::format("PlayerEcho_{}", i), context.CommonAssets.CharacterShaderAsset
			);
			context.CommonAssets.PlayerEchoMaterialAssets[i]->SetProperty("Color", echoColors[i]);
		}

		context.CommonAssets.TransitionScreenMaterialAsset = Veng::Asset::Material::CreateAsset(
			"TransitionScreenMaterial", context.CommonAssets.SimpleShaderAsset
		);
		context.CommonAssets.TransitionScreenMaterialAsset->SetProperty(
			"Texture", Veng::Asset::Texture::CreateAsset("TransitionScreenTexture", { 1, 1 })
		);

		CreateDungeonTileAtlas(context);

		context.CommonAssets.MenuMusic = SoundSystem::Sound::CreateAsset("Data/Sound/menu.mp3");
		context.CommonAssets.MenuMusic->SetIsLooping(true);
		context.CommonAssets.MenuMusic->SetBaseVolume(0.75f);

		context.CommonAssets.LevelMusic = SoundSystem::Sound::CreateAsset("Data/Sound/level.mp3");
		context.CommonAssets.LevelMusic->SetIsLooping(true);
		context.CommonAssets.LevelMusic->SetBasePitch(0.75f);
		context.CommonAssets.LevelMusic->SetBaseVolume(0.6f);

		context.CommonAssets.ClockSfx = SoundSystem::Sound::CreateAsset("Data/Sound/clock.mp3");
		context.CommonAssets.ClockSfx->SetIsLooping(true);
		context.CommonAssets.ClockSfx->SetBaseVolume(0.2f);
		context.CommonAssets.ClockFastSfx = SoundSystem::Sound::CreateAsset("Data/Sound/clock_fast.mp3");
		context.CommonAssets.ClockFastSfx->SetIsLooping(true);
		context.CommonAssets.ClockFastSfx->SetBaseVolume(0.3f);

		context.CommonAssets.RewindSfx = SoundSystem::Sound::CreateAsset("Data/Sound/rewind.mp3");
		context.CommonAssets.RewindSfx->SetBaseVolume(1.5f);

		context.CommonAssets.SlideSfx = SoundSystem::Sound::CreateAsset("Data/Sound/slide.mp3");
		context.CommonAssets.SlideSfx->SetIsLooping(true);
		context.CommonAssets.SlideSfx->SetBaseVolume(0.75f);
		context.CommonAssets.SlideSfx->SetVolume(0.0f);
		context.CommonAssets.SlideSfx->Play();

		context.CommonAssets.ButtonDownSfx = SoundSystem::Sound::CreateAsset("Data/Sound/button_down.mp3");
		context.CommonAssets.ButtonDownSfx->SetBaseVolume(5.0f);
		context.CommonAssets.ButtonUpSfx = SoundSystem::Sound::CreateAsset("Data/Sound/button_up.mp3");
		context.CommonAssets.ButtonUpSfx->SetBaseVolume(5.0f);

		context.CommonAssets.DoorOpenSfx = SoundSystem::Sound::CreateAsset("Data/Sound/door_open.mp3");
		context.CommonAssets.DoorOpenSfx->SetBaseVolume(0.5f);
		context.CommonAssets.DoorCloseSfx = SoundSystem::Sound::CreateAsset("Data/Sound/door_close.mp3");
		context.CommonAssets.DoorCloseSfx->SetBaseVolume(0.5f);

		context.CommonAssets.SwitchSfx = SoundSystem::Sound::CreateAsset("Data/Sound/switch.mp3");
		context.CommonAssets.SwitchSfx->SetBaseVolume(0.5f);

		context.CommonAssets.LevelCompleteSfx = SoundSystem::Sound::CreateAsset("Data/Sound/level_complete.mp3");
		context.CommonAssets.LevelCompleteSfx->SetBaseVolume(10.0f);

		context.CommonAssets.WalkSfx = SoundSystem::Sound::CreateAsset("Data/Sound/walk.mp3");
		context.CommonAssets.WalkSfx->SetIsLooping(true);
		context.CommonAssets.WalkSfx->SetBaseVolume(0.25f);
		context.CommonAssets.WalkSfx->SetVolume(0.0f);
		context.CommonAssets.WalkSfx->Play();
	}

	auto InitializationLayerHelper::CreateQuadMeshAsset() -> Veng::Asset::AssetMeta<Veng::Asset::Mesh>
	{
		struct VertexData
		{
			glm::vec2 Position;
			glm::vec2 UV;
		};

		auto vertices = std::vector<VertexData>
		{
			{ {-0.5f,  0.5f}, {0.0f, 1.0f} },	// Top left
			{ { 0.5f,  0.5f}, {1.0f, 1.0f} },	// Top right
			{ { 0.5f, -0.5f}, {1.0f, 0.0f} },	// Bottom right
			{ {-0.5f, -0.5f}, {0.0f, 0.0f} }	// Bottom left
		};
		auto indices = std::vector<unsigned int>
		{
			0, 3, 2,
			0, 2, 1
		};
		auto meshAsset = Veng::Asset::Mesh::CreateAsset("Quad", Veng::Asset::Mesh::Info
		{
			.Type = Veng::Asset::Mesh::MeshType::Triangle,
			.VertexData = vertices.data(),
			.VertexSize = sizeof(VertexData),
			.VertexCount = vertices.size(),
			.VertexAttribInfo =
			{
				{ Veng::Asset::Mesh::VertexAttribType::Vec2, offsetof(VertexData, Position) },
				{ Veng::Asset::Mesh::VertexAttribType::Vec2, offsetof(VertexData, UV) }
			},
			.Indices = indices
		});

		return meshAsset;
	}
	void InitializationLayerHelper::CreateDungeonTileAtlas(ApplicationContext& context)
	{
		context.CommonAssets.DungeonTileAtlas = std::make_unique<TileSystem::Atlas>(
			"Data/Sprite/0x72_DungeonTilesetII_v1.7/0x72_DungeonTilesetII_v1.7.png",
			glm::vec2(32), context.CommonAssets.TileShaderAsset
		);

		context.CommonAssets.DungeonTileAtlas->AddSprite("Wall", glm::vec2(2, 30));
		context.CommonAssets.DungeonTileAtlas->AddSprite("WallTop", glm::vec2(2, 31));
		context.CommonAssets.DungeonTileAtlas->AddSprite("WallTopLeft", glm::vec2(1 + 0.315, 23.5));
		context.CommonAssets.DungeonTileAtlas->AddSprite("WallTopRight", glm::vec2(4 - 0.310, 23.5));
		context.CommonAssets.DungeonTileAtlas->AddSprite("WallBottomLeft", glm::vec2(0.315, 30));
		context.CommonAssets.DungeonTileAtlas->AddSprite("WallBottomRight", glm::vec2(1, 20.5));
		context.CommonAssets.DungeonTileAtlas->AddSprite("SideWallRight", glm::vec2(2, 21));
		context.CommonAssets.DungeonTileAtlas->AddSprite("SideWallLeft", glm::vec2(3, 21));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Ground", glm::vec2(1, 27));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door00", glm::vec2(1, 15));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door01", glm::vec2(1, 16));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door02", glm::vec2(1, 17));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door10", glm::vec2(2, 15));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door11", glm::vec2(2, 16));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door12", glm::vec2(2, 17));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door20", glm::vec2(3, 15));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door21", glm::vec2(3, 16));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door22", glm::vec2(3, 17));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door30", glm::vec2(4, 15));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door31", glm::vec2(4, 16));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door32", glm::vec2(4, 17));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door10_Open", glm::vec2(5, 15));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door11_Open", glm::vec2(5, 16));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door20_Open", glm::vec2(6, 15));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Door21_Open", glm::vec2(6, 16));
		context.CommonAssets.DungeonTileAtlas->AddSprite("RedButton", glm::vec2(1, 18));
		context.CommonAssets.DungeonTileAtlas->AddSprite("RedButtonPressed", glm::vec2(2, 18));
		context.CommonAssets.DungeonTileAtlas->AddSprite("BlueButton", glm::vec2(3, 18));
		context.CommonAssets.DungeonTileAtlas->AddSprite("BlueButtonPressed", glm::vec2(4, 18));
		context.CommonAssets.DungeonTileAtlas->AddSprite("Switch", glm::vec2(5, 18));
		context.CommonAssets.DungeonTileAtlas->AddSprite("SwitchPressed", glm::vec2(6, 18));
		context.CommonAssets.DungeonTileAtlas->AddSprite("RedFlask", glm::vec2(18, 10));
		context.CommonAssets.DungeonTileAtlas->AddSprite("BlueFlask", glm::vec2(19, 10));
	}
}
