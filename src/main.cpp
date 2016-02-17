// Mountainhomes - an open-source game about dwarfs.
// Copyright(C) 2016 Szymon Kurek
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see http://www.gnu.org/licenses/.

#include <iostream>
#include <SFML/Graphics.hpp>

#include "foundation/Logger.h"
using mh::foundation::Logger;
#include "foundation/ResourceId.h"
using mh::foundation::ResourceId;
#include "map/TileTypeDictionary.h"
using mh::map::TileTypeDictionary;
#include "rendering/TextureManager.h"
using mh::rendering::TextureManager;

namespace
{

	void initialize(const std::string& dataFolder)
	{
		// TODO should be per-library initialization functions
		TileTypeDictionary::Initialize();
		TileTypeDictionary::Instance().add(ResourceId("grass.png"));
		TileTypeDictionary::Instance().add(ResourceId("dirt.png"));

		TextureManager::Initialize(dataFolder);
		TextureManager::Instance().getTexture(ResourceId("grass.png"));
		TextureManager::Instance().getTexture(ResourceId("grass.png"));
	}

}

int main(int argc, char* argv[]) 
{
	sf::Window app(sf::VideoMode(800, 600), "Mountainhomes");
	Logger::LogInfo("This program comes with ABSOLUTELY NO WARRANTY; see http://www.gnu.org/licenses/gpl-3.0.en.html for details.");
	Logger::LogInfo("This is free software, and you are welcome to redistribute it under certain conditions.");

	if (argc < 2)
	{
		Logger::LogError(
			std::string("Not enough arguments supplied. Usage: ") + argv[0] + " <path to data folder>"
		);
		return 1;
	}
	const std::string dataFolder = argv[1];
	initialize(dataFolder);
	Logger::LogInfo("MH initialized");

	while (app.isOpen()) {
		sf::Event event;
		while (app.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				app.close();
		}
		app.display();
	}
}
