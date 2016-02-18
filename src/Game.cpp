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

Game::Game(const std::string& dataFolder)
{
	// TODO should be per-library initialization functions
	TileTypeDictionary::Initialize();
	TileTypeDictionary::Instance().add(ResourceId("grass.png"));
	TileTypeDictionary::Instance().add(ResourceId("dirt.png"));

	TextureManager::Initialize(dataFolder);
	TextureManager::Instance().getTexture(ResourceId("grass.png"));
	TextureManager::Instance().getTexture(ResourceId("grass.png"));

	this->map_ = std::make_unique<Map>(MapCoords(10, 10, 10), 0);
	this->map_->set(MapCoords(3, 3, 5), 1);
	this->renderer_ = std::make_unique<Renderer>();
	this->mapCenter_ = std::make_unique<MapCoords>(5, 5, 5);
}

Game::~Game()
{}

void Game::update(sf::RenderWindow& window)
{
	this->renderer_->renderMap(window, *this->map_, *this->mapCenter_);
}

