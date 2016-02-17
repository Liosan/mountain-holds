#include "Map.h"
using mh::map::Map;
using mh::map::TileTypeId;
using mh::map::TileType;
using mh::map::MapCoords;
#include "TileTypeDictionary.h"
using mh::map::TileTypeDictionary;
#include "foundation/debug.h"

Map::Map(const MapCoords& size, const TileTypeId& initialTileType) :
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

const TileTypeId& Map::typeIdAt(const MapCoords& point) const
{
	MH_ASSERT(
		(point.x < this->size_.x) && (point.y < this->size_.y) && (point.z < this->size_.z),
		"Point out of range"
	);
	return this->tiles_[point.x][point.y][point.z];
}

const TileType& Map::typeAt(const MapCoords& point) const
{
	MH_ASSERT(
		(point.x < this->size_.x) && (point.y < this->size_.y) && (point.z < this->size_.z),
		"Point out of range"
	);
	return TileTypeDictionary::Instance().get(this->typeIdAt(point));
}

void Map::set(const MapCoords& point, const TileTypeId& tileTypeId)
{
	MH_ASSERT(
		(point.x < this->size_.x) && (point.y < this->size_.y) && (point.z < this->size_.z),
		"Point out of range"
	);
	this->tiles_[point.x][point.y][point.z] = tileTypeId;
}

const MapCoords& Map::size() const
{
	return this->size_;
}
