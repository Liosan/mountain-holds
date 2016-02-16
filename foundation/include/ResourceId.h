#pragma once

#include "foundation_DLL_Export.hpp"

#include <string>

namespace mh
{
	namespace foundation
	{
		/// resource-identifying string, like h.jpg core/foo.png:12:15
		class ResourceId
		{
		public:
			explicit ResourceId(const std::string& text);
		private:
			const std::string text_;
		};
	}
}