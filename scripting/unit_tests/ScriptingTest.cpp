#include <gtest/gtest.h>

#include <string>
using std::string;

#include "scripting.h"

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
				ASSERT_EQ(1, loadedModules().size());
				EXPECT_EQ(0, loadedModules()[0]) << "Root module should have id 0";
			}

			TEST_F(ScriptingTest, DoubleInitialize_ShouldThrow)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "moduleA");
				EXPECT_THROW(initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "moduleA"), ScriptLoadingException);
			}

			TEST_F(ScriptingTest, InitializeNonExistentModule_ShouldThrow)
			{
				EXPECT_THROW(initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "moduleA"), ScriptLoadingException);
			}

			TEST_F(ScriptingTest, AfterFinalize_ShouldHaveZeroModules)
			{
				initializeScripting(SCRIPTING_TEST_DATA_DIRECTORY, "moduleA");
				finalizeScripting();
				EXPECT_EQ(0, loadedModules().size());
			}

			TEST_F(ScriptingTest, FinalizeWithoutInitialize_ShouldThrow)
			{
				EXPECT_THROW(finalizeScripting(), std::logic_error);
			}
		}
	}
}
