#pragma once

class ISubscriptionWrapper
{
public:
	ISubscriptionWrapper() noexcept = default;
	ISubscriptionWrapper(const ISubscriptionWrapper&) = delete;
	virtual ~ISubscriptionWrapper() noexcept = default;

	ISubscriptionWrapper& operator=(const ISubscriptionWrapper&) = delete;

	virtual void Subscribe() noexcept = 0;
	virtual void Unsubscribe() noexcept = 0;

	virtual bool IsHandlerSubscribed() const noexcept = 0;
};
