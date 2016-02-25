#include <Python.h>

#include "scripting.h"
using mh::scripting::ModuleId;
using mh::scripting::ScriptLoadingException;
using mh::scripting::ScriptExecutionException;
using std::string;
using std::vector;

#include <map>
using std::map;
#include <mutex>
using std::mutex;
using std::lock_guard;

#include "foundation/debug.h"
#include "foundation/Logger.h"
using mh::foundation::Logger;

namespace
{
#ifdef _WIN32
	const std::wstring delimiter = L";";
#else
	const std::wstring delimiter = L":";
#endif

	mutex modulesLock;
	map<ModuleId, string> modules;

	// TODO inherit lock?
	ModuleId loadModule_noLock(const string& moduleName)
	{
		// TODO: https://docs.python.org/3/extending/embedding.html
		// TODO: RAII-wrapper for PyObject* (including cast to PyObject* operator?)
		throw ScriptLoadingException("Not implemented");
	}
}

ScriptLoadingException::ScriptLoadingException(const string& message) :
	std::exception(message.c_str())
{}

ScriptExecutionException::ScriptExecutionException(const string& message) :
	std::exception(message.c_str())
{}

void mh::scripting::initializeScripting(const string& moduleLoadPath, const string& coreModule)
{
	lock_guard<mutex> _(modulesLock);
	Logger::LogInfo("Initializing root module \"" + coreModule + "\" from " + moduleLoadPath);

	std::wstring buffer;
	buffer.resize(moduleLoadPath.size());
	std::mbstowcs(&buffer[0], moduleLoadPath.c_str(), moduleLoadPath.size());

	const auto oldPath = Py_GetPath();
	const auto newPath = buffer + delimiter + oldPath;
	Py_SetPath(newPath.c_str());
	Py_Initialize();
	// TODO check for errors?
	loadModule_noLock(coreModule);
}

void mh::scripting::finalizeScripting()
{
	lock_guard<mutex> _(modulesLock);
	Logger::LogInfo("Scripting module shutting down");
	if (modules.empty())
		throw std::logic_error("No modules loaded during finalizeScripting() - was initializeScripting() called?");
	modules.clear();
	Py_Finalize();
}

ModuleId mh::scripting::loadModule(const string& moduleName)
{
	lock_guard<mutex> _(modulesLock);
	Logger::LogInfo("Initializing module \"" + moduleName + "\"");
	return loadModule_noLock(moduleName);
}

vector<ModuleId> mh::scripting::loadedModules()
{
	lock_guard<mutex> _(modulesLock);
	vector<ModuleId> result;
	for (const auto& entry : modules)
		result.push_back(entry.first);
	return result;
}

void mh::scripting::invoke(const ModuleId module, const string& functionName)
{
	throw ScriptExecutionException("Not implemented");
}