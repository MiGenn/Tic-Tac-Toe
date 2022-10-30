#pragma once
#include "WinapiException.h"
#include "WinapiUtilities.h"

template<GDIOBJ T>
class UniqueHGDIOBJ final
{
public:
	UniqueHGDIOBJ<T>() noexcept = default;
	UniqueHGDIOBJ<T>(T object) noexcept;
	UniqueHGDIOBJ<T>(const UniqueHGDIOBJ<T>&) = delete;
	UniqueHGDIOBJ<T>(UniqueHGDIOBJ<T>&& uniqueHGDIOBJ) noexcept;
	~UniqueHGDIOBJ<T>() noexcept;

	bool operator==(const UniqueHGDIOBJ<T>& uniqueHGDIOBJ) const noexcept;
	bool operator==(const T& object) const noexcept;
	bool operator==(std::nullptr_t) const noexcept;
	UniqueHGDIOBJ<T>& operator=(const UniqueHGDIOBJ<T>&) = delete;
	UniqueHGDIOBJ<T>& operator=(UniqueHGDIOBJ<T>&& uniqueHGDIOBJ) noexcept;
	explicit operator bool() const noexcept;

	void Reset(T object = nullptr) noexcept;
	T Release() noexcept;

	T Get() const noexcept;

private:
	T m_object{ nullptr };
};

template<GDIOBJ T>
inline UniqueHGDIOBJ<T>::UniqueHGDIOBJ(T object) noexcept
{
	m_object = object;
}

template<GDIOBJ T>
inline UniqueHGDIOBJ<T>::UniqueHGDIOBJ(UniqueHGDIOBJ<T>&& uniqueHGDIOBJ) noexcept
{
	(*this) = std::move(uniqueHGDIOBJ);
}

template<GDIOBJ T>
inline UniqueHGDIOBJ<T>::~UniqueHGDIOBJ() noexcept
{
	DeleteObject(m_object);
}

template<GDIOBJ T>
inline bool UniqueHGDIOBJ<T>::operator==(const UniqueHGDIOBJ<T>& uniqueHGDIOBJ) const noexcept
{
	return (*this) == uniqueHGDIOBJ.m_object;
}

template<GDIOBJ T>
inline bool UniqueHGDIOBJ<T>::operator==(const T& object) const noexcept
{
	return m_object == object;
}

template<GDIOBJ T>
inline bool UniqueHGDIOBJ<T>::operator==(std::nullptr_t) const noexcept
{
	return m_object == nullptr;
}

template<GDIOBJ T>
inline UniqueHGDIOBJ<T>& UniqueHGDIOBJ<T>::operator=(UniqueHGDIOBJ<T>&& uniqueHGDIOBJ) noexcept
{
	m_object = uniqueHGDIOBJ.m_object;
	uniqueHGDIOBJ.m_object = nullptr;

	return (*this);
}

template<GDIOBJ T>
inline UniqueHGDIOBJ<T>::operator bool() const noexcept
{
	return m_object != nullptr;
}

template<GDIOBJ T>
inline void UniqueHGDIOBJ<T>::Reset(T object) noexcept
{
	this->~UniqueHGDIOBJ();
	m_object = object;
}

template<GDIOBJ T>
inline T UniqueHGDIOBJ<T>::Release() noexcept
{
	auto object{ m_object };
	m_object = nullptr;

	return object;
}

template<GDIOBJ T>
inline T UniqueHGDIOBJ<T>::Get() const noexcept
{
	return m_object;
}
