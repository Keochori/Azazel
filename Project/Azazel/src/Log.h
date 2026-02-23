#pragma once

#include <windows.h>
#include <iostream>
#include <filesystem>
#include <assert.h>

enum LogType
{
	eStandard,
	eSuccess,
	eWarning,
	eError
};

namespace LOG
{
	static void Print(const std::string& aString, const char* aFile, int aLine, const LogType aLogType = LogType::eStandard)
	{
		constexpr WORD RED = 12;
		constexpr WORD GREEN = 10;
		constexpr WORD YELLOW = 14;
		constexpr WORD WHITE = 15;

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (aLogType)
		{
			case eStandard: break;
			case eSuccess: SetConsoleTextAttribute(hConsole, GREEN); break;
			case eWarning: SetConsoleTextAttribute(hConsole, YELLOW); break;
			case eError: SetConsoleTextAttribute(hConsole, RED); break;
			default: break;
		}

		std::cout << "[" << std::filesystem::path(aFile).filename().string() << ":" << aLine << "] " << aString << std::endl;
		SetConsoleTextAttribute(hConsole, WHITE);
	}
}

#define LOG(string) LOG::Print(string, __FILE__, __LINE__, LogType::eStandard)
#define LOG_SUCCESS(string) LOG::Print(string, __FILE__, __LINE__, LogType::eSuccess)
#define LOG_WARNING(string) LOG::Print(string, __FILE__, __LINE__, LogType::eWarning)
#define LOG_ERROR(string) LOG::Print(string, __FILE__, __LINE__, LogType::eError)
#define Assert(bool) assert(bool)