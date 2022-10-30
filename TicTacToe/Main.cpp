#include "TicTacToe.h"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	static constexpr auto messageBoxTitle{ "Error" };
	static constexpr UINT messageBoxStyle{ MB_OK | MB_ICONEXCLAMATION };

	try
	{
		return TicTacToe().Run();
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), messageBoxTitle, messageBoxStyle);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "An unknown error has occured", messageBoxTitle, messageBoxStyle);
	}

	return -1;
}