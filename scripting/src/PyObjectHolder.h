#pragma once

#include "scripting_DLL_Export.hpp"

#include <Python.h>

namespace mh
{
	namespace scripting
	{
		class MH_SCRIPTING_EXPORT PyObjectHolder
		{
		public:
			PyObjectHolder();
			PyObjectHolder(PyObject* const object);
			PyObjectHolder(const PyObjectHolder& other);
			PyObjectHolder(PyObjectHolder&& other);
			PyObjectHolder& operator=(const PyObjectHolder& other);
			PyObjectHolder& operator=(PyObjectHolder&& other);
			~PyObjectHolder();
			
			bool operator==(const std::nullptr_t) const;
			bool operator!=(const std::nullptr_t) const;
			explicit operator bool() const;
			operator PyObject*() const;
		private:
			PyObject* obj_;
		};
	}
}
