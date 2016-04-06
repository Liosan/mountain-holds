#include <gtest/gtest.h>

#include <string>
using std::string;

#include <Python.h>

#include "PyObjectHolder.h"
#include "PythonUtils.h"

namespace mh
{
	namespace scripting
	{
		namespace test
		{
			struct PyObjectHolderTest : ::testing::Test
			{
			public:
				virtual void SetUp() override
				{
					Py_Initialize();
				}

				virtual void TearDown() override
				{
					Py_Finalize();
				}
			};

			TEST_F(PyObjectHolderTest, Empty_ShouldBeNull)
			{
				const PyObjectHolder empty;
				EXPECT_EQ(nullptr, empty);
			}

			TEST_F(PyObjectHolderTest, CopyingEmpty_ShouldNotThrow)
			{
				const PyObjectHolder empty;
				const PyObjectHolder copy(empty);
				EXPECT_EQ(nullptr, empty);
				EXPECT_EQ(nullptr, copy);
			}

			TEST_F(PyObjectHolderTest, AssignmentEmpty_ShouldNotThrow)
			{
				const PyObjectHolder empty;
				PyObjectHolder copy;
				copy = empty;
				EXPECT_EQ(nullptr, empty);
				EXPECT_EQ(nullptr, copy);
			}

			TEST_F(PyObjectHolderTest, MovingEmpty_ShouldNotThrow)
			{
				PyObjectHolder empty;
				const PyObjectHolder copy(std::move(empty));
				EXPECT_EQ(nullptr, empty);
				EXPECT_EQ(nullptr, copy);
			}

			TEST_F(PyObjectHolderTest, MoveAssignmentEmpty_ShouldNotThrow)
			{
				PyObjectHolder empty;
				PyObjectHolder copy;
				copy = std::move(empty);
				EXPECT_EQ(nullptr, empty);
				EXPECT_EQ(nullptr, copy);
			}

			TEST_F(PyObjectHolderTest, Copying_ShouldNotThrow)
			{
				const PyObjectHolder str(Py_BuildValue("s", "foo"));
				const PyObjectHolder copy(str);
				ASSERT_NO_THROW(checkPythonError());
				EXPECT_NE(nullptr, copy);
			}

			TEST_F(PyObjectHolderTest, Assignment_ShouldNotThrow)
			{
				const PyObjectHolder str(Py_BuildValue("s", "foo"));
				PyObjectHolder copy;
				copy = str;
				ASSERT_NO_THROW(checkPythonError());
				EXPECT_NE(nullptr, copy);
			}

			TEST_F(PyObjectHolderTest, Moving_ShouldNotThrow)
			{
				PyObjectHolder str(Py_BuildValue("s", "foo"));
				const PyObjectHolder copy(std::move(str));
				ASSERT_NO_THROW(checkPythonError());
				EXPECT_EQ(nullptr, str);
				EXPECT_NE(nullptr, copy);
			}

			TEST_F(PyObjectHolderTest, MoveAssignment_ShouldNotThrow)
			{
				PyObjectHolder str(Py_BuildValue("s", "foo"));
				PyObjectHolder copy;
				copy = std::move(str);
				ASSERT_NO_THROW(checkPythonError());
				EXPECT_EQ(nullptr, str);
				EXPECT_NE(nullptr, copy);
			}
		}
	}
}
