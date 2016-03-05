#include <Python.h>

#include "Value.h"
using mh::scripting::Value;
using mh::scripting::ScriptValueConversionException;
using std::string;
using std::vector;

#include "PyObjectHolder.h"
using mh::scripting::PyObjectHolder;
#include "PythonUtils.h"
using mh::scripting::PyObjectToString;

ScriptValueConversionException::ScriptValueConversionException(const string& message, const Value& value) :
	std::exception((message + "; object: " + value.debugString()).c_str())
{}

Value::Value(const PyObjectHolder& obj) :
	obj_(std::make_unique<PyObjectHolder>(obj))
{}

Value::Value(const Value& other) :
	obj_(nullptr)
{
	if (other.obj_ != nullptr)
		this->obj_ = std::make_unique<PyObjectHolder>(*other.obj_);
}

mh::scripting::Value::Value(std::unique_ptr<PyObjectHolder>&& obj) :
	obj_(move(obj))
{}

Value::~Value()
{}

string Value::debugString() const
{
	if (this->obj_ == nullptr)
		return "(null)";

	return PyObjectToString(*this->obj_);
}

bool Value::empty() const
{
	return (this->obj_ == nullptr) || (*this->obj_ == Py_None);
}

std::int64_t Value::convertToNumber() const
{
	throw ScriptValueConversionException("Not implemented", *this);
}

string Value::convertToString() const
{
	throw ScriptValueConversionException("Not implemented", *this);
}

vector<Value> Value::convertToArray() const
{
	throw ScriptValueConversionException("Not implemented", *this);
}
