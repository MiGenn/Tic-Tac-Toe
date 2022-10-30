#pragma once
#include "WinapiException.h"
#include <stdexcept>

template<SelectableGDIOBJ T>
class SelectionWrapper final
{
public:
	SelectionWrapper<T>() noexcept = default;
	SelectionWrapper<T>(HDC deviceContext, T object);
	SelectionWrapper<T>(const SelectionWrapper<T>&) = delete;
	SelectionWrapper<T>(SelectionWrapper<T>&& selectionWrapper) noexcept;
	~SelectionWrapper<T>() noexcept;

	SelectionWrapper<T>& operator=(const SelectionWrapper<T>&) = delete;
	SelectionWrapper<T>& operator=(SelectionWrapper<T>&& selectionWrapper) noexcept;

	void Select(T object);
	void Select(HDC deviceContext, T object);

private:
	HDC m_deviceContext{ nullptr };
	T m_previousObject{ nullptr };
};

template<SelectableGDIOBJ T>
inline SelectionWrapper<T>::SelectionWrapper(HDC deviceContext, T object)
{
	Select(deviceContext, object);
}

template<SelectableGDIOBJ T>
inline SelectionWrapper<T>::SelectionWrapper(SelectionWrapper<T>&& selectionWrapper) noexcept
{
	(*this) = std::move(selectionWrapper);
}

template<SelectableGDIOBJ T>
inline SelectionWrapper<T>::~SelectionWrapper() noexcept
{
	SelectObject(m_deviceContext, m_previousObject);
}

template<SelectableGDIOBJ T>
inline SelectionWrapper<T>& SelectionWrapper<T>::operator=(SelectionWrapper<T>&& selectionWrapper) noexcept
{
	m_deviceContext = selectionWrapper.m_deviceContext;
	m_previousObject = selectionWrapper.m_previousObject;

	selectionWrapper.m_deviceContext = nullptr;
	selectionWrapper.m_previousObject = nullptr;
}

template<SelectableGDIOBJ T>
inline void SelectionWrapper<T>::Select(T object)
{
	if (m_deviceContext == nullptr)
		throw std::logic_error("Device context isn't set");

	if (object == nullptr)
		throw std::runtime_error("GDI object is null");

	SelectObject(m_deviceContext, object);
}

template<SelectableGDIOBJ T>
inline void SelectionWrapper<T>::Select(HDC deviceContext, T object)
{
	if (deviceContext == nullptr)
		throw std::runtime_error("Device context is null");

	if (object == nullptr)
		throw std::runtime_error("GDI object is null");

	this->~SelectionWrapper();
	m_deviceContext = deviceContext;
	m_previousObject = reinterpret_cast<T>(SelectObject(m_deviceContext, object));
}
