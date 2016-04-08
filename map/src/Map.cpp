#include "Map.h"
using mh::map::Map;
using mh::map::TileTypeId;
using mh::map::TileType;
using mh::map::MapCoords;
using std::unique_ptr;
#include "foundation/ResourceId.h"
using mh::foundation::ResourceId;
#include "foundation/debug.h"
#include "scripting/scripting.h"
using mh::scripting::invoke;
using mh::scripting::CoreModuleId;
#include "scripting/Value.h"
using mh::scripting::Value;
#include "TileTypeDictionary.h"
using mh::map::TileTypeDictionary;

void mh::map::initializeMaps()
{
	TileTypeDictionary::Initialize();
	const auto tileTypes = invoke(CoreModuleId, "loadTileTypes").convertToArray();
	for (const auto& tileType : tileTypes)
		TileTypeDictionary::Instance().add(ResourceId(tileType.convertToString()));
}

Map::Map(const MapCoords& size, const TileTypeId& initialTileType) :
	size_(size)
{
	this->tiles_.resize(size.x);
	for (auto& col : this->tiles_)
	{
		col.resize(size.y, initialTileType);
	}
}

const TileTypeId& Map::typeIdAt(const MapCoords& point) const
{
	MH_ASSERT(
		(point.x < this->size_.x) && (point.y < this->size_.y),
		"Point out of range"
	);
	return this->tiles_[point.x][point.y];
}

const TileType& Map::typeAt(const MapCoords& point) const
{
	MH_ASSERT(
		(point.x < this->size_.x) && (point.y < this->size_.y),
		"Point out of range"
	);
	return TileTypeDictionary::Instance().get(this->typeIdAt(point));
}

void Map::set(const MapCoords& point, const TileTypeId& tileTypeId)
{
	MH_ASSERT(
		(point.x < this->size_.x) && (point.y < this->size_.y),
		"Point out of range"
	);
	this->tiles_[point.x][point.y] = tileTypeId;
}

const MapCoords& Map::size() const
{
	return this->size_;
}

unique_ptr<Map> mh::map::generateMapFromScript()
{
	const auto mapData = invoke(CoreModuleId, "generateMap").convertToArray();
	const uint32_t sizeX = static_cast<uint32_t>(mapData.size());
	const uint32_t sizeY = static_cast<uint32_t>(mapData[0].convertToArray().size());
	auto map = std::make_unique<Map>(MapCoords(sizeX, sizeY), 0);
	for (uint32_t x = 0; x < sizeX; ++x)
		for (uint32_t y = 0; y < sizeY; ++y)
			map->set(
				MapCoords(x, y),
				static_cast<uint32_t>(mapData[x].convertToArray()[y].convertToNumber())
			);
	return map;
}
