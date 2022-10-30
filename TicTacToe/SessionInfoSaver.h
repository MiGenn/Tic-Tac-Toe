#pragma once
#include "WinapiException.h"
#include "GameWorld.h"
#include <fstream>

class PlayerNonGameData;

class SessionInfoSaver final
{
public:
	SessionInfoSaver() = delete;
	SessionInfoSaver(const SessionInfoSaver&) = delete;

	SessionInfoSaver& operator=(const SessionInfoSaver&) = delete;

	static void Save(std::wofstream& file, const SYSTEMTIME& endTime, 
		const PlayerNonGameData& firstPlayerData, const PlayerNonGameData& secondPlayerData, GameWorld::Player winner);

private:
	static std::pair<std::wstring, std::wstring> FormatSystemTime(const SYSTEMTIME& time);
	static const std::wstring& GetWinnerName(GameWorld::Player winner,
		const PlayerNonGameData& firstPlayerData, const PlayerNonGameData& secondPlayerData) noexcept;
};
