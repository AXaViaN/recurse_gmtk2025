#include "Gameplay/PlayerEchoController.h"

namespace Game::Gameplay
{
	void PlayerEchoController::Reset()
	{
		m_EchoRecords.clear();
		m_EchoObjects.clear();
		for (auto& isEchoMoving : m_IsEchoMovings)
		{
			isEchoMoving = false;
		}
	}

	void PlayerEchoController::AddEcho(const Gameplay::PlayerRecord& record)
	{
		m_EchoRecords.emplace(m_EchoRecords.begin(), record);
		if (m_EchoRecords.size() > 3)
		{
			m_EchoRecords.pop_back();
		}

		m_EchoObjects.clear();

		m_EchoRecords[0].Steps.emplace_back(PlayerRecordStep
		{
			.Time = 0.0f,
			.Movement = glm::vec2(0.0f)
		});
	}

	void PlayerEchoController::Initialize(Core::ApplicationContext& context)
	{
		m_EchoObjects.clear();
		for (auto i = 0; i < m_EchoRecords.size(); ++i)
		{
			auto& echoRecord = m_EchoRecords[i];
			echoRecord.CurrentStep = 0;

			auto& echoObjectID = m_EchoObjects.emplace_back(context.Scene.CreateDynamicObject());
			auto echoObject = context.Scene.GetObject(echoObjectID);
			echoObject->MaterialAsset = context.CommonAssets.PlayerEchoMaterialAssets[i];
			echoObject->Scale = context.Player.Scale;
			echoObject->Position = context.CurrentLevelInfo->GetTilePositionInScene(
				context.CurrentLevelInfo->PlayerSpawnPosition
			);
			echoObject->AdditiveAlphaBlending = true;

			m_IsEchoMovings[i] = false;
		}
	}

	void PlayerEchoController::Update(Core::ApplicationContext& context, float deltaTime)
	{
		for (auto i = 0; i < m_EchoObjects.size(); ++i)
		{
			auto& echoRecord = m_EchoRecords[i];
			auto& echoObjectID = m_EchoObjects[i];

			while (echoRecord.CurrentStep < echoRecord.Steps.size() &&
				   echoRecord.Steps[echoRecord.CurrentStep].Time >= context.CurrentLevelInfo->LevelTimeRemaining)
			{
				++echoRecord.CurrentStep;
			}
			--echoRecord.CurrentStep;

			auto echoObject = context.Scene.GetObject(echoObjectID);
			auto movement = echoRecord.Steps[echoRecord.CurrentStep].Movement;
			echoObject->Position += movement * deltaTime;

			auto isMoving = (glm::dot(movement, movement) > 0.0f);
			if (isMoving != m_IsEchoMovings[i])
			{
				m_IsEchoMovings[i] = isMoving;
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
	}

	void PlayerEchoController::RewindInitialize(Core::ApplicationContext& context)
	{
		auto playerObject = context.Scene.GetObject(context.Player.ObjectID);

		for (auto i = 0; i < m_EchoRecords.size(); ++i)
		{
			auto& echoRecord = m_EchoRecords[i];
			echoRecord.CurrentStep = echoRecord.Steps.size() - 1;

			auto echoObject = playerObject;
			if (i > 0)
			{
				auto& echoObjectID = m_EchoObjects[i - 1];
				echoObject = context.Scene.GetObject(echoObjectID);
			}

			echoObject->Position = context.CurrentLevelInfo->GetTilePositionInScene(
				context.CurrentLevelInfo->PlayerSpawnPosition
			);

			for (auto stepIdx = 0; stepIdx < echoRecord.Steps.size(); ++stepIdx)
			{
				auto& step = echoRecord.Steps[stepIdx];

				auto stepEndTime = context.CurrentLevelInfo->LevelTime;
				if ((stepIdx + 1) < echoRecord.Steps.size())
				{
					auto& nextStep = echoRecord.Steps[stepIdx + 1];
					stepEndTime = nextStep.Time;
				}

				auto deltaTime = step.Time - stepEndTime;
				echoObject->Position += step.Movement * deltaTime;
			}
		}

		auto lastEchoObject = context.Scene.GetObject(m_EchoObjects.back());
		lastEchoObject->Position = glm::vec2(1000.0f);
	}
	void PlayerEchoController::RewindFinalize(Core::ApplicationContext& context)
	{
		auto playerObject = context.Scene.GetObject(context.Player.ObjectID);
		playerObject->Position = context.CurrentLevelInfo->GetTilePositionInScene(
			context.CurrentLevelInfo->PlayerSpawnPosition
		);

		for (auto i = 0; i < m_EchoRecords.size(); ++i)
		{
			auto& echoRecord = m_EchoRecords[i];
			echoRecord.CurrentStep = 0;

			auto& echoObjectID = m_EchoObjects[i];
			auto echoObject = context.Scene.GetObject(echoObjectID);
			echoObject->Position = context.CurrentLevelInfo->GetTilePositionInScene(
				context.CurrentLevelInfo->PlayerSpawnPosition
			);
		}
	}
	void PlayerEchoController::RewindUpdate(Core::ApplicationContext& context, float timeRemaining, float deltaTime)
	{
		auto playerObject = context.Scene.GetObject(context.Player.ObjectID);

		for (auto i = 0; i < m_EchoObjects.size(); ++i)
		{
			auto& echoRecord = m_EchoRecords[i];

			auto nextStep = echoRecord.CurrentStep;
			while (nextStep > 0 &&
				   echoRecord.Steps[nextStep].Time < timeRemaining)
			{
				--nextStep;
			}

			auto echoObject = playerObject;
			if (i > 0)
			{
				auto& echoObjectID = m_EchoObjects[i - 1];
				echoObject = context.Scene.GetObject(echoObjectID);
			}

			while (echoRecord.CurrentStep > nextStep)
			{
				auto delta = timeRemaining - echoRecord.Steps[echoRecord.CurrentStep].Time;
				echoObject->Position -= echoRecord.Steps[echoRecord.CurrentStep].Movement * delta;

				deltaTime -= delta;
				--echoRecord.CurrentStep;
			}

			echoObject->Position -= echoRecord.Steps[echoRecord.CurrentStep].Movement * deltaTime;
		}
	}
}
