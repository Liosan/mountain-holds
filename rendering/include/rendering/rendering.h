#pragma once

#include <string>
#include "rendering_DLL_Export.hpp"

namespace mh
{
	namespace rendering
	{
		/// Loads necessary rendering-data from all data modules, enabling entities to be rendered later
		void MH_RENDERING_EXPORT initializeRendering(const std::string& dataFolder);
	}
}
