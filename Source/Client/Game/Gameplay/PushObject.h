#ifndef _GAME_GAMEPLAY_PUSHOBJECT
#define _GAME_GAMEPLAY_PUSHOBJECT

#include <AX/ID.h>
#include "Core/ApplicationContext.h"

namespace Game::Gameplay
{
	class PushObject
	{
	public:
		PushObject(Core::ApplicationContext& context, const std::string& sprite, const glm::vec2& spawnPosition);

		void FixedUpdate(const std::vector<AX::ID<SceneSystem::Object>>& echoObjects);

		inline auto GetObjectID() const -> const AX::ID<SceneSystem::Object>&;

	private:
		Core::ApplicationContext& m_Context;

		AX::ID<SceneSystem::Object> m_ObjectID;

		bool m_IsBeingPushed = false;
	};

	/***** IMPL *****/

	inline auto PushObject::GetObjectID() const -> const AX::ID<SceneSystem::Object>&
	{
		return m_ObjectID;
	}
}

#endif // !_GAME_GAMEPLAY_PUSHOBJECT
