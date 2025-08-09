#ifndef _GAME_GAMEPLAY_EVENTS
#define _GAME_GAMEPLAY_EVENTS

#include <AX/EventSystem/Event.h>
#include "Gameplay/PlayerRecord.h"

#include <glm/glm.hpp>

namespace Game::Gameplay
{
	class LevelStartedEvent : public AX::EventSystem::Event
	{
	};
	class LevelEndedEvent : public AX::EventSystem::Event
	{
	};

	class LevelRestartEvent : public AX::EventSystem::Event
	{
	};
	class LevelCompletedEvent : public AX::EventSystem::Event
	{
	};

	class TransitionScreenCompletedEvent : public AX::EventSystem::Event
	{
	};

	class PlayerMovementEvent : public AX::EventSystem::Event
	{
	public:
		PlayerMovementEvent(const glm::vec2& movement) : Movement(movement) {}

		glm::vec2 Movement;
	};

	class PlayerRecordedEvent : public AX::EventSystem::Event
	{
	public:
		PlayerRecordedEvent(const PlayerRecord& record) : Record(record) {}

		PlayerRecord Record;
	};
}

#endif // !_GAME_GAMEPLAY_EVENTS
