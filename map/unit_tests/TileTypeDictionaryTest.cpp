#include <gtest/gtest.h>

#include <string>
using std::string;

#include "TileType.h"
using mh::map::TileType;
using mh::map::TileTypeDictionary;
using mh::foundation::ResourceId;

namespace mh
{
	namespace foundation
	{
		namespace test
		{
			struct TileTypeDictionaryTest : ::testing::Test
			{
			public:
				virtual void SetUp() override
				{
					TileTypeDictionary::Initialize();
				}

				virtual void TearDown() override
				{
					TileTypeDictionary::Destroy();
				}
			};

			TEST_F(TileTypeDictionaryTest, Empty)
			{
				EXPECT_EQ(0, TileTypeDictionary::Instance().size());
			}

			TEST_F(TileTypeDictionaryTest, SingleTile)
			{
				TileTypeDictionary::Instance().add(ResourceId("foo.png"));
				ASSERT_EQ(1, TileTypeDictionary::Instance().size());
				EXPECT_EQ(ResourceId("foo.png"), TileTypeDictionary::Instance().get(0).resource());
			}

			TEST_F(TileTypeDictionaryTest, ManyTiles)
			{
				const auto& t1 = TileTypeDictionary::Instance().add(ResourceId("foo.png"));
				EXPECT_EQ(0, t1.id());
				const auto& t2 = TileTypeDictionary::Instance().add(ResourceId("bar.png"));
				EXPECT_EQ(1, t2.id());
				const auto& t3 = TileTypeDictionary::Instance().add(ResourceId("foo.png"));
				EXPECT_EQ(2, t3.id());
				ASSERT_EQ(3, TileTypeDictionary::Instance().size());
			}

		}
	}
}
