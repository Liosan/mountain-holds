#include <Python.h>

#include "PyObjectHolder.h"
using mh::scripting::PyObjectHolder;

PyObjectHolder::PyObjectHolder() :
	obj_(nullptr)
{}

PyObjectHolder::PyObjectHolder(PyObject* const object) :
	obj_(object)
{}

PyObjectHolder::PyObjectHolder(const PyObjectHolder& other) :
	obj_(other.obj_)
{
	Py_XINCREF(this->obj_);
}

PyObjectHolder::PyObjectHolder(PyObjectHolder&& other) :
	obj_(other.obj_)
{} // No reference increase? TODO verify

PyObjectHolder& PyObjectHolder::operator=(const PyObjectHolder& other)
{
	Py_XDECREF(this->obj_);
	this->obj_ = other.obj_;
	Py_XINCREF(this->obj_);
	return *this;
}

PyObjectHolder& PyObjectHolder::operator=(PyObjectHolder&& other)
{
	Py_XDECREF(this->obj_);
	this->obj_ = other.obj_;
	// No reference increase? TODO verify
	return *this;
}

PyObjectHolder::~PyObjectHolder()
{
	Py_XDECREF(this->obj_);
}

bool PyObjectHolder::operator==(const std::nullptr_t) const
{
	return this->obj_ == nullptr;
}

bool PyObjectHolder::operator!=(const std::nullptr_t) const
{
	return this->obj_ != nullptr;
}

PyObjectHolder::operator bool() const
{
	return this->obj_ != nullptr;
}

PyObjectHolder::operator PyObject*() const
{
	return this->obj_;
}
