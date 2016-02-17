#pragma once

#include "foundation_DLL_Export.hpp"

#include <string>

namespace mh
{
	namespace foundation
	{
		/// resource-identifying string, like h.jpg core/foo.png:12:15
		class MH_FOUNDATION_EXPORT ResourceId
		{
		public:
			explicit ResourceId(const std::string& text);

			bool operator<(const ResourceId& other) const;
			bool operator==(const ResourceId& other) const;

			const std::string& filename() const;
		private:
			const std::string text_;
		};
	}
}