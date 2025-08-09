#include "SoundSystem/SoundManager.h"

#include <AX/Tool/PathManager.h>
#include <AX/Tool/Reporter.h>

#include <miniaudio/miniaudio.h>

#include <unordered_map>

namespace Game::SoundSystem
{
	class SoundManagerHelper
	{
	public:
		ma_engine SoundEngine;
		std::unordered_map<Veng::Asset::AssetMeta<Sound>, ma_sound> SoundAssetDataMap;
	};

	static SoundManagerHelper s_Helper;

	/***** CLASS IMPL *****/

	void SoundManager::Initialize()
	{
		auto result = ma_engine_init(nullptr, &s_Helper.SoundEngine);
		AX_Assert(result == MA_SUCCESS);
	}
	void SoundManager::Terminate()
	{
		for (auto& asset_sound : s_Helper.SoundAssetDataMap)
		{
			auto soundAsset = asset_sound.first;
			Veng::Asset::AssetManager::RemoveAsset(soundAsset);
		}
		s_Helper.SoundAssetDataMap.clear();

		ma_engine_uninit(&s_Helper.SoundEngine);
	}

	void SoundManager::CreateSound(const Veng::Asset::AssetMeta<Sound>& soundAsset, const std::string& path)
	{
		s_Helper.SoundAssetDataMap[soundAsset] = ma_sound();
		auto soundPtr = &s_Helper.SoundAssetDataMap[soundAsset];

		auto result = ma_sound_init_from_file(
			&s_Helper.SoundEngine, AX::Tool::PathManager::GetFullPath(path).c_str(),
			MA_SOUND_FLAG_DECODE, nullptr, nullptr, soundPtr
		);
		AX_Assert(result == MA_SUCCESS);
	}
	void SoundManager::RemoveSound(const Veng::Asset::AssetMeta<Sound>& soundAsset)
	{
		AX_Assert(s_Helper.SoundAssetDataMap.contains(soundAsset));
		ma_sound_uninit(&s_Helper.SoundAssetDataMap[soundAsset]);
		s_Helper.SoundAssetDataMap.erase(soundAsset);
	}

	void SoundManager::Play(const Veng::Asset::AssetMeta<Sound>& soundAsset)
	{
		AX_Assert(s_Helper.SoundAssetDataMap.contains(soundAsset));

		auto soundPtr = &s_Helper.SoundAssetDataMap[soundAsset];
		ma_sound_seek_to_second(soundPtr, 0.0f);
		ma_sound_start(soundPtr);
	}
	void SoundManager::Stop(const Veng::Asset::AssetMeta<Sound>& soundAsset)
	{
		AX_Assert(s_Helper.SoundAssetDataMap.contains(soundAsset));
		ma_sound_stop(&s_Helper.SoundAssetDataMap[soundAsset]);
	}

	void SoundManager::MoveHead(const Veng::Asset::AssetMeta<Sound>& soundAsset, float seconds)
	{
		AX_Assert(s_Helper.SoundAssetDataMap.contains(soundAsset));
		ma_sound_seek_to_second(&s_Helper.SoundAssetDataMap[soundAsset], seconds);
	}
	auto SoundManager::GetHeadPosition(const Veng::Asset::AssetMeta<Sound>& soundAsset) -> float
	{
		AX_Assert(s_Helper.SoundAssetDataMap.contains(soundAsset));

		auto seconds = 0.0f;
		ma_sound_get_cursor_in_seconds(&s_Helper.SoundAssetDataMap[soundAsset], &seconds);
		return seconds;
	}

	void SoundManager::SetIsLooping(const Veng::Asset::AssetMeta<Sound>& soundAsset, bool isLooping)
	{
		AX_Assert(s_Helper.SoundAssetDataMap.contains(soundAsset));
		ma_sound_set_looping(&s_Helper.SoundAssetDataMap[soundAsset], isLooping);
	}
	void SoundManager::SetVolume(const Veng::Asset::AssetMeta<Sound>& soundAsset, float volume)
	{
		AX_Assert(s_Helper.SoundAssetDataMap.contains(soundAsset));
		ma_sound_set_volume(&s_Helper.SoundAssetDataMap[soundAsset], volume);
	}
	void SoundManager::SetPitch(const Veng::Asset::AssetMeta<Sound>& soundAsset, float pitch)
	{
		AX_Assert(s_Helper.SoundAssetDataMap.contains(soundAsset));
		ma_sound_set_pitch(&s_Helper.SoundAssetDataMap[soundAsset], pitch);
	}
}
