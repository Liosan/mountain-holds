#include <Python.h>

#include "scripting.h"
using mh::scripting::ModuleId;
using mh::scripting::Module;
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

const ModuleId mh::scripting::CoreModuleId = 0;

namespace
{
	struct PyObjectHolder
	{
		PyObjectHolder() :
			obj_(nullptr)
		{}

		PyObjectHolder(PyObject* const object) :
			obj_(object)
		{}

		PyObjectHolder(const PyObjectHolder& other) :
			obj_(other.obj_)
		{
			Py_INCREF(this->obj_);
		}
		
		PyObjectHolder(PyObjectHolder&& other) :
			obj_(other.obj_)
		{} // No reference increase? TODO verify

		PyObjectHolder& operator=(const PyObjectHolder& other)
		{
			Py_XDECREF(this->obj_);
			this->obj_ = other.obj_;
			Py_INCREF(this->obj_);
			return *this;
		}

		PyObjectHolder& operator=(PyObjectHolder&& other)
		{
			Py_XDECREF(this->obj_);
			this->obj_ = other.obj_;
			// No reference increase? TODO verify
			return *this;
		}

		// TODO pull out to header, write unit tests, operator != nullptr

		~PyObjectHolder()
		{
			Py_XDECREF(this->obj_);
		}

		bool operator==(const std::nullptr_t) const
		{
			return this->obj_ == nullptr;
		}

		explicit operator bool() const
		{
			return this->obj_ != nullptr;
		}

		operator PyObject*() const
		{
			return this->obj_;
		}

		PyObject* obj_;
	};

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

		const PyObjectHolder module(PyImport_Import(name.obj_));
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

	string PyUnicodeStringToString(PyObject* const obj)
	{
		PyObjectHolder strObj = PyObject_Str(obj);
		const wstring widechar(PyUnicode_AS_UNICODE(strObj.obj_));
		string buffer;
		buffer.resize(widechar.size());
		std::wcstombs(&buffer[0], &widechar[0], widechar.size());
		return buffer;
	}

	void checkPythonError()
	{
		if (PyErr_Occurred())
		{
			PyObject *ptype, *pvalue, *ptraceback;
			PyErr_Fetch(&ptype, &pvalue, &ptraceback);
			throw ScriptExecutionException(PyUnicodeStringToString(pvalue));
		}
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

void mh::scripting::invoke(const ModuleId& moduleId, const string& functionName)
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

	PyObjectHolder args = PyTuple_New(0);
	PyObject_CallObject(function, args);
	checkPythonError();
}
