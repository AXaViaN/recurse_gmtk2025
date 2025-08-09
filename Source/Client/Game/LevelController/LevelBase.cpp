#include "LevelController/LevelBase.h"

#include "LevelController/Level_001.h"
#include "LevelController/Level_002.h"
#include "LevelController/Level_003.h"
#include "LevelController/Level_004.h"
#include "LevelController/Level_005.h"

namespace Game::LevelController
{
	auto LevelBase::CreateLevelList() -> std::vector<std::unique_ptr<LevelController::LevelBase>>
	{
		auto levelList = std::vector<std::unique_ptr<LevelController::LevelBase>>();
		levelList.emplace_back(std::make_unique<Level_001>());
		levelList.emplace_back(std::make_unique<Level_002>());
		levelList.emplace_back(std::make_unique<Level_003>());
		levelList.emplace_back(std::make_unique<Level_004>());
		levelList.emplace_back(std::make_unique<Level_005>());
		return levelList;
	}

	void LevelBase::SpawnPlayer(Core::ApplicationContext& context)
	{
		context.Player.ObjectID = context.Scene.CreateDynamicObject();
		auto playerObject = context.Scene.GetObject(context.Player.ObjectID);
		playerObject->MaterialAsset = context.CommonAssets.PlayerMaterialAsset;
		playerObject->Scale = context.Player.Scale;
		playerObject->Position = context.CurrentLevelInfo->GetTilePositionInScene(context.CurrentLevelInfo->PlayerSpawnPosition);
		playerObject->AdditiveAlphaBlending = true;
	}
	void LevelBase::SetDoorState(Core::ApplicationContext& context, bool isOpen)
	{
		if (isOpen == m_IsDoorOpen)
		{
			return;
		}
		m_IsDoorOpen = isOpen;

		context.CurrentLevelInfo->SetIsDoorWalkable(m_IsDoorOpen);

		auto sfx = context.CommonAssets.DoorCloseSfx;
		if (m_IsDoorOpen)
		{
			sfx = context.CommonAssets.DoorOpenSfx;
		}
		sfx->Play();
	}
}
