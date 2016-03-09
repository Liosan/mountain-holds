#pragma once

#include "scripting_DLL_Export.hpp"

#include <string>
#include <memory>
#include <vector>

namespace mh
{
	namespace scripting
	{
		class PyObjectHolder;
		class Value;

		/// Represents an error with converting a value returned from a script.
		class ScriptValueConversionException : public std::exception
		{
		public:
			explicit ScriptValueConversionException(const std::string& message);
			ScriptValueConversionException(const std::string& message, const Value& value);
		};

		/// Wraps a value returned from scripts.
		class MH_SCRIPTING_EXPORT Value
		{
		public:
			explicit Value(const PyObjectHolder& obj);
			Value(const Value& other);
			Value(std::unique_ptr<PyObjectHolder>&& obj);
			~Value();

			std::string debugString() const;

			/// Whether the object is a None, null or similar unspecified value.
			/// If true, conversion functions will fail.
			bool empty() const;

			/// Throws ScriptValueConversionException if conversion fails.
			std::int64_t convertToNumber() const;
			/// Throws ScriptValueConversionException if conversion fails.
			std::string convertToString() const;
			/// Throws ScriptValueConversionException if conversion fails.
			std::vector<Value> convertToArray() const;
		private:
			std::unique_ptr<PyObjectHolder> obj_;
		};
	}
}