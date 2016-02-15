#include "Logger.h"
using mh::foundation::Logger;

#include <iostream>

void Logger::LogInfo(const std::string& text)
{
	std::cout << text << std::endl;
}

void Logger::LogError(const std::string& text)
{
	std::cout << text << std::endl;
}