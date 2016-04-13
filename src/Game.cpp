#include "Game.h"
using mh::app::Game;

#include <SFML/Graphics.hpp>

#include "foundation/Logger.h"
using mh::foundation::Logger;
#include "map/TileTypeDictionary.h"
using mh::map::TileTypeDictionary;
#include "map/Map.h"
using mh::map::Map;
using mh::map::MapCoords;
using mh::map::initializeMaps;
using mh::map::generateMapFromScript;
#include "rendering/rendering.h"
using mh::rendering::initializeRendering;
#include "rendering/Renderer.h"
using mh::rendering::Renderer;
#include "scripting/scripting.h"
using mh::scripting::initializeScripting;
using mh::scripting::invoke;
using mh::scripting::CoreModuleId;
#include "scripting/Value.h"
using mh::scripting::Value;

Game::Game(const std::string& dataFolder)
{
	initializeScripting(dataFolder, "mh-core");
	initializeMaps();
	this->map_ = generateMapFromScript();
	initializeRendering(dataFolder);
	this->renderer_ = std::make_unique<Renderer>();
	this->mapCenter_ = std::make_unique<MapCoords>(this->map_->size().x / 2, this->map_->size().y / 2);
}

Game::~Game()
{}

int32_t Game::run()
{
	sf::RenderWindow app(sf::VideoMode(800, 600), "Mountain Holds");

	Logger::LogInfo("MH initialized");

	while (app.isOpen())
	{
		sf::Event event;
		while (app.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				app.close();
		}
		app.clear();
		this->update(app);
		app.display();
	}
	return 0;
}

void Game::update(sf::RenderWindow& window)
{
	this->renderer_->renderMap(window, *this->map_, *this->mapCenter_);
}

