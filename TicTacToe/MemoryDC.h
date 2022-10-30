#pragma once
#include "WinapiException.h"

class MemoryDC final
{
public:
	MemoryDC() noexcept = default;
	MemoryDC(HDC memoryDC) noexcept;
	MemoryDC(const MemoryDC&) = delete;
	MemoryDC(MemoryDC&& memoryDC) noexcept;
	~MemoryDC() noexcept;

	bool operator==(std::nullptr_t) const noexcept;
	MemoryDC& operator=(const MemoryDC&) = delete;
	MemoryDC& operator=(MemoryDC&& memoryDC) noexcept;
	explicit operator bool() const noexcept;

	void Reset(HDC memoryDC) noexcept;
	HDC Get() const noexcept;

private:
	HDC m_memoryDC{ nullptr };
};
