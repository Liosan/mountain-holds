#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "map_DLL_Export.hpp"

#include "TileType.h"

namespace mh
{
	namespace map
	{
		class MapCoords : public sf::Vector2<std::uint32_t>
		{
		public:
			MapCoords(const std::uint32_t x, const std::uint32_t y) :
				sf::Vector2<std::uint32_t>(x, y)
			{}
		};

		class MH_MAP_EXPORT Map
		{
		public:
			Map(const MapCoords& size, const TileTypeId& initialTileType);
			
			const TileTypeId& typeIdAt(const MapCoords& point) const;
			const TileType& typeAt(const MapCoords& point) const;
			void set(const MapCoords& point, const TileTypeId& tileTypeId);
			const MapCoords& size() const;
		private:
			const MapCoords size_;
			std::vector<std::vector<TileTypeId>> tiles_;
		};
	}
}
