#pragma once

#include "map_DLL_Export.hpp"

#include <cstdint>
#include <vector>

#include "ResourceId.h"
#include "Singleton.hpp"

namespace mh
{
	namespace map
	{
		typedef std::uint16_t TileTypeId;

		class MH_MAP_EXPORT TileType
		{
		public:
			TileType(
				const TileTypeId& id,
				const foundation::ResourceId& resource
			);

			TileTypeId id() const;
			const foundation::ResourceId& resource() const; 
		private:
			const TileTypeId id_;
			const foundation::ResourceId resource_;
		};

		class MH_MAP_EXPORT TileTypeDictionary : public foundation::Singleton<TileTypeDictionary>
		{
			friend class foundation::Singleton<TileTypeDictionary>;
		public:
			const TileType& add(const foundation::ResourceId& resource);
			const TileType& get(const TileTypeId& id) const;
			std::uint16_t size() const;
		private:
			TileTypeDictionary();

			std::vector<TileType> tileTypes_;
		};
	}
}
