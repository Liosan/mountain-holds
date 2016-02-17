#pragma once

#include <vector>

#include <SFML/System/Vector3.hpp>

#include "map_DLL_Export.hpp"

#include "TileType.h"

namespace mh
{
	namespace map
	{
		class MH_MAP_EXPORT Map
		{
		public:
			Map(const sf::Vector3i& size, const TileTypeId& initialTileType);
			
			const TileType& typeAt(const sf::Vector3i& point) const;
			void set(const sf::Vector3i& point, const TileTypeId& tileTypeId);
			const sf::Vector3i& size() const;
		private:
			const sf::Vector3i size_;
			std::vector<std::vector<std::vector<TileTypeId>>> tiles_;
		};
	}
}
