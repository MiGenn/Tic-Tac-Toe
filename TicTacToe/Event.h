#pragma once
#include <unordered_map>
#include <stdexcept>
#include "EventRaiser.h"

template<class... EventHandlerParams>
class Event final
{
public:
	using EventHandler = std::function<void(EventHandlerParams...)>;
	using EventHandlerID = unsigned long;

	Event(EventRaiser<EventHandlerParams...>& outputEventRaiser) noexcept
	{
		outputEventRaiser = EventRaiser<EventHandlerParams...>([this](EventHandlerParams... params) { Raise(params...); });
	}

	Event(const Event&) = delete;

	Event& operator=(const Event&) = delete;

	EventHandlerID operator+=(EventHandler subscribingEventHandler) const
	{
		if (subscribingEventHandler == nullptr)
			throw std::runtime_error("Event handler is null");

		auto eventHandlerID{ m_nextAvailableID++ };
		m_eventHandlerByID.emplace(eventHandlerID, subscribingEventHandler);

		return eventHandlerID;
	}

	void operator-=(EventHandlerID unsubscribingEventHandlerID) const
	{
		auto unsubscribingEventHandlerIterator{ m_eventHandlerByID.find(unsubscribingEventHandlerID) };
		if (unsubscribingEventHandlerIterator != m_eventHandlerByID.end())
			throw std::logic_error("ID is incorrect");

		m_eventHandlerByID.erase(unsubscribingEventHandlerIterator);
	}

private:
	mutable std::unordered_map<EventHandlerID, EventHandler> m_eventHandlerByID;
	mutable EventHandlerID m_nextAvailableID{ 0ul };

	void Raise(EventHandlerParams... params) const noexcept
	{
		for (auto& iterator : m_eventHandlerByID)
			iterator.second(params...);
	}
};
