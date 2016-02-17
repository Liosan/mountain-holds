#pragma once

#include <cstdint>

#include "map_DLL_Export.hpp"

#include "foundation/ResourceId.h"

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
	}
}
