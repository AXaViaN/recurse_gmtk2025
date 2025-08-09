#ifndef _GAME_CORE_APPLICATIONCONTEXT
#define _GAME_CORE_APPLICATIONCONTEXT

#include <Veng/Asset/Mesh.h>
#include <Veng/Asset/Shader.h>
#include <Veng/LayerSystem/Context.h>
#include "Gameplay/LevelInfo.h"
#include "SceneSystem/Scene.h"
#include "SoundSystem/Sound.h"
#include "TileSystem/Atlas.h"

#include <vector>
#include <memory>
#include <optional>
#include <array>

namespace Game::Core
{
	class ApplicationContext : public Veng::LayerSystem::Context
	{
		VENG_ImplementContext(ApplicationContext)
	public:
		SceneSystem::Scene Scene;

		struct
		{
			Veng::Asset::AssetMeta<Veng::Asset::Mesh> QuadMeshAsset;
			Veng::Asset::AssetMeta<Veng::Asset::Shader> SimpleShaderAsset;
			Veng::Asset::AssetMeta<Veng::Asset::Shader> TileShaderAsset;
			Veng::Asset::AssetMeta<Veng::Asset::Shader> CharacterShaderAsset;

			Veng::Asset::AssetMeta<Veng::Asset::Material> PlayerMaterialAsset;
			std::array<Veng::Asset::AssetMeta<Veng::Asset::Material>, 3> PlayerEchoMaterialAssets;

			Veng::Asset::AssetMeta<Veng::Asset::Material> TransitionScreenMaterialAsset;

			std::unique_ptr<TileSystem::Atlas> DungeonTileAtlas;

			Veng::Asset::AssetMeta<SoundSystem::Sound> MenuMusic;
			Veng::Asset::AssetMeta<SoundSystem::Sound> LevelMusic;

			Veng::Asset::AssetMeta<SoundSystem::Sound> ClockSfx;
			Veng::Asset::AssetMeta<SoundSystem::Sound> ClockFastSfx;
			Veng::Asset::AssetMeta<SoundSystem::Sound> RewindSfx;
			Veng::Asset::AssetMeta<SoundSystem::Sound> SlideSfx;
			Veng::Asset::AssetMeta<SoundSystem::Sound> ButtonDownSfx;
			Veng::Asset::AssetMeta<SoundSystem::Sound> ButtonUpSfx;
			Veng::Asset::AssetMeta<SoundSystem::Sound> DoorOpenSfx;
			Veng::Asset::AssetMeta<SoundSystem::Sound> DoorCloseSfx;
			Veng::Asset::AssetMeta<SoundSystem::Sound> SwitchSfx;
			Veng::Asset::AssetMeta<SoundSystem::Sound> LevelCompleteSfx;
			Veng::Asset::AssetMeta<SoundSystem::Sound> WalkSfx;
		} CommonAssets;

		std::optional<Gameplay::LevelInfo> CurrentLevelInfo;

		struct
		{
			AX::ID<SceneSystem::Object> ObjectID;
			glm::vec2 Scale = glm::vec2(0.175f);
		} Player;
	};
}

#endif // !_GAME_CORE_APPLICATIONCONTEXT
