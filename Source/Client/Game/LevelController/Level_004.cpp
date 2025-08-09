#include "LevelController/Level_004.h"

namespace Game::LevelController
{
	void Level_004::Initialize(Core::ApplicationContext& context)
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
		auto levelTime = 5.0f;
		auto playerSpawnPosition = glm::vec2(4.5f, 7);
		auto redButtonSpawnPosition = glm::vec2(7, 4);
		auto redFlaskSpawnPosition = glm::vec2(7, 6);
		auto blueButtonSpawnPosition = glm::vec2(2, 6);
		auto blueFlaskSpawnPosition = glm::vec2(2, 4);

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

		SpawnPlayer(context);
	}

	void Level_004::Update(Core::ApplicationContext& context, float /*deltaTime*/, const std::vector<AX::ID<SceneSystem::Object>>& /*echoObjects*/)
	{
		auto isRedButtonPressed = m_RedButton->IsColliding(m_RedFlask->GetObjectID());
		auto isBlueButtonPressed = m_BlueButton->IsColliding(m_BlueFlask->GetObjectID());

		m_RedButton->SetToggled(isRedButtonPressed);
		m_BlueButton->SetToggled(isBlueButtonPressed);

		SetDoorState(context, (isRedButtonPressed && isBlueButtonPressed));
	}
	void Level_004::FixedUpdate(Core::ApplicationContext& /*context*/, const std::vector<AX::ID<SceneSystem::Object>>& echoObjects)
	{
		m_RedFlask->FixedUpdate(echoObjects);
		m_BlueFlask->FixedUpdate(echoObjects);
	}
}
