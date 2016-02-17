#pragma once

#include "rendering_DLL_Export.hpp"

namespace sf
{
	class RenderWindow;
}

namespace mh
{
	namespace map
	{
		class MapCoords;
		class Map;
	}

	namespace rendering
	{
		class MH_RENDERING_EXPORT Renderer
		{
		public:
			void renderMap(sf::RenderWindow& window, const map::Map& map, const map::MapCoords& center);
		};
	}
}
