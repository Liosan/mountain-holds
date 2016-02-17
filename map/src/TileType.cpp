#include "TileType.h"
using mh::map::TileType;
using mh::map::TileTypeId;
using mh::map::TileTypeDictionary;
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

TileTypeDictionary::TileTypeDictionary()
{}

const TileType& TileTypeDictionary::add(const ResourceId& resource)
{
	this->tileTypes_.push_back(
		TileType(static_cast<TileTypeId>(this->tileTypes_.size()), resource)
	);
	return this->tileTypes_.back();
}

const TileType& TileTypeDictionary::get(const TileTypeId& id) const
{
	MH_ASSERT(id < this->tileTypes_.size(), "id supplied is out of range");
	return this->tileTypes_[id];
}

std::uint16_t TileTypeDictionary::size() const
{
	return static_cast<std::uint16_t>(this->tileTypes_.size());
}
