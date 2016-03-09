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

ScriptValueConversionException::ScriptValueConversionException(const string& message) :
	std::exception((message + "; object: (null)").c_str())
{}

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
	return PyObjectToString(*this->obj_);
}

bool Value::empty() const
{
	return (*this->obj_ == nullptr) || (*this->obj_ == Py_None);
}

std::int64_t Value::convertToNumber() const
{
	if (this->empty())
		throw ScriptValueConversionException("Failed to convert empty python object to number");

	if (!PyNumber_Check(*this->obj_))
		throw ScriptValueConversionException(
			"Failed to convert python object to number",
			*this
		);

	return PyNumber_AsSsize_t(*this->obj_, NULL); // silently swallow exceptions, overflows will be clipped
}

string Value::convertToString() const
{
	if (this->empty())
		throw ScriptValueConversionException("Failed to convert empty python object to string");

	if (!PyUnicode_Check(*this->obj_))
		throw ScriptValueConversionException(
			"Failed to convert python object to unicode string",
			*this
		);
		
	return PyObjectToString(*this->obj_);
}

vector<Value> Value::convertToArray() const
{
	throw ScriptValueConversionException("Not implemented", *this);
}
