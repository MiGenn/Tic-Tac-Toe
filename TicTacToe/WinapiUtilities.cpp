#include "WinapiUtilities.h"
#include <stdexcept>
#include "PathUtilities.h"
#include "TypeUtilities.h"

std::string GetErrorDescription(HRESULT errorCode) noexcept
{
	char* errorDescriptionBuffer;
	DWORD errorDescriptionBufferSize{ FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&errorDescriptionBuffer), 0ul, nullptr) };

	if (errorDescriptionBufferSize == 0ul)
		return "Undefined error code";

	std::string errorDescription(errorDescriptionBuffer, errorDescriptionBufferSize);
	LocalFree(errorDescriptionBuffer);

	return errorDescription;
}

std::wstring GetApplicationDirectory(HMODULE application)
{
	static constexpr int applicationFullPathBufferSize{ 256 };

	wchar_t applicationFullPathBuffer[applicationFullPathBufferSize];
	if (!GetModuleFileName(application, applicationFullPathBuffer, applicationFullPathBufferSize))
		throw WINAPI_LAST_EXCEPTION();

	return ExtractDirectory(applicationFullPathBuffer);
}

Vector2i CalculateWindowSize(Vector2i windowClientAreaSize, DWORD windowStyle, bool hasMenu)
{
	RECT windowRect{ 0, 0, windowClientAreaSize.x, windowClientAreaSize.y };
	if (!AdjustWindowRect(&windowRect, windowStyle, hasMenu))
		throw WINAPI_LAST_EXCEPTION();

	return { windowRect.right - windowRect.left, windowRect.bottom - windowRect.top };
}

Vector2i GetWindowSize(HWND window)
{
	RECT windowRect{};
	if (!GetWindowRect(window, &windowRect))
		throw WINAPI_LAST_EXCEPTION();

	return { windowRect.right - windowRect.left, windowRect.bottom - windowRect.top };
}

Vector2i GetWindowClientAreaSize(HWND window)
{
	RECT windowRect{};
	if (!GetClientRect(window, &windowRect))
		throw WINAPI_LAST_EXCEPTION();

	return { windowRect.right, windowRect.bottom };
}

Vector2i GetWindowPosition(HWND window)
{
	RECT windowRect{};
	if (!GetWindowRect(window, &windowRect))
		throw WINAPI_LAST_EXCEPTION();

	return { windowRect.left, windowRect.top };
}

Box2i GetWindowBox(HWND handle)
{
	RECT windowRect{};
	if (!GetWindowRect(handle, &windowRect))
		throw WINAPI_LAST_EXCEPTION();

	return RectToBox(windowRect);
}

Vector2i GetScreenSize() noexcept
{
	return { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
}

Vector2i GetMousePosition()
{
	POINT mousePosition;
	if (!GetCursorPos(&mousePosition))
		throw std::runtime_error("Cannot get mouse position");

	return PointToVector(mousePosition);
}

HFONT CreateWinapiFont(int characterSize, UINT style, const std::wstring& fontName)
{
	if (characterSize <= 0)
		throw std::logic_error("Character size is incorrect");

	return CreateFont(characterSize, NULL, 0, 0, style & 0x01 ? FW_BOLD : FW_NORMAL, style & 0x02, style & 0x04, style & 0x08,
		DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_DONTCARE, fontName.c_str());
}
