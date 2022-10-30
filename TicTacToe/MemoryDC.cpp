#include "MemoryDC.h"

MemoryDC::MemoryDC(HDC memoryDC) noexcept :
	m_memoryDC(memoryDC)
{
	
}

MemoryDC::MemoryDC(MemoryDC&& memoryDC) noexcept
{
	(*this) = std::move(memoryDC);
}

MemoryDC::~MemoryDC() noexcept
{
	if (m_memoryDC != nullptr)
		DeleteDC(m_memoryDC);
}

bool MemoryDC::operator==(std::nullptr_t) const noexcept
{
	return m_memoryDC == nullptr;
}

MemoryDC& MemoryDC::operator=(MemoryDC&& memoryDC) noexcept
{
	m_memoryDC = memoryDC.m_memoryDC;
	memoryDC.m_memoryDC = nullptr;

	return *this;
}

MemoryDC::operator bool() const noexcept
{
	return m_memoryDC != nullptr;
}

void MemoryDC::Reset(HDC memoryDC) noexcept
{
	this->~MemoryDC();
	m_memoryDC = memoryDC;
}

HDC MemoryDC::Get() const noexcept
{
	return m_memoryDC;
}
