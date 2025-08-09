#include <AX/EventSystem/EventManager.h>

#include <algorithm>

namespace AX::EventSystem
{
	void EventManager::StopListener(EventListener& listener)
	{
		if (listener.m_ID == 0)
		{
			return;
		}

		auto& eventIDToListeners = GetEventIDToListeners();
		auto& listeners = eventIDToListeners[listener.m_EventID];
		for (auto i = 0; i < listeners.size(); ++i)
		{
			if (listeners[i].m_ID == listener.m_ID)
			{
				// TODO_ASC: Protect m_EventIDToListeners from modifying while SendEvent() is running
				listeners.erase(listeners.begin() + i);
				break;
			}
		}

		listener = EventListener();
	}

	auto EventManager::CreateListenerGroup() -> EventListenerGroup
	{
		auto listenerGroup = EventListenerGroup();
		listenerGroup.m_ID = GetNextListenerGroupID();

		auto& listenerGroupMap = GetListenerGroupMap();
		listenerGroupMap.emplace(listenerGroup.m_ID, listenerGroup);

		return listenerGroup;
	}
	void EventManager::SetListenerGroupOrder(EventListenerGroup& listenerGroup, const size_t& order)
	{
		if (listenerGroup.m_ID == 0 ||
			order == listenerGroup.m_Order)
		{
			return;
		}

		auto& listenerGroupMap = GetListenerGroupMap();
		AX_Assert(listenerGroupMap.contains(listenerGroup.m_ID));

		auto& listenerGroupRef = listenerGroupMap.at(listenerGroup.m_ID);
		listenerGroupRef.m_Order = order;

		auto& eventIDToListeners = GetEventIDToListeners();
		for (auto& eventID_listeners : eventIDToListeners)
		{
			auto& listeners = eventID_listeners.second;
			std::sort(listeners.begin(), listeners.end(), [&](EventListener& lhs, EventListener& rhs)
			{
				auto& lhsListenerGroup = listenerGroupMap.at(lhs.m_GroupID);
				auto& rhsListenerGroup = listenerGroupMap.at(rhs.m_GroupID);
				return (lhsListenerGroup.m_Order < rhsListenerGroup.m_Order);
			});
		}

		listenerGroup = listenerGroupRef;
	}
	void EventManager::DestroyListenerGroup(EventListenerGroup& listenerGroup)
	{
		if (listenerGroup.m_ID == 0)
		{
			return;
		}

		auto& listenerGroupMap = GetListenerGroupMap();
		AX_Assert(listenerGroupMap.contains(listenerGroup.m_ID));

		listenerGroupMap.erase(listenerGroup.m_ID);

		auto& eventIDToListeners = GetEventIDToListeners();
		for (auto& eventID_listeners : eventIDToListeners)
		{
			auto& listeners = eventID_listeners.second;
			for (auto i = 0; i < listeners.size(); ++i)
			{
				if (listeners.at(i).m_GroupID == listenerGroup.m_ID)
				{
					listeners.erase(listeners.begin() + i);
					--i;
				}
			}
		}

		listenerGroup = EventListenerGroup();
	}

	void EventManager::EmplaceByOrder(std::vector<EventListener>& listeners, const EventListener& listener, const EventListenerGroup& listenerGroup)
	{
		auto& listenerGroupMap = GetListenerGroupMap();

		auto placementIdx = listeners.size();
		for (auto i = 0; i < listeners.size(); ++i)
		{
			auto currentGroupID = listeners.at(i).m_GroupID;
			if (listenerGroup.m_Order < listenerGroupMap.at(currentGroupID).m_Order)
			{
				placementIdx = i;
				break;
			}
		}
		listeners.emplace(listeners.begin() + placementIdx, listener);
	}
}
