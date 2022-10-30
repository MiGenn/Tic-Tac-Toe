#include "SessionInfoSaver.h"
#include "PlayerNonGameData.h"
#include <stdexcept>

void SessionInfoSaver::Save(std::wofstream& file, const SYSTEMTIME& endTime, 
	const PlayerNonGameData& firstPlayerData, const PlayerNonGameData& secondPlayerData, GameWorld::Player winner)
{
	if (!file.is_open())
		throw std::logic_error("File must be opened");

	auto [date, time] { FormatSystemTime(endTime) };
	auto& winnerName{ GetWinnerName(winner, firstPlayerData, secondPlayerData) };

	file << L"\n";
	file << firstPlayerData.name << L" vs " << secondPlayerData.name << L"\n";
	file << L"{\n";
	file << L"\tDate: " << date << L"\n";
	file << L"\tTime: " << time << L"\n";
	file << L"\n";
	file << L"\t" << winnerName << L" is winner" << L"\n";
	file << L"}" << std::endl;
}

std::pair<std::wstring, std::wstring> SessionInfoSaver::FormatSystemTime(const SYSTEMTIME& time)
{
	static constexpr int dateBufferSize{ 96 };
	static constexpr int timeBufferSize{ 32 };

	wchar_t dateBuffer[dateBufferSize];
	wchar_t timeBuffer[timeBufferSize];

	if (!GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &time, nullptr, dateBuffer, dateBufferSize))
		throw WINAPI_LAST_EXCEPTION();

	if (!GetTimeFormat(LOCALE_USER_DEFAULT, NULL, &time, nullptr, timeBuffer, timeBufferSize))
		throw WINAPI_LAST_EXCEPTION();

	return { dateBuffer, timeBuffer };
}

const std::wstring& SessionInfoSaver::GetWinnerName(GameWorld::Player winner,
	const PlayerNonGameData& firstPlayerData, const PlayerNonGameData& secondPlayerData) noexcept
{
	if (winner == GameWorld::Player::FirstPlayer)
		return firstPlayerData.name;

	return secondPlayerData.name;
}
