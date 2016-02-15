#pragma once

#include <string>

#include "foundation_DLL_Export.hpp"

namespace mh
{
namespace foundation
{

class MH_FOUNDATION_EXPORT Logger
{
public:
	static void LogInfo(const std::string&);
	static void LogError(const std::string&);
};

}}
