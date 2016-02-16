#pragma once

#include "map_DLL_Export.hpp"

#include <cstdint>

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
			TileTypeId id() const;
			foundation::ResourceId resource() const; 
		};

		class MH_MAP_EXPORT TileTypeDictionary : public foundation::Singleton<TileTypeDictionary>
		{
			friend class foundation::Singleton<TileTypeDictionary>;
		public:
		private:
			TileTypeDictionary();
		};
	}
}
