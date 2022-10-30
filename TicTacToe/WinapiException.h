#pragma once
#define WIN32_LEAN_AND_MEAN

#include "Exception.h"
#include <windows.h>

class WinapiException final : public Exception
{
public:
	WinapiException(int line, const char* file, HRESULT errorCode) noexcept;

	virtual const std::string& GetDescription() const noexcept override;

private:
	std::string m_description;
};

#define WINAPI_EXCEPTION(errorCode) WinapiException(__LINE__, __FILE__, errorCode)
#define WINAPI_LAST_EXCEPTION() WinapiException(__LINE__, __FILE__, GetLastError())

#undef min
#undef max
