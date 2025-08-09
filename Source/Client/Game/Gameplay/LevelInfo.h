#ifndef _GAME_GAMEPLAY_LEVELINFO
#define _GAME_GAMEPLAY_LEVELINFO

#include <AX/ID.h>
#include "SceneSystem/Object.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

namespace Game::Core
{
	class ApplicationContext;
}

namespace Game::Gameplay
{
	class LevelInfo
	{
	public:
		LevelInfo(
			Core::ApplicationContext& context,
			const std::vector<std::vector<std::string>>& map,
			float tileSize,
			const glm::vec2& playerSpawnPosition,
			float levelTime
		);

		void SpawnMap();

		auto GetTilePositionInScene(const glm::vec2& tilePosition) -> glm::vec2;
		auto GetTilePositionInMap(const glm::vec2& scenePosition) -> glm::vec2;

		auto GetTile(const glm::vec2& tilePosition) -> const std::string&;
		auto GetTileSceneObject(const glm::vec2& tilePosition) -> const AX::ID<SceneSystem::Object>&;

		void SetTileVariation(const glm::vec2& tilePosition, const std::string& variationSuffix);
		void SetDoorFrameVariation(const std::string& variationSuffix);

		void SetIsDoorWalkable(bool isDoorWalkable);
		auto IsInsideDoor(const glm::vec2& tilePosition) -> bool;

	public:
		std::vector<std::vector<std::string>> Map;
		glm::vec2 MapSize;
		float TileSize;

		glm::vec2 PlayerSpawnPosition;
		glm::vec2 DoorFramePosition;

		glm::vec2 TopLeftPositionInScene;

		std::vector<std::vector<AX::ID<SceneSystem::Object>>> SceneObjects;

		float LevelTime;
		float LevelTimeRemaining;

	private:
		Core::ApplicationContext& m_Context;
		bool m_IsDoorWalkable = false;
	};
}

#endif // !_GAME_GAMEPLAY_LEVELINFO
