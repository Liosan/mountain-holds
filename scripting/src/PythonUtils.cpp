#include "PythonUtils.h"

#include <string>
using std::string;
using std::wstring;

#include "PyObjectHolder.h"
using mh::scripting::PyObjectHolder;
#include "scripting.h"
using mh::scripting::ScriptExecutionException;

string mh::scripting::PyObjectToString(const PyObjectHolder& obj)
{
	PyObjectHolder strObj = PyObject_Str(obj);
	const wstring widechar(PyUnicode_AS_UNICODE(static_cast<PyObject*>(strObj)));
	string buffer;
	buffer.resize(widechar.size());
	std::wcstombs(&buffer[0], &widechar[0], widechar.size());
	return buffer;
}

void mh::scripting::checkPythonError()
{
	if (PyErr_Occurred())
	{
		PyObject *ptype, *pvalue, *ptraceback;
		PyErr_Fetch(&ptype, &pvalue, &ptraceback);
		throw ScriptExecutionException(PyObjectToString(pvalue));
	}
}
