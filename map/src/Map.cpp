#include "Map.h"
using mh::map::Map;
using mh::map::TileTypeId;
using mh::map::TileType;
using sf::Vector3i;
#include "TileTypeDictionary.h"
using mh::map::TileTypeDictionary;
#include "foundation/debug.h"

Map::Map(const sf::Vector3i& size, const TileTypeId& initialTileType) :
	size_(size)
{
	this->tiles_.resize(size.x);
	for (auto& col : this->tiles_)
	{
		col.resize(size.y);
		for (auto& slice : col)
			slice.resize(size.z, initialTileType);
	}
}

const TileType& Map::typeAt(const sf::Vector3i& point) const
{
	MH_ASSERT(
		(point.x < this->size_.x) && (point.y < this->size_.y) && (point.z < this->size_.z),
		"Point out of range"
	);
	return TileTypeDictionary::Instance().get(this->tiles_[point.x][point.y][point.z]);
}

void Map::set(const sf::Vector3i& point, const TileTypeId& tileTypeId)
{
	MH_ASSERT(
		(point.x < this->size_.x) && (point.y < this->size_.y) && (point.z < this->size_.z),
		"Point out of range"
	);
	this->tiles_[point.x][point.y][point.z] = tileTypeId;
}

const sf::Vector3i& Map::size() const
{
	return this->size_;
}