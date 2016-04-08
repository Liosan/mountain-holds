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

void Renderer::renderMap(sf::RenderWindow& window, const Map& map, const MapCoords& center)
{
	for (std::uint32_t x = 0; x < map.size().x; ++x)
	{
		for (std::uint32_t y = 0; y < map.size().y; ++y)
		{
			const auto& tileType = map.typeAt(MapCoords(x, y));
			const auto& texture = TextureManager::Instance().getTexture(tileType.resource());
			sf::Sprite sprite(texture);
			const std::int32_t offsetX = static_cast<std::int32_t>(x) - static_cast<std::int32_t>(center.x);
			const std::int32_t offsetY = static_cast<std::int32_t>(y) - static_cast<std::int32_t>(center.y);
			sprite.setPosition(
				offsetX * sprite.getLocalBounds().width + window.getSize().x / 2,
				offsetY * sprite.getLocalBounds().height + window.getSize().y / 2
			);
			window.draw(sprite);
		}
	}
}
