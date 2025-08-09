#include "LevelController/Level_005.h"

namespace Game::LevelController
{
	void Level_005::Initialize(Core::ApplicationContext& context)
	{
		auto map = std::vector<std::vector<std::string>>
		{
			{ "",				"",			"",			"",			"",			"",			"Door02",	"Door12",	"Door22",	"Door32",	"",			"",			"",			"",			"",			"" },
			{ "WallTopLeft",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"Door01",	"Door11",	"Door21",	"Door31",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTopRight" },
			{ "SideWallLeft",	"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Door00",	"Door10",	"Door20",	"Door30",	"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "SideWallLeft",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"Ground",	"SideWallRight" },
			{ "WallTopLeft",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTop",	"WallTopRight" },
			{ "WallBottomLeft",	"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"Wall",		"WallBottomRight" },
		};
		auto mapTileSize = 0.15f;
		auto levelTime = 5.0f;
		auto playerSpawnPosition = glm::vec2(7.5f, 5);
		auto redButtonSpawnPosition = glm::vec2(2, 6);
		auto redFlaskSpawnPosition = glm::vec2(2, 7);
		auto blueButtonSpawnPosition = glm::vec2(13, 6);
		auto blueFlaskSpawnPosition = glm::vec2(13, 7);
		auto switch1SpawnPosition = glm::vec2(6, 3);
		auto switch2SpawnPosition = glm::vec2(9, 3);

		context.CurrentLevelInfo.emplace(context, map, mapTileSize, playerSpawnPosition, levelTime);

		m_RedButton.emplace(
			context, "RedButton", "RedButtonPressed",
			context.CommonAssets.ButtonUpSfx, context.CommonAssets.ButtonDownSfx, redButtonSpawnPosition
		);
		m_RedFlask.emplace(context, "RedFlask", redFlaskSpawnPosition);

		m_BlueButton.emplace(
			context, "BlueButton", "BlueButtonPressed",
			context.CommonAssets.ButtonUpSfx, context.CommonAssets.ButtonDownSfx, blueButtonSpawnPosition
		);
		m_BlueFlask.emplace(context, "BlueFlask", blueFlaskSpawnPosition);

		m_Switch1.emplace(
			context, "Switch", "SwitchPressed",
			context.CommonAssets.SwitchSfx, context.CommonAssets.SwitchSfx, switch1SpawnPosition
		);
		m_Switch2.emplace(
			context, "Switch", "SwitchPressed",
			context.CommonAssets.SwitchSfx, context.CommonAssets.SwitchSfx, switch2SpawnPosition
		);

		SpawnPlayer(context);
	}

	void Level_005::Update(Core::ApplicationContext& context, float /*deltaTime*/, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects)
	{
		auto isRedButtonPressed = m_RedButton->IsColliding(m_RedFlask->GetObjectID());
		auto isBlueButtonPressed = m_BlueButton->IsColliding(m_BlueFlask->GetObjectID());

		m_RedButton->SetToggled(isRedButtonPressed);
		m_BlueButton->SetToggled(isBlueButtonPressed);

		auto isPlayerOnSwitch1 = m_Switch1->IsColliding(context.Player.ObjectID);
		auto isPlayerOnSwitch2 = m_Switch2->IsColliding(context.Player.ObjectID);

		auto isAnyEchoOnSwitch1 = false;
		auto isAnyEchoOnSwitch2 = false;
		for (auto& echoObjectID : echoObjects)
		{
			if (m_Switch1->IsColliding(echoObjectID))
			{
				isAnyEchoOnSwitch1 = true;
			}
			if (m_Switch2->IsColliding(echoObjectID))
			{
				isAnyEchoOnSwitch2 = true;
			}
		}

		auto isSwitch1On = (isPlayerOnSwitch1 || isAnyEchoOnSwitch1);
		auto isSwitch2On = (isPlayerOnSwitch2 || isAnyEchoOnSwitch2);

		m_Switch1->SetToggled(isSwitch1On);
		m_Switch2->SetToggled(isSwitch2On);

		SetDoorState(context, (isRedButtonPressed && isBlueButtonPressed && isSwitch1On && isSwitch2On));
	}
	void Level_005::FixedUpdate(Core::ApplicationContext& /*context*/, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects)
	{
		m_RedFlask->FixedUpdate(echoObjects);
		m_BlueFlask->FixedUpdate(echoObjects);
	}
}
