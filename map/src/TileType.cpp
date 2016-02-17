#include "TileType.h"
using mh::map::TileType;
using mh::map::TileTypeId;
using mh::foundation::ResourceId;

TileType::TileType(
	const TileTypeId& id,
	const ResourceId& resource
) :
	id_(id),
	resource_(resource)
{}

TileTypeId TileType::id() const
{
	return this->id_;
}

const ResourceId& TileType::resource() const
{
	return this->resource_;
}
