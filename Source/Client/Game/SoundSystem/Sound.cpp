#include "SoundSystem/Sound.h"

#include <AX/EventSystem/EventManager.h>
#include "SoundSystem/Events.h"
#include "SoundSystem/SoundManager.h"

namespace Game::SoundSystem
{
	auto Sound::CreateAsset(const std::string& path) -> Veng::Asset::AssetMeta<Sound>
	{
		auto sound = Sound(path);
		auto asset = sound.m_Asset;
		sound.m_Asset = {};

		return asset;
	}

	Sound::Sound(const std::string& path)
	{
		m_Asset = Veng::Asset::AssetManager::CreateAsset(path, Sound());
		m_Asset->m_Asset = m_Asset;

		SoundManager::CreateSound(m_Asset, path);
	}

	void Sound::SetBaseVolume(float baseVolume)
	{
		m_BaseVolume = baseVolume;
		SetVolume(m_Volume);
	}
	void Sound::SetBasePitch(float basePitch)
	{
		m_BasePitch = basePitch;
		SetPitch(m_Pitch);
	}

	void Sound::Play()
	{
		SoundManager::Play(m_Asset);
	}
	void Sound::Stop()
	{
		SoundManager::Stop(m_Asset);
	}

	void Sound::MoveHead(float seconds)
	{
		SoundManager::MoveHead(m_Asset, seconds);
	}
	auto Sound::GetHeadPosition() -> float
	{
		return SoundManager::GetHeadPosition(m_Asset);
	}

	void Sound::Fade(float startVolume, float endVolume, float time)
	{
		auto event = FadeEvent();
		event.Sound = m_Asset;
		event.StartVolume = startVolume;
		event.EndVolume = endVolume;
		event.Time = time;
		AX::EventSystem::EventManager::SendEvent(event);
	}

	void Sound::SetIsLooping(bool isLooping)
	{
		SoundManager::SetIsLooping(m_Asset, isLooping);
	}
	void Sound::SetVolume(float volume)
	{
		m_Volume = volume;
		SoundManager::SetVolume(m_Asset, m_BaseVolume * m_Volume);
	}
	void Sound::SetPitch(float pitch)
	{
		m_Pitch = pitch;
		SoundManager::SetPitch(m_Asset, m_BasePitch * m_Pitch);
	}

	Sound::~Sound() noexcept
	{
		if (m_Asset)
		{
			SoundManager::RemoveSound(m_Asset);
		}
	}
}
