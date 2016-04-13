#include "ExposedModule.h"
using mh::scripting::ExposedModule;
using mh::scripting::Value;
using std::string;

ExposedModule::ExposedModule(const string& name) :
	name_(name)
{}

ExposedModule& ExposedModule::addFunction(
	const std::string& name,
	const std::function<Value(Value, Value)>& f,
	const std::string& doc
)
{
	return *this;
}

void ExposedModule::expose()
{

}