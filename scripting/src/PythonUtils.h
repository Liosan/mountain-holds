#pragma once

#include <Python.h>
#include <string>

namespace mh
{
	namespace scripting
	{
		class PyObjectHolder;

		/// Invokes str() from the Pythen Object protocol, converts result to std::string
		std::string PyObjectToString(const PyObjectHolder& obj);

		/// Throws ScriptExecutionError if python exception is signaled.
		void checkPythonError();
	}
}
