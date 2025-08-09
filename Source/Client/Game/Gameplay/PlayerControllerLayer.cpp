#include "Gameplay/PlayerControllerLayer.h"

#include "Core/ApplicationContext.h"
#include "Gameplay/Events.h"

namespace Game::Gameplay
{
	void PlayerControllerLayer::OnUpdate(float /*deltaTime*/)
	{
		auto& context = GetContext<Core::ApplicationContext>();

		if (context.Input.GetKeyDown(Veng::IO::Input::Key::Escape))
		{
			context.Window.Close();
		}
		if (context.Input.GetKeyDown(Veng::IO::Input::Key::R))
		{
			context.CommonAssets.WalkSfx->Fade(1.0f, 0.001f, 0.2f);
			AX::EventSystem::EventManager::SendEvent(LevelRestartEvent());
			return;
		}

		auto playerObject = context.Scene.GetObject(context.Player.ObjectID);
		playerObject->MaterialAsset->SetProperty("Time", context.Window.GetTime());

		auto wasMoving = glm::dot(m_PlayerMovement, m_PlayerMovement) > 0.0f;
		m_PlayerMovement = glm::vec2(0.0f);
		if (context.Input.GetKeyHold(Veng::IO::Input::Key::W))
		{
			m_PlayerMovement.y += 1.0f;
		}
		if (context.Input.GetKeyHold(Veng::IO::Input::Key::S))
		{
			m_PlayerMovement.y -= 1.0f;
		}
		if (context.Input.GetKeyHold(Veng::IO::Input::Key::D))
		{
			m_PlayerMovement.x += 1.0f;
		}
		if (context.Input.GetKeyHold(Veng::IO::Input::Key::A))
		{
			m_PlayerMovement.x -= 1.0f;
		}

		auto isMoving = glm::dot(m_PlayerMovement, m_PlayerMovement) > 0.0f;
		if (isMoving != wasMoving)
		{
			if (isMoving)
			{
				context.CommonAssets.WalkSfx->SetVolume(1.0f);
			}
			else
			{
				context.CommonAssets.WalkSfx->Fade(1.0f, 0.001f, 0.2f);
			}
		}
	}
	void PlayerControllerLayer::OnFixedUpdate()
	{
		auto playerMovementLength2 = glm::dot(m_PlayerMovement, m_PlayerMovement);
		if (playerMovementLength2 == 0)
		{
			AX::EventSystem::EventManager::SendEvent(PlayerMovementEvent(glm::vec2(0.0f)));
			return;
		}

		auto& context = GetContext<Core::ApplicationContext>();

		constexpr auto playerSpeed = 1.0f;
		m_PlayerMovement /= glm::sqrt(playerMovementLength2);
		m_PlayerMovement *= playerSpeed;

		auto playerObject = context.Scene.GetObject(context.Player.ObjectID);
		auto targetPosition = playerObject->Position + m_PlayerMovement * FixedDeltaTime;
		auto targetMapTilePosition = context.CurrentLevelInfo->GetTilePositionInMap(targetPosition);
		auto targetTopLeftMapTile = context.CurrentLevelInfo->GetTile(targetMapTilePosition);
		auto targetBottomRightMapTile = context.CurrentLevelInfo->GetTile(targetMapTilePosition + glm::vec2(1, 1));

		if (targetTopLeftMapTile == "Ground" && targetBottomRightMapTile == "Ground")
		{
			playerObject->Position = targetPosition;

			if (m_IsFirstMove)
			{
				m_IsFirstMove = false;

				AX::EventSystem::EventManager::SendEvent(LevelStartedEvent());
			}

			if (context.CurrentLevelInfo->IsInsideDoor(targetMapTilePosition))
			{
				context.CommonAssets.WalkSfx->Fade(1.0f, 0.001f, 0.2f);
				AX::EventSystem::EventManager::SendEvent(LevelCompletedEvent());
			}
		}
		else
		{
			m_PlayerMovement = glm::vec2(0.0f);
		}

		AX::EventSystem::EventManager::SendEvent(PlayerMovementEvent(m_PlayerMovement));
	}
}
