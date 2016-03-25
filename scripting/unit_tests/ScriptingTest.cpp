#include <gtest/gtest.h>

#include <string>
using std::string;

#include <Python.h>

#include "scripting.h"
#include "Value.h"
#include "PyObjectHolder.h"
#include "PythonUtils.h"

namespace mh
{
	namespace scripting
	{
		namespace test
		{
			struct ScriptingTest : ::testing::Test
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
					{}
				}
			};

			TEST_F(ScriptingTest, BeforeInitialize_ShouldHaveZeroModules)
			{
				EXPECT_EQ(0, loadedModules().size());
			}

			TEST_F(ScriptingTest, AfterInitialize_ShouldHaveOneModule)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "moduleA");
				EXPECT_TRUE(Py_IsInitialized() > 0);
				ASSERT_EQ(1, loadedModules().size());
				EXPECT_EQ(0, loadedModules()[0].id_) << "Root module should have id 0";
			}

			TEST_F(ScriptingTest, DoubleInitialize_ShouldThrow)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "moduleA");
				EXPECT_THROW(initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "moduleA"), ScriptLoadingException);
			}

			TEST_F(ScriptingTest, InitializeNonExistentModule_ShouldThrow)
			{
				EXPECT_THROW(initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "nonexistent"), ScriptLoadingException);
				EXPECT_EQ(0, Py_IsInitialized());
			}

			TEST_F(ScriptingTest, AfterFinalize_ShouldHaveZeroModules)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "moduleA");
				finalizeScripting();
				EXPECT_EQ(0, Py_IsInitialized());
				EXPECT_EQ(0, loadedModules().size());
			}

			TEST_F(ScriptingTest, FinalizeWithoutInitialize_ShouldThrow)
			{
				EXPECT_THROW(finalizeScripting(), std::logic_error);
			}

			// TODO test value -> from natively constructed python int, string, list, nested list, null
			// TODO test read value -> return int, string, list, nested list, null
			// TODO test read value -> failure cases: wrong module, non-existant symbol

			TEST_F(ScriptingTest, InvokeFunctionPass_ShouldDoNothing)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "core");
				const auto result = invoke(CoreModuleId, "doNothing");
				EXPECT_TRUE(result.empty());
			}

			TEST_F(ScriptingTest, InvokeFunctionReturningInt_ShouldRetrieveInt)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "core");
				const auto result = invoke(CoreModuleId, "returnInt");
				EXPECT_FALSE(result.empty());
				EXPECT_EQ(4, result.convertToNumber());
				EXPECT_THROW(result.convertToString(), ScriptValueConversionException);
			}

			TEST_F(ScriptingTest, InvokeFunctionReturningString_ShouldRetrieveString)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "core");
				const auto result = invoke(CoreModuleId, "returnString");
				EXPECT_FALSE(result.empty());
				EXPECT_EQ("I am cow", result.convertToString());
				EXPECT_THROW(result.convertToNumber(), ScriptValueConversionException);
			}

			TEST_F(ScriptingTest, InvokeNonExistantFunction_ShouldThrow)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "core");
				EXPECT_THROW(invoke(CoreModuleId, "nonexistant"), ScriptExecutionException);
			}

			TEST_F(ScriptingTest, InvokeNonExistantModule_ShouldThrow)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "core");
				EXPECT_THROW(invoke(12, "doNothing"), ScriptExecutionException);
			}

			TEST_F(ScriptingTest, InvokeNotCallableSymbol_ShouldThrow)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "core");
				EXPECT_THROW(invoke(CoreModuleId, "number"), ScriptExecutionException);
			}

			TEST_F(ScriptingTest, InvokeFunctionWithException_ShouldThrow)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "core");
				EXPECT_THROW(invoke(CoreModuleId, "throwException"), ScriptExecutionException);
			}
			
			struct ScriptingValueTest : ::testing::Test
			{
			public:
				virtual void SetUp() override
				{
					initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "core");
				}

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
					const PyObjectHolder locals = Py_BuildValue("{}");
					
					const PyObjectHolder result = PyRun_String(script.c_str(), Py_eval_input, globals, locals);
					checkPythonError();
					return result;
				}
			};

			TEST_F(ScriptingValueTest, NoneValue_ShouldBeEmpty)
			{
				const Value value(Py_None);
				EXPECT_TRUE(value.empty());
				EXPECT_EQ("None", value.debugString());
			}

			TEST_F(ScriptingValueTest, NullptrValue_ShouldBeEmpty)
			{
				const Value value(PyObjectHolder(nullptr));
				EXPECT_TRUE(value.empty());
				EXPECT_EQ("<NULL>", value.debugString());
			}

			TEST_F(ScriptingValueTest, StringValue_ShouldConvert)
			{
				const Value value(this->run("'bob'"));
				ASSERT_FALSE(value.empty());
				EXPECT_EQ("bob", value.convertToString());
				EXPECT_THROW(value.convertToNumber(), ScriptValueConversionException);
				EXPECT_THROW(value.convertToArray(), ScriptValueConversionException);
			}

			TEST_F(ScriptingValueTest, IntValue_ShouldConvert)
			{
				const Value value(this->run("29"));
				ASSERT_FALSE(value.empty());
				EXPECT_EQ(29, value.convertToNumber());
				EXPECT_THROW(value.convertToString(), ScriptValueConversionException);
				EXPECT_THROW(value.convertToArray(), ScriptValueConversionException);
			}

			TEST_F(ScriptingValueTest, EmptyArrayValue_ShouldConvert)
			{
				const Value empty(this->run("[]"));
				ASSERT_FALSE(empty.empty());
				EXPECT_THROW(empty.convertToNumber(), ScriptValueConversionException);
				EXPECT_THROW(empty.convertToString(), ScriptValueConversionException);
				ASSERT_TRUE(empty.convertToArray().empty());
			}

			TEST_F(ScriptingValueTest, DifferentArrayValues_ShouldConvert)
			{
				const Value values(this->run("[12, 13, 'bob', [99, 98]]"));
				ASSERT_FALSE(values.empty());
				EXPECT_THROW(values.convertToNumber(), ScriptValueConversionException);
				EXPECT_THROW(values.convertToString(), ScriptValueConversionException);
				const auto arr = values.convertToArray();
				ASSERT_EQ(4, arr.size());
				EXPECT_EQ(12, arr[0].convertToNumber());
				EXPECT_EQ(13, arr[1].convertToNumber());
				EXPECT_EQ("bob", arr[2].convertToString());
				const auto nestedArr = arr[3].convertToArray();
				EXPECT_EQ(99, nestedArr[0].convertToNumber());
				EXPECT_EQ(98, nestedArr[1].convertToNumber());
			}
		}
	}
}
