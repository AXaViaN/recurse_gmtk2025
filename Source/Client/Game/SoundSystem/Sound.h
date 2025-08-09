#ifndef _GAME_SOUNDSYSTEM_SOUND
#define _GAME_SOUNDSYSTEM_SOUND

#include <AX/Tool/Macro.h>
#include <Veng/Asset/AssetManager.h>

namespace Game::SoundSystem
{
	class Sound
	{
	public:
		static auto CreateAsset(const std::string& path) -> Veng::Asset::AssetMeta<Sound>;

	public:
		Sound(const std::string& path);

		void SetBaseVolume(float baseVolume);
		void SetBasePitch(float basePitch);

		void Play();
		void Stop();

		void MoveHead(float seconds);
		auto GetHeadPosition() -> float;

		void Fade(float startVolume, float endVolume, float time);

		void SetIsLooping(bool isLooping);
		void SetVolume(float volume);
		void SetPitch(float pitch);

	public:
		~Sound() noexcept;
		AX_SemanticsDeleteCopy(Sound)
		AX_SemanticsDefaultMove(Sound)

	private:
		Sound() = default;

	private:
		Veng::Asset::AssetMeta<Sound> m_Asset;

		float m_BaseVolume = 1.0f;
		float m_BasePitch = 1.0f;
		float m_Volume = 1.0f;
		float m_Pitch = 1.0f;
	};
}

#endif // !_GAME_SOUNDSYSTEM_SOUND
