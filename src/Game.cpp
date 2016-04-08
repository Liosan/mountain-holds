#include "Game.h"
using mh::app::Game;
#include "foundation/ResourceId.h"
using mh::foundation::ResourceId;
#include "map/TileTypeDictionary.h"
using mh::map::TileTypeDictionary;
#include "map/Map.h"
using mh::map::Map;
using mh::map::MapCoords;
#include "rendering/TextureManager.h"
using mh::rendering::TextureManager;
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

	// TODO should be per-library initialization functions

	TileTypeDictionary::Initialize();
	const auto tileTypes = invoke(CoreModuleId, "loadTileTypes").convertToArray();
	for (const auto& tileType : tileTypes)
		TileTypeDictionary::Instance().add(ResourceId(tileType.convertToString()));
	
	TextureManager::Initialize(dataFolder);

	const auto mapData = invoke(CoreModuleId, "generateMap").convertToArray();
	const uint32_t sizeX = static_cast<uint32_t>(mapData.size());
	const uint32_t sizeY = static_cast<uint32_t>(mapData[0].convertToArray().size());
	this->map_ = std::make_unique<Map>(MapCoords(sizeX, sizeY), 0);
	for (uint32_t x = 0; x < sizeX; ++x)
		for (uint32_t y = 0; y < sizeY; ++y)
			this->map_->set(
				MapCoords(x, y), 
				static_cast<uint32_t>(mapData[x].convertToArray()[y].convertToNumber())
			);
	this->renderer_ = std::make_unique<Renderer>();
	this->mapCenter_ = std::make_unique<MapCoords>(sizeX / 2, sizeY / 2);
}

Game::~Game()
{}

void Game::update(sf::RenderWindow& window)
{
	this->renderer_->renderMap(window, *this->map_, *this->mapCenter_);
}

