#pragma once

#include "scripting_DLL_Export.hpp"

#include <functional>
#include <vector>
#include <string>

#include "Value.h"

namespace mh
{
	namespace scripting
	{
		class MH_SCRIPTING_EXPORT ExposedModule
		{
		public:
			explicit ExposedModule(const std::string& name);
			
			/// Adds a fuction to the module, under the given name. The function must match the signature:
			///		(module object, argument tuple) -> return value
			/// Both arguments and the return value is wrapped in a Value object.
			/// @return A reference to this object.
			ExposedModule& addFunction(
				const std::string& name, 
				const std::function<Value(Value, Value)>& f, 
				const std::string& doc
			);

			/// Exposes the built module as a python built-in module. This must be called before initializeScripting()
			/// Throws ScriptExecutionException is the exposing fails.
			void expose();
		private:
			const std::string name_;
			std::vector<std::function<Value(Value, Value)>> functions_;
		};
	}
}
