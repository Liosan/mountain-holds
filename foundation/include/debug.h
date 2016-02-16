#pragma once

#include <mutex>
#include "foundation_DLL_Export.hpp"

#if defined(_DEBUG)

#if defined(_WIN32) 
#define _HANDLE_MH_ASSERT_FAILURE(conditionMessage, message) \
	{ \
		if (mh::debug::IsWindowsDebuggerPresent()) \
		{ \
			__debugbreak(); \
		} \
		else \
		{ \
			mh::debug::ShowAssertionWindow(__FILE__, __LINE__, conditionMessage, message); \
		} \
	}
#elif __APPLE__	
#define _HANDLE_MH_ASSERT_FAILURE(conditionMessage, message) \
	{ \
		mh::debug::ShowAssertionWindow(__FILE__, __LINE__, conditionMessage, message); \
		__asm__	("int $3"); \
	}
#else
#define _HANDLE_MH_ASSERT_FAILURE(message) \
	{ \
		const char* cmd = "\012SHOW CALLS"; \
		lib$signal(SS$_DEBUG, 1, cmd); \
	}
#endif

#define MH_ASSERT(condition, message) \
	{	\
		if ((condition) == false) \
		{ \
			std::lock_guard<std::mutex> _lck_(mh::debug::assertionMutex); \
			_HANDLE_MH_ASSERT_FAILURE(#condition, message); \
		} \
	}; (false)

#else // defined(_DEBUG) || defined(XCODE_DEBUG)

#define MH_ASSERT(condition, message) do {} while(false)

#endif // defined(_DEBUG) || defined(XCODE_DEBUG)

namespace mh
{
	namespace debug
	{
		void MH_FOUNDATION_EXPORT ShowAssertionWindow(
			const char* file, 
			const int line, 
			const char* conditionMessage, 
			const char* message
		);
		bool MH_FOUNDATION_EXPORT IsWindowsDebuggerPresent();

		extern std::mutex MH_FOUNDATION_EXPORT assertionMutex;
	}
}
