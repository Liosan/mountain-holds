#include "Debug.h"

#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

std::mutex mh::debug::assertionMutex;

void mh::debug::ShowAssertionWindow(const char* file, const int line, const char* conditionMessage, const char* message)
{
	std::ostringstream oss;
	oss << "Debug Assertion '" << conditionMessage << "' Failed\nFile: " << file << "\nLine: " << line << "\n" << message;
	const std::string errorMessage = oss.str();

#if _WIN32
	const int result = ::MessageBox(
		NULL,
		errorMessage.c_str(),
		"Debug Assertion Failed",
		MB_RETRYCANCEL | MB_ICONSTOP
	);
	if (result == IDCANCEL)
	{
		::TerminateProcess(GetCurrentProcess(), -1);
	}
	else if (result == IDRETRY)
	{
		DebugBreak();
	}
#else

#error Not implemented

#endif
}

bool mh::debug::IsWindowsDebuggerPresent()
{
#ifdef _WIN32
	return IsDebuggerPresent() == TRUE;
#else
	return false;
#endif
}
