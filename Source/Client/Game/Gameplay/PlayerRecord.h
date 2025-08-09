#ifndef _GAME_GAMEPLAY_PLAYERRECORD
#define _GAME_GAMEPLAY_PLAYERRECORD

#include <glm/glm.hpp>

#include <vector>

namespace Game::Gameplay
{
	struct PlayerRecordStep
	{
		float Time;
		glm::vec2 Movement;
	};

	struct PlayerRecord
	{
		std::vector<PlayerRecordStep> Steps;
		size_t CurrentStep;
	};
}

#endif // !_GAME_GAMEPLAY_PLAYERRECORD
