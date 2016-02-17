#pragma once

#include <vector>

#include "map_DLL_Export.hpp"

#include "TileType.h"
#include "foundation/Singleton.hpp"

namespace mh
{
	namespace map
	{
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
