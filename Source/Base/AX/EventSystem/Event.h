#ifndef _AX_EVENTSYSTEM_EVENT
#define _AX_EVENTSYSTEM_EVENT

#include <AX/Tool/Macro.h>

namespace AX::EventSystem
{
	// This is used as a composite ID.
	// auto eventID = (
	//		(event.GetCustomID() << 32) | EventInfo<EventType>::GetBaseID()
	// );
	using EventID = size_t;
	using EventBaseID = size_t;
	using EventCustomID = size_t;

	struct EventInfoHelper
	{
		inline static auto GetNextBaseID() -> EventCustomID;
	};

	template<typename T>
	struct EventInfo
	{
		inline static auto GetBaseID() -> EventBaseID;
		inline static auto GetEventID(const T& reference) -> EventID;
	};

	class Event
	{
	public:
		inline virtual auto GetCustomID() const -> EventCustomID;

		inline void Consume();
		inline auto IsConsumed() const;

	public:
		virtual ~Event() noexcept = default;
		AX_SemanticsDefaultAll(Event)

	protected:
		Event() = default;

	private:
		bool m_IsConsumed = false;
	};

	/***** IMPL *****/

	inline auto EventInfoHelper::GetNextBaseID() -> EventBaseID
	{
		static auto nextID = static_cast<EventBaseID>(1);
		return nextID++;
	}

	template<typename T>
	inline auto EventInfo<T>::GetBaseID() -> EventBaseID
	{
		static auto id = EventInfoHelper::GetNextBaseID();
		return id;
	}
	template<typename T>
	inline auto EventInfo<T>::GetEventID(const T& reference) -> EventID
	{
		return ((reference.GetCustomID() << 32) | GetBaseID());
	}

	inline auto Event::GetCustomID() const -> EventCustomID
	{
		return 0;
	}

	inline void Event::Consume()
	{
		m_IsConsumed = true;
	}
	inline auto Event::IsConsumed() const
	{
		return m_IsConsumed;
	}
}

#endif // _AX_EVENTSYSTEM_EVENT
