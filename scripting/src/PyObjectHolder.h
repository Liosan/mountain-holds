#pragma once

#include <Python.h>

namespace mh
{
	namespace scripting
	{
		class PyObjectHolder
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
