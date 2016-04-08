#include "rendering.h"

#include "TextureManager.h"
using mh::rendering::TextureManager;

void mh::rendering::initializeRendering(const std::string& dataFolder)
{
	TextureManager::Initialize(dataFolder);
}
