#pragma once

#include "scripting_DLL_Export.hpp"

#include <Python.h>
#include <string>

namespace mh
{
	namespace scripting
	{
		class PyObjectHolder;

		/// Invokes str() from the Pythen Object protocol, converts result to std::string
		std::string MH_SCRIPTING_EXPORT PyObjectToString(const PyObjectHolder& obj);

		/// Throws ScriptExecutionError if python exception is signaled.
		void MH_SCRIPTING_EXPORT checkPythonError();
	}
}
