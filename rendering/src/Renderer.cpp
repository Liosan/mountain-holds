#include "Renderer.h"
using mh::rendering::Renderer;

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "TextureManager.h"
using mh::rendering::TextureManager;
#include "map/Map.h"
using mh::map::Map;
using mh::map::MapCoords;
#include "map/TileType.h"
using mh::map::TileType;

namespace
{
	
}

void Renderer::renderMap(sf::RenderWindow& window, const Map& map, const MapCoords& center)
{
	for (std::uint32_t x = 0; x < map.size().x; ++x)
		for (std::uint32_t y = 0; y < map.size().y; ++y)
		{
			const auto& tileType = map.typeAt(MapCoords(x, y, center.z));
			const auto& texture = TextureManager::Instance().getTexture(tileType.resource());
			sf::Sprite sprite(texture);
			sprite.setPosition(
				x * sprite.getLocalBounds().width,
				y * sprite.getLocalBounds().height
			);
			window.draw(sprite);
		}
}
