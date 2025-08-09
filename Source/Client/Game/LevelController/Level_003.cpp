#include "LevelController/Level_003.h"

namespace Game::LevelController
{
	void Level_003::Initialize(Core::ApplicationContext& context)
	{
		auto map = std::vector<std::vector<std::string>>
		{
			{ "",				"",			"",			"Door02",	"Door12",	"Door22",	"Door32",	"",			"",			"" },
			{ "WallTopLeft",	"WallTop",	"WallTop",	"Door01",	"Door11",	"Door21",	"Door31",	"WallTop",	"WallTop",	"WallTopRight" },
			{ "SideWallLeft",	"Wall",		"Wall",		"Door00",	"Door10",	"Door20",	"Door30",	"Wall",		"Wall",		"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "WallTopLeft",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTopRight" },
			{ "WallBottomLeft",	"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"WallBottomRight" },
		};
		auto mapTileSize = 0.15f;
		auto levelTime = 10.0f;
		auto playerSpawnPosition = glm::vec2(4.5f, 7);
		auto redButtonSpawnPosition = glm::vec2(2, 5);
		auto redFlaskSpawnPosition = glm::vec2(7, 5);

		context.CurrentLevelInfo.emplace(context, map, mapTileSize, playerSpawnPosition, levelTime);

		m_RedButton.emplace(
			context, "RedButton", "RedButtonPressed",
			context.CommonAssets.ButtonUpSfx, context.CommonAssets.ButtonDownSfx, redButtonSpawnPosition
		);
		m_RedFlask.emplace(context, "RedFlask", redFlaskSpawnPosition);

		SpawnPlayer(context);
	}

	void Level_003::Update(Core::ApplicationContext& context, float /*deltaTime*/, const std::vector<AX::ID<SceneSystem::Object>>& /*echoObjects*/)
	{
		auto isRedButtonPressed = m_RedButton->IsColliding(m_RedFlask->GetObjectID());

		m_RedButton->SetToggled(isRedButtonPressed);

		SetDoorState(context, isRedButtonPressed);
	}
	void Level_003::FixedUpdate(Core::ApplicationContext& /*context*/, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects)
	{
		m_RedFlask->FixedUpdate(echoObjects);
	}
}
