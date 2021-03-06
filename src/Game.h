#pragma once

#include <memory>
#include <string>

namespace sf
{
	class RenderWindow;
}

namespace mh
{
	namespace map
	{
		class Map;
		class MapCoords;
	}
	namespace rendering
	{
		class Renderer;
	}

	namespace app
	{
		class Game
		{
		public:
			explicit Game(const std::string& dataFolder);
			~Game();

			int32_t run();
		private:
			void update(sf::RenderWindow& window);

			std::unique_ptr<map::MapCoords> mapCenter_;
			std::unique_ptr<map::Map> map_;
			std::unique_ptr<rendering::Renderer> renderer_;
		};
	}
}