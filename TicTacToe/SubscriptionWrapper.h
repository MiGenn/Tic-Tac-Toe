#pragma once
#include <vector>
#include "ISubscriptionWrapper.h"
#include "Event.h"

template<class... HandlerParams>
class SubscriptionWrapper final : public ISubscriptionWrapper
{
public:
	using Event = Event<HandlerParams...>;
	using EvenHandler = Event::Handler;

	SubscriptionWrapper(Event& event, EvenHandler eventHandler) noexcept :
		m_event(event), m_eventHandler(eventHandler)
	{

	}

	virtual ~SubscriptionWrapper() noexcept
	{
		Unsubscribe();
	}

	virtual void Subscribe() noexcept override
	{
		if (m_isEventHandlerSubscribed)
			return;

		m_eventHandlerID = m_event += m_eventHandler;
		m_isEventHandlerSubscribed = true;
	}

	virtual void Unsubscribe() noexcept override
	{
		if (m_isEventHandlerSubscribed)
		{
			m_event -= m_eventHandlerID;
			m_isEventHandlerSubscribed = false;
		}
	}

	virtual bool IsHandlerSubscribed() const noexcept
	{
		return m_isEventHandlerSubscribed;
	}

private:
	Event& m_event;
	EvenHandler m_eventHandler;
	Event::HandlerID m_eventHandlerID;
	bool m_isEventHandlerSubscribed{ false };
};
