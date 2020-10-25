#include "console.h"
#include <Windows.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <random>

std::string random_string(const int len) {
	const std::string alpha_numeric("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");

	std::default_random_engine generator{ std::random_device{}() };
	const std::uniform_int_distribution< std::string::size_type > distribution{ 0, alpha_numeric.size() - 1 };

	std::string str(len, 0);
	for (auto& it : str) {
		it = alpha_numeric[distribution(generator)];
	}

	return str;
}

void console::alloc()
{
	AllocConsole();

	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	SetConsoleTitle(random_string(32).c_str());
	printf("\nCommunity Discord: https://discord.gg/AEfuvwT \n");
	printf("Premium Discord: https://discord.gg/KXEETbr \n");

	ShowWindow(GetConsoleWindow(), SW_MINIMIZE);

	HKEY hKey;
	LONG nResult;
	DWORD buffer;
	unsigned long type=REG_DWORD, size=1024;
	
	nResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "System\\ActivationBroker\\fgp\\license", 0, KEY_READ, &hKey);
	if(nResult == ERROR_SUCCESS) {
		RegQueryValueEx(hKey, "valid", NULL, &type, (LPBYTE)&buffer, &size);
		RegCloseKey(hKey);
		printf("\n\nThe license is active, thank you for your support,\r\nin case of any irregularities, please report it on the discord.\n\n");
	} else {
		TerminateProcess(GetCurrentProcess(), 0);
	}
}

void console::clean()
{
	FreeConsole();
}

void console::log(const char* fmt, ...)
{
	va_list va_alist;
	char logBuf[1024] = { 0 };

	va_start(va_alist, fmt);
	_vsnprintf(logBuf + strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	if (logBuf[0] != '\0')
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_GREEN | FOREGROUND_INTENSITY));
		printf("[%s]", get_time_string().c_str());
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
		printf(": %s\n", logBuf);
	}
}

std::string console::get_time_string()
{
	time_t current_time;
	struct tm* time_info;
	static char timeString[10];

	time(&current_time);
	time_info = localtime(&current_time);

	strftime(timeString, sizeof(timeString), "%I:%M %p", time_info);
	return timeString;
}