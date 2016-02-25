#pragma once

#include "scripting_DLL_Export.hpp"

#include <vector>
#include <string>

namespace mh
{
	namespace scripting
	{
		/// Represents an error with initialization of the scripting system or a particular module.
		class ScriptLoadingException : std::exception
		{
		public:
			explicit ScriptLoadingException(const std::string& message);
		};

		/// Represents an error with running a particular scripting function.
		class ScriptExecutionException : std::exception
		{
		public:
			explicit ScriptExecutionException(const std::string& message);
		};

		typedef std::uint64_t ModuleId;

		/// Initializes scripting and loades the first module - the core module. 
		/// May throw ScriptLoadingException if loading of the core module fails.
		void MH_SCRIPTING_EXPORT initializeScripting(const std::string& moduleLoadPath, const std::string& coreModule);
		/// Shuts down scripting. Throws std::logic_error if invoked without matching initializeScripting call.
		void MH_SCRIPTING_EXPORT finalizeScripting();

		/// Loads a module and returns it's identifier. Throws ScriptLoadingException in case of error.
		ModuleId MH_SCRIPTING_EXPORT loadModule(const std::string& moduleName);
		/// Returns a list of loaded modules.
		std::vector<ModuleId> MH_SCRIPTING_EXPORT loadedModules();
		void MH_SCRIPTING_EXPORT invoke(const ModuleId module, const std::string& functionName);
	}
}