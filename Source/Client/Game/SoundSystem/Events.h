#ifndef _GAME_SOUNDSYSTEM_EVENTS
#define _GAME_SOUNDSYSTEM_EVENTS

#include <AX/EventSystem/Event.h>
#include "SoundSystem/Sound.h"

namespace Game::SoundSystem
{
	class FadeEvent : public AX::EventSystem::Event
	{
	public:
		Veng::Asset::AssetMeta<SoundSystem::Sound> Sound;
		float StartVolume;
		float EndVolume;
		float Time;
	};
}

#endif // !_GAME_SOUNDSYSTEM_EVENTS
