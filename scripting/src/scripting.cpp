#include <Python.h>

#include "scripting.h"
using mh::scripting::ModuleId;
using mh::scripting::Module;
using mh::scripting::ScriptLoadingException;
using mh::scripting::ScriptExecutionException;
using std::string;
using std::vector;

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
	vector<Module> modules;

	struct PyObjectHolder
	{
		explicit PyObjectHolder(PyObject* const object) :
			obj_(object)
		{}

		PyObjectHolder(const PyObjectHolder& other) :
			obj_(other.obj_)
		{
			Py_INCREF(this->obj_);
		}

		~PyObjectHolder()
		{
			Py_XDECREF(this->obj_);
		}

		bool operator==(const std::nullptr_t null) const
		{
			return this->obj_ == nullptr;
		}

		explicit operator bool() const
		{
			return this->obj_ != nullptr;
		}

		PyObject* const obj_;
	};

	ModuleId loadModule_noLock(const string& moduleName)
	{
		PyObjectHolder name(PyUnicode_DecodeFSDefault(moduleName.c_str()));
		if (!name)
			throw ScriptLoadingException("Cannot decode module name " + moduleName);

		const PyObjectHolder module(PyImport_Import(name.obj_));
		if (!module)
			throw ScriptLoadingException("Cannot load module " + moduleName);

		const ModuleId id = modules.size();
		modules.push_back(Module(id, moduleName));
		return id;
	}
}

ScriptLoadingException::ScriptLoadingException(const string& message) :
	std::exception(message.c_str())
{}

ScriptExecutionException::ScriptExecutionException(const string& message) :
	std::exception(message.c_str())
{}

Module::Module(const ModuleId& id, const std::string& name) :
	id_(id),
	name_(name)
{}

void mh::scripting::initializeScripting(const string& moduleLoadPath, const string& coreModule)
{
	lock_guard<mutex> _(modulesLock);
	if (modules.size() > 0)
		throw ScriptLoadingException("Scripting already initialized");

	Logger::LogInfo("Initializing root module \"" + coreModule + "\" from " + moduleLoadPath);

	std::wstring buffer;
	buffer.resize(moduleLoadPath.size());
	std::mbstowcs(&buffer[0], moduleLoadPath.c_str(), moduleLoadPath.size());

	const auto oldPath = Py_GetPath();
	const auto newPath = buffer + delimiter + oldPath;
	Py_SetPath(newPath.c_str());
	// No need to check for errors - "it is a fatal error if the initialization fails."
	Py_Initialize();
	try
	{
		loadModule_noLock(coreModule);
	}
	catch (const std::exception&)
	{
		Py_Finalize();
		throw;
	}
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

vector<Module> mh::scripting::loadedModules()
{
	lock_guard<mutex> _(modulesLock);
	return modules;
}

void mh::scripting::invoke(const ModuleId module, const string& functionName)
{
	throw ScriptExecutionException("Not implemented");
}