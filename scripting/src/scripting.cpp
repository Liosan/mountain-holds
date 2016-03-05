#include "scripting.h"
using mh::scripting::ModuleId;
using mh::scripting::Module;
using mh::scripting::Value;
using mh::scripting::ScriptLoadingException;
using mh::scripting::ScriptExecutionException;
using std::string;
using std::wstring;
using std::vector;

#include <mutex>
using std::mutex;
using std::lock_guard;
using std::pair;
using std::make_pair;
#include <memory>
using std::unique_ptr;
using std::make_unique;
#include <string>
using std::to_string;

#include "foundation/debug.h"
#include "foundation/Logger.h"
using mh::foundation::Logger;
#include "Value.h"
using mh::scripting::Value;
#include "PyObjectHolder.h"
using mh::scripting::PyObjectHolder;
#include "PythonUtils.h"
using mh::scripting::checkPythonError;
using mh::scripting::PyObjectToString;

const ModuleId mh::scripting::CoreModuleId = 0;

namespace
{
	struct PythonModule
	{
		PythonModule(const Module& module, const PyObjectHolder& pyObj) :
			module_(module),
			pyObj_(pyObj)
		{}
		PythonModule(const PythonModule& other) = delete;
		PythonModule(PythonModule&& other) = delete;
		PythonModule& operator=(const PythonModule& other) = delete;
		PythonModule& operator=(PythonModule&& other) = delete;

		const Module module_;
		PyObjectHolder pyObj_;
	};

	mutex modulesLock;
	vector<unique_ptr<PythonModule>> modules;

	ModuleId loadModule_noLock(const string& moduleName)
	{
		PyObjectHolder name(PyUnicode_DecodeFSDefault(moduleName.c_str()));
		if (!name)
			throw ScriptLoadingException("Cannot decode module name " + moduleName);

		const PyObjectHolder module(PyImport_Import(name));
		if (!module)
			throw ScriptLoadingException("Cannot load module " + moduleName);

		const ModuleId id = modules.size();
		modules.push_back(make_unique<PythonModule>(Module(id, moduleName), module));
		return id;
	}

	PythonModule& findModule(const ModuleId& moduleId)
	{
		lock_guard<mutex> _(modulesLock);
		if (modules.size() < moduleId)
			throw ScriptExecutionException("Module with id " + to_string(moduleId) + " not found");
		return *modules[moduleId];
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

	wstring buffer;
	buffer.resize(moduleLoadPath.size());
	std::mbstowcs(&buffer[0], moduleLoadPath.c_str(), moduleLoadPath.size());

#ifdef _WIN32
	const wstring delimiter = L";";
#else
	const wstring delimiter = L":";
#endif

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
	vector<Module> result;
	for (const auto& module : modules)
		result.push_back(module->module_);
	return result;
}

Value mh::scripting::invoke(const ModuleId& moduleId, const string& functionName)
{
	auto& module = findModule(moduleId);
	PyObjectHolder function(PyObject_GetAttrString(module.pyObj_, functionName.c_str()));

	if (!function)
		throw ScriptExecutionException(
			"Function with name " + functionName + " not found in module " + module.module_.name_
		);

	if (!PyCallable_Check(function))
		throw ScriptExecutionException(
			"Symbol " + functionName + " in module " + module.module_.name_ + " is not callable"
		);

	const PyObjectHolder args = PyTuple_New(0);
	auto result = std::make_unique<PyObjectHolder>(PyObject_CallObject(function, args));
	checkPythonError();
	return Value(std::move(result));
}
