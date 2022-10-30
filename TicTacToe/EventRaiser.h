#pragma once
#include <functional>

template<class... EventHandlerParams>
class EventRaiser final
{
public:
	using RaiseFunction = std::function<void(EventHandlerParams...)>;

	EventRaiser() noexcept = default;
	EventRaiser(const RaiseFunction& raiseFunction) noexcept :
		m_raiseFunction(raiseFunction)
	{

	}

	void Raise(EventHandlerParams ...params) const noexcept
	{
		m_raiseFunction(params...);
	}

private:
	RaiseFunction m_raiseFunction;
};
