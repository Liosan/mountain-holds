#pragma once

#include <map>

#include <SFML/Graphics/Texture.hpp>

#include "rendering_DLL_Export.hpp"

#include "foundation/Singleton.hpp"
#include "foundation/ResourceId.h"

namespace mh
{
	namespace rendering
	{
		class MH_RENDERING_EXPORT TextureManager : public foundation::Singleton<TextureManager>
		{
			friend class foundation::Singleton<TextureManager>;
		public:
			const sf::Texture& getTexture(const foundation::ResourceId& resource);
		private:
			TextureManager(const std::string& dataFolder);

			const std::string dataFolder_;
			std::map<foundation::ResourceId, std::unique_ptr<sf::Texture>> textures_;
		};
	}
}