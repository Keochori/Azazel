#pragma once

#include <windows.h>
#include <iostream>
#include <filesystem>
#include <assert.h>
#include <crtdbg.h>

enum LogType
{
	eStandard,
	eSuccess,
	eWarning,
	eError
};

namespace LOG
{
	template<typename T>
	static void Print(const T& aValue, const char* aFile, int aLine, const LogType aLogType = LogType::eStandard)
	{
		constexpr WORD WHITE = 15;
		constexpr WORD GREEN = 10;
		constexpr WORD YELLOW = 6;
		constexpr WORD RED = 12;

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (aLogType)
		{
			case eStandard: break;
			case eSuccess: SetConsoleTextAttribute(hConsole, GREEN); break;
			case eWarning: SetConsoleTextAttribute(hConsole, YELLOW); break;
			case eError: SetConsoleTextAttribute(hConsole, RED); break;
			default: break;
		}

		std::cout << "[" << std::filesystem::path(aFile).filename().string() << ":" << aLine << "] " << aValue << std::endl;
		SetConsoleTextAttribute(hConsole, WHITE);
	}
}

#define LOG(value) LOG::Print(value, __FILE__, __LINE__, LogType::eStandard)
#define LOG_SUCCESS(value) LOG::Print(value, __FILE__, __LINE__, LogType::eSuccess)
#define LOG_WARNING(value) LOG::Print(value, __FILE__, __LINE__, LogType::eWarning)
#define LOG_ERROR(value) LOG::Print(value, __FILE__, __LINE__, LogType::eError)
#define ASSERT(bool) assert(bool)