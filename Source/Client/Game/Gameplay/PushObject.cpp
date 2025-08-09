#include "Gameplay/PushObject.h"

#include <Veng/LayerSystem/Layer.h>

namespace Game::Gameplay
{
	class PushObjectHelper
	{
	public:
		auto HandleCollision(Core::ApplicationContext& context, SceneSystem::Object* pushObject, SceneSystem::Object* otherObject) -> bool;
	};

	static PushObjectHelper s_Helper;

	/***** CLASS IMPL *****/

	PushObject::PushObject(Core::ApplicationContext& context, const std::string& sprite, const glm::vec2& spawnPosition) :
		m_Context(context),
		m_ObjectID(context.Scene.CreateDynamicObject())
	{
		auto pushObject = m_Context.Scene.GetObject(m_ObjectID);
		pushObject->Scale = glm::vec2(m_Context.CurrentLevelInfo->TileSize);
		pushObject->Position = m_Context.CurrentLevelInfo->GetTilePositionInScene(spawnPosition);
		pushObject->MaterialAsset = m_Context.CommonAssets.DungeonTileAtlas->GetSpriteMaterial(sprite);
	}

	void PushObject::FixedUpdate(const std::vector<AX::ID<SceneSystem::Object>>& echoObjects)
	{
		auto pushObject = m_Context.Scene.GetObject(m_ObjectID);

		auto isBeingPushed = false;
		for (auto echoIdx = static_cast<int>(echoObjects.size()) - 1; echoIdx >= 0; --echoIdx)
		{
			auto echoObject = m_Context.Scene.GetObject(echoObjects[echoIdx]);
			auto isPushed = s_Helper.HandleCollision(m_Context, pushObject, echoObject);

			isBeingPushed = isBeingPushed || isPushed;
		}

		auto playerObject = m_Context.Scene.GetObject(m_Context.Player.ObjectID);
		auto isPushed = s_Helper.HandleCollision(m_Context, pushObject, playerObject);

		isBeingPushed = isBeingPushed || isPushed;
		if (isBeingPushed != m_IsBeingPushed)
		{
			m_IsBeingPushed = isBeingPushed;
			if (m_IsBeingPushed)
			{
				m_Context.CommonAssets.SlideSfx->SetVolume(1.0f);
			}
			else
			{
				m_Context.CommonAssets.SlideSfx->Fade(1.0f, 0.001f, 0.2f);
			}
		}
	}

	/***** HELPER IMPL *****/

	auto PushObjectHelper::HandleCollision(Core::ApplicationContext& context, SceneSystem::Object* pushObject, SceneSystem::Object* otherObject) -> bool
	{
		auto distance = glm::distance(pushObject->Position, otherObject->Position);
		auto minDistance = (0.75f * context.CurrentLevelInfo->TileSize);

		auto collisionDistance = minDistance - distance;
		if (collisionDistance < 0.0f)
		{
			return false;
		}

		auto avoidanceDirection = glm::normalize(pushObject->Position - otherObject->Position);
		auto avoidanceMovement = avoidanceDirection * Veng::LayerSystem::Layer::FixedDeltaTime;

		auto targetPosition = pushObject->Position + avoidanceMovement;
		auto targetMapTilePosition = context.CurrentLevelInfo->GetTilePositionInMap(targetPosition);
		auto targetTopLeftMapTile = context.CurrentLevelInfo->GetTile(targetMapTilePosition);
		auto targetBottomRightMapTile = context.CurrentLevelInfo->GetTile(targetMapTilePosition + glm::vec2(1, 1));

		if (targetTopLeftMapTile == "Ground" && targetBottomRightMapTile == "Ground")
		{
			pushObject->Position = targetPosition;
			return true;
		}

		return false;
	}
}
