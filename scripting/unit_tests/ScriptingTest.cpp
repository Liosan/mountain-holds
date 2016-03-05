#include <gtest/gtest.h>

#include <string>
using std::string;

#include "scripting.h"
#include "Value.h"

#include <Python.h>

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
			// TODO test invoke -> return list, nested list
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
			}

			TEST_F(ScriptingTest, InvokeFunctionReturningString_ShouldRetrieveString)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "core");
				const auto result = invoke(CoreModuleId, "returnString");
				EXPECT_FALSE(result.empty());
				EXPECT_EQ("I am cow", result.convertToString());
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
		}
	}
}
