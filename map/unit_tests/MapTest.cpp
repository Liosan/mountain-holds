#include <gtest/gtest.h>

#include <string>
using std::string;

#include "Map.h"
using mh::map::Map;
using mh::map::TileType;
using mh::map::TileTypeId;
#include "TileTypeDictionary.h"
using mh::foundation::ResourceId;
using mh::map::TileTypeDictionary;

namespace
{

	const TileTypeId emptyTile = 0;
	const TileTypeId someTile = 1;

}

namespace mh
{
	namespace foundation
	{
		namespace test
		{
			struct MapTest : ::testing::Test
			{
			public:
				virtual void SetUp() override
				{
					TileTypeDictionary::Initialize();
					TileTypeDictionary::Instance().add(ResourceId(""));
					TileTypeDictionary::Instance().add(ResourceId("foo.png"));
				}

				virtual void TearDown() override
				{
					TileTypeDictionary::Destroy();
				}
			};

			TEST_F(MapTest, Empty_ShouldHaveProperSize)
			{
				const Map map(sf::Vector3i(2, 4, 6), emptyTile);
				EXPECT_EQ(sf::Vector3i(2, 4, 6), map.size());
			}

			TEST_F(MapTest, Empty_ShouldHaveProperTiles)
			{
				const Map map(sf::Vector3i(2, 4, 6), emptyTile);
				EXPECT_EQ(emptyTile, map.typeAt(sf::Vector3i(0, 0, 0)).id());
				EXPECT_EQ(emptyTile, map.typeAt(sf::Vector3i(1, 3, 5)).id());
			}

			TEST_F(MapTest, SingleTileSet_ShouldProperTileType)
			{
				Map map(sf::Vector3i(2, 4, 6), emptyTile);
				map.set(sf::Vector3i(1, 2, 3), someTile);
				EXPECT_EQ(someTile, map.typeAt(sf::Vector3i(1, 2, 3)).id());
			}
		}
	}
}
