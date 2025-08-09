#ifndef _GAME_SOUNDSYSTEM_SOUNDMANAGER
#define _GAME_SOUNDSYSTEM_SOUNDMANAGER

#include "SoundSystem/Sound.h"

#include <string>

namespace Game::SoundSystem
{
	class SoundManager
	{
	public:
		static void Initialize();
		static void Terminate();

		static void CreateSound(const Veng::Asset::AssetMeta<Sound>& soundAsset, const std::string& path);
		static void RemoveSound(const Veng::Asset::AssetMeta<Sound>& soundAsset);

		static void Play(const Veng::Asset::AssetMeta<Sound>& soundAsset);
		static void Stop(const Veng::Asset::AssetMeta<Sound>& soundAsset);

		static void MoveHead(const Veng::Asset::AssetMeta<Sound>& soundAsset, float seconds);
		static auto GetHeadPosition(const Veng::Asset::AssetMeta<Sound>& soundAsset) -> float;

		static void SetIsLooping(const Veng::Asset::AssetMeta<Sound>& soundAsset, bool isLooping);
		static void SetVolume(const Veng::Asset::AssetMeta<Sound>& soundAsset, float volume);
		static void SetPitch(const Veng::Asset::AssetMeta<Sound>& soundAsset, float pitch);
	};
}

#endif // !_GAME_SOUNDSYSTEM_SOUNDMANAGER
