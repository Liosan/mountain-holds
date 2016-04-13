#include <gtest/gtest.h>

#include <string>
using std::string;

#include <Python.h>

#include "scripting.h"
#include "ExposedModule.h"
#include "Value.h"
#include "PyObjectHolder.h"
#include "PythonUtils.h"

namespace mh
{
	namespace scripting
	{
		namespace test
		{
			struct ExposedModuleTest : ::testing::Test
			{
			public:
				virtual void SetUp() override
				{}

				virtual void TearDown() override
				{
					try
					{
						finalizeScripting();
					}
					catch (...)
					{
					}
				}

				PyObjectHolder run(const string& script) const
				{
					const PyObjectHolder globals = Py_BuildValue("{}");
					PyDict_SetItemString(globals, "__builtins__", PyEval_GetBuiltins());
					checkPythonError();
					const PyObjectHolder locals = Py_BuildValue("{}");

					const PyObjectHolder result = PyRun_String(script.c_str(), Py_file_input, globals, locals);
					checkPythonError();
					return result;
				}
			};

			TEST_F(ExposedModuleTest, NoModule_ShouldSeeBuiltins)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "moduleA");
				const Value module(PyImport_ImportModule("sys"));
				EXPECT_FALSE(module.empty());
				EXPECT_NO_THROW(this->run("import sys"));
			}

			TEST_F(ExposedModuleTest, EmptyModule_ShouldExpose)
			{
				ExposedModule("bob").expose();
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "moduleA");
				const Value module(PyImport_ImportModule("bob"));
				EXPECT_FALSE(module.empty());
				EXPECT_NO_THROW(this->run("import bob"));
			}

			// TODO failure: duplicate module name
			// TODO module name with.dot
			// TODO failure: expose after initialize failure
			// TODO returning values from functions
			// TODO processing function arguments
			// TODO add() function
			// TODO failure: duplicate function name
		}
	}
}
