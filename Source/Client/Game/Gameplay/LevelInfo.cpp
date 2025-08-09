#include "Gameplay/LevelInfo.h"

#include "Core/ApplicationContext.h"

namespace Game::Gameplay
{
	LevelInfo::LevelInfo(
		Core::ApplicationContext& context,
		const std::vector<std::vector<std::string>>& map,
		float tileSize,
		const glm::vec2& playerSpawnPosition,
		float levelTime
	) :
		Map(map),
		MapSize(Map[0].size(), Map.size()),
		TileSize(tileSize),
		PlayerSpawnPosition(playerSpawnPosition),
		TopLeftPositionInScene(glm::vec2(-MapSize.x, MapSize.y) / 2.0f),
		LevelTime(levelTime),
		LevelTimeRemaining(levelTime),
		m_Context(context)
	{
		auto isDoorFrameFound = false;
		for (auto mapY = 0; mapY < Map.size(); ++mapY)
		{
			for (auto mapX = 0; mapX < Map[0].size(); ++mapX)
			{
				if (Map[mapY][mapX] == "Door10")
				{
					DoorFramePosition = glm::vec2(mapX, mapY);

					isDoorFrameFound = true;
					break;
				}
			}

			if (isDoorFrameFound)
			{
				break;
			}
		}
	}

	void LevelInfo::SpawnMap()
	{
		auto mapTilePosition = TopLeftPositionInScene;
		for (auto mapY = 0; mapY < MapSize.y; ++mapY)
		{
			auto& mapRow = Map[mapY];
			auto& mapObjectsRow = SceneObjects.emplace_back();
			auto isOverlayRow = (mapY >= MapSize.y - 2);

			mapTilePosition.x = TopLeftPositionInScene.x;
			for (auto& mapTile : mapRow)
			{
				if (mapTile != "")
				{
					auto tileObjectID = AX::ID<SceneSystem::Object>();
					if (isOverlayRow)
					{
						tileObjectID = m_Context.Scene.CreateOverlayObject();
					}
					else
					{
						tileObjectID = m_Context.Scene.CreateBackgroundObject();
					}

					auto tileObject = m_Context.Scene.GetObject(tileObjectID);
					tileObject->Scale = glm::vec2(TileSize);
					tileObject->Position = mapTilePosition * TileSize;
					tileObject->MaterialAsset = m_Context.CommonAssets.DungeonTileAtlas->GetSpriteMaterial(mapTile);

					if (isOverlayRow)
					{
						tileObject->Position.y += 1.0f * TileSize;
					}

					mapObjectsRow.emplace_back(tileObjectID);
				}
				else
				{
					mapObjectsRow.emplace_back();
				}

				++mapTilePosition.x;
			}

			--mapTilePosition.y;
		}
	}

	auto LevelInfo::GetTilePositionInScene(const glm::vec2& tilePosition) -> glm::vec2
	{
		return (TopLeftPositionInScene + glm::vec2(tilePosition.x, -tilePosition.y)) * TileSize;
	}
	auto LevelInfo::GetTilePositionInMap(const glm::vec2& scenePosition) -> glm::vec2
	{
		auto tilePositionFlipped = (scenePosition / TileSize) - TopLeftPositionInScene;
		return glm::vec2(tilePositionFlipped.x, -tilePositionFlipped.y);
	}

	auto LevelInfo::GetTile(const glm::vec2& tilePosition) -> const std::string&
	{
		auto tilePositionNormalized = glm::ivec2(tilePosition);

		if (tilePositionNormalized.x < 0 || tilePositionNormalized.x >= MapSize.x ||
			tilePositionNormalized.y < 0 || tilePositionNormalized.y >= MapSize.y)
		{
			static auto emptyStr = std::string("");
			return emptyStr;
		}

		return Map[tilePositionNormalized.y][tilePositionNormalized.x];
	}
	auto LevelInfo::GetTileSceneObject(const glm::vec2& tilePosition) -> const AX::ID<SceneSystem::Object>&
	{
		auto tilePositionNormalized = glm::ivec2(tilePosition);

		if (tilePositionNormalized.x < 0 || tilePositionNormalized.x >= MapSize.x ||
			tilePositionNormalized.y < 0 || tilePositionNormalized.y >= MapSize.y)
		{
			static auto emptyID = AX::ID<SceneSystem::Object>();
			return emptyID;
		}

		return SceneObjects[tilePositionNormalized.y][tilePositionNormalized.x];
	}

	void LevelInfo::SetTileVariation(const glm::vec2& tilePosition, const std::string& variationSuffix)
	{
		auto& mapDoorFrameObjectID = GetTileSceneObject(tilePosition);
		auto mapDoorFrameObject = m_Context.Scene.GetObject(mapDoorFrameObjectID);
		mapDoorFrameObject->MaterialAsset = m_Context.CommonAssets.DungeonTileAtlas->GetSpriteMaterial(
			GetTile(tilePosition) + variationSuffix
		);
	}
	void LevelInfo::SetDoorFrameVariation(const std::string& variationSuffix)
	{
		auto tilePosition = DoorFramePosition;
		SetTileVariation(tilePosition, variationSuffix);

		++tilePosition.x;
		SetTileVariation(tilePosition, variationSuffix);

		--tilePosition.x;
		--tilePosition.y;
		SetTileVariation(tilePosition, variationSuffix);

		++tilePosition.x;
		SetTileVariation(tilePosition, variationSuffix);
	}

	void LevelInfo::SetIsDoorWalkable(bool isDoorWalkable)
	{
		if (isDoorWalkable == m_IsDoorWalkable)
		{
			return;
		}
		m_IsDoorWalkable = isDoorWalkable;

		auto doorFramePositionNormalized = glm::ivec2(DoorFramePosition);
		if (isDoorWalkable)
		{
			SetDoorFrameVariation("_Open");
			
			Map[doorFramePositionNormalized.y][doorFramePositionNormalized.x] = "Ground";
		}
		else
		{
			Map[doorFramePositionNormalized.y][doorFramePositionNormalized.x] = "Door10";

			SetDoorFrameVariation("");
		}
	}
	auto LevelInfo::IsInsideDoor(const glm::vec2& tilePosition) -> bool
	{
		auto tilePositionNormalized = glm::ivec2(tilePosition);
		auto doorFramePositionNormalized = glm::ivec2(DoorFramePosition);

		return (
			tilePositionNormalized == doorFramePositionNormalized ||
			tilePositionNormalized == (doorFramePositionNormalized + glm::ivec2(1, 0))
		);
	}
}
