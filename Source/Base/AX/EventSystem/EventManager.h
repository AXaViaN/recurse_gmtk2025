#ifndef _AX_EVENTSYSTEM_EVENTMANAGER
#define _AX_EVENTSYSTEM_EVENTMANAGER

#include <AX/EventSystem/Event.h>
#include <AX/Tool/Reporter.h>

#include <unordered_map>
#include <functional>
#include <vector>

namespace AX::EventSystem
{
	struct EventListener
	{
		friend class EventManager;
	private:
		size_t m_ID = 0;
		EventID m_EventID = 0;
		size_t m_GroupID = 0;
		std::function<void(Event&)> m_Callback;
	};

	struct EventListenerGroup
	{
		friend class EventManager;
	private:
		size_t m_ID = 0;
		size_t m_Order = SIZE_MAX;
	};

	class EventManager
	{
	public:
		template<typename T>
		inline static auto StartListener(std::function<void(T&)> callback) -> EventListener;
		template<typename T>
		inline static auto StartListener(std::function<void(T&)> callback, EventID eventID) -> EventListener;
		template<typename T>
		inline static auto StartListener(std::function<void(T&)> callback, const EventListenerGroup& listenerGroup) -> EventListener;
		template<typename T>
		inline static auto StartListener(std::function<void(T&)> callback, const EventListenerGroup& listenerGroup, EventID eventID) -> EventListener;

		static void StopListener(EventListener& listener);

		template<typename T>
		inline static void SendEvent(T event);

		static auto CreateListenerGroup() -> EventListenerGroup;
		static void SetListenerGroupOrder(EventListenerGroup& listenerGroup, const size_t& order);
		static void DestroyListenerGroup(EventListenerGroup& listenerGroup);

	private:
		inline static auto GetEventIDToListeners() -> std::unordered_map<size_t, std::vector<EventListener>>&;
		inline static auto GetListenerGroupMap() -> std::unordered_map<size_t, EventListenerGroup>&;

		inline static auto GetNextListenerID() -> size_t;
		inline static auto GetNextListenerGroupID() -> size_t;

		static void EmplaceByOrder(std::vector<EventListener>& listeners, const EventListener& listener, const EventListenerGroup& listenerGroup);
	};

	/***** IMPL *****/

	template<typename T>
	inline auto EventManager::StartListener(std::function<void(T&)> callback) -> EventListener
	{
		return StartListener(callback, {}, EventInfo<T>::GetBaseID());
	}
	template<typename T>
	inline auto EventManager::StartListener(std::function<void(T&)> callback, EventID eventID) -> EventListener
	{
		return StartListener(callback, {}, eventID);
	}
	template<typename T>
	inline auto EventManager::StartListener(std::function<void(T&)> callback, const EventListenerGroup& listenerGroup) -> EventListener
	{
		return StartListener(callback, listenerGroup, EventInfo<T>::GetBaseID());
	}
	template<typename T>
	inline auto EventManager::StartListener(std::function<void(T&)> callback, const EventListenerGroup& listenerGroup, EventID eventID) -> EventListener
	{
		auto listener = EventListener();
		listener.m_ID = GetNextListenerID();
		listener.m_EventID = eventID;
		listener.m_GroupID = listenerGroup.m_ID;
		listener.m_Callback = [callback](Event& event)
		{
			callback(static_cast<T&>(event));
		};

		auto& eventIDToListeners = GetEventIDToListeners();
		auto& listeners = eventIDToListeners[listener.m_EventID];
		EmplaceByOrder(listeners, listener, listenerGroup);

		return listener;
	}

	template<typename T>
	inline void EventManager::SendEvent(T event)
	{
		auto eventBaseID = EventInfo<T>::GetBaseID();
		auto eventID = EventInfo<T>::GetEventID(event);

		auto& eventIDToListeners = GetEventIDToListeners();
		auto listeners = eventIDToListeners[eventBaseID];
		if (eventID != eventBaseID)
		{
			auto& listenerGroupMap = GetListenerGroupMap();

			auto& eventListeners = eventIDToListeners[eventID];
			for (auto& listener : eventListeners)
			{
				auto& listenerGroup = listenerGroupMap.at(listener.m_GroupID);
				EmplaceByOrder(listeners, listener, listenerGroup);
			}
		}

		for (auto& listener : listeners)
		{
			listener.m_Callback(event);

			if (event.IsConsumed())
			{
				return;
			}
		}
	}

	inline auto EventManager::GetEventIDToListeners() -> std::unordered_map<size_t, std::vector<EventListener>>&
	{
		static auto eventIDToListeners = std::unordered_map<size_t, std::vector<EventListener>>();
		return eventIDToListeners;
	}
	inline auto EventManager::GetListenerGroupMap() -> std::unordered_map<size_t, EventListenerGroup>&
	{
		static auto eventIDToListeners = std::unordered_map<size_t, EventListenerGroup>{
			{ 0, EventListenerGroup{} }
		};
		return eventIDToListeners;
	}

	inline auto EventManager::GetNextListenerID() -> size_t
	{
		static auto nextListenerID = 1ull;
		return nextListenerID++;
	}
	inline auto EventManager::GetNextListenerGroupID() -> size_t
	{
		static auto nextListenerGroupID = 1ull;
		return nextListenerGroupID++;
	}
}

#endif // !_AX_EVENTSYSTEM_EVENTMANAGER
