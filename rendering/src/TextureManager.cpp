#include "TextureManager.h"
using mh::rendering::TextureManager;
using mh::foundation::ResourceId;
#include "foundation/Logger.h"
using mh::foundation::Logger;

namespace
{
	std::unique_ptr<sf::Texture> loadTexture(const std::string& path)
	{
		std::unique_ptr<sf::Texture> texture(new sf::Texture());
		if (!texture->loadFromFile(path))
		{
			Logger::LogError("Failed loading texture " + path);
			return texture;
		}
		else
		{
			texture->setRepeated(false);
			texture->setSmooth(true);
			Logger::LogInfo("Loaded texture " + path);
			return texture;
		}
	}
}

TextureManager::TextureManager(const std::string& dataFolder) :
	dataFolder_(dataFolder)
{}

const sf::Texture& TextureManager::getTexture(const ResourceId& resource)
{
	if (this->textures_.count(resource) == 0)
		this->textures_[resource] = loadTexture(this->dataFolder_ + "/" + resource.filename());
	return *this->textures_[resource];
}
