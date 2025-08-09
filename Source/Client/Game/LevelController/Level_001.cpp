#include "LevelController/Level_001.h"

namespace Game::LevelController
{
	void Level_001::Initialize(Core::ApplicationContext& context)
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
		auto playerSpawnPosition = glm::vec2(6, 6);
		auto switchSpawnPosition = glm::vec2(3, 5);

		context.CurrentLevelInfo.emplace(context, map, mapTileSize, playerSpawnPosition, levelTime);

		m_Switch.emplace(
			context, "Switch", "SwitchPressed",
			context.CommonAssets.SwitchSfx, context.CommonAssets.SwitchSfx, switchSpawnPosition
		);

		SpawnPlayer(context);
	}

	void Level_001::Update(Core::ApplicationContext& context, float /*deltaTime*/, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects)
	{
		auto isPlayerOnSwitch = m_Switch->IsColliding(context.Player.ObjectID);
		auto isAnyEchoOnSwitch = false;
		for (auto& echoObjectID : echoObjects)
		{
			if (m_Switch->IsColliding(echoObjectID))
			{
				isAnyEchoOnSwitch = true;
				break;
			}
		}

		auto isDoorOpen = (isPlayerOnSwitch || isAnyEchoOnSwitch);
		m_Switch->SetToggled(isDoorOpen);
		SetDoorState(context, isDoorOpen);
	}
}
