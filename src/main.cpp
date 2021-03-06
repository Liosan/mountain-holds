// MountainHolds - an open-source 4X strategy game in a unique setting.
// Copyright(C) 2016 Szymon Kurek
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see http://www.gnu.org/licenses/.

#include <string>
using std::string;

#include "foundation/Logger.h"
using mh::foundation::Logger;
#include "Game.h"
using mh::app::Game;

int main(int argc, char* argv[]) 
{
	try
	{
		if (argc < 2)
		{
			Logger::LogError(
				string("Not enough arguments supplied. Usage: ") + argv[0] + " <path to data folder>"
			);
			return 1;
		}

		Logger::LogInfo("This program comes with ABSOLUTELY NO WARRANTY; see http://www.gnu.org/licenses/gpl-3.0.en.html for details.");
		Logger::LogInfo("This is free software, and you are welcome to redistribute it under certain conditions.");

		Game game(argv[1]);
		return game.run();
	}
	catch (const std::exception& ex)
	{
		Logger::LogError(string("FATAL: unhandled exception:") + ex.what());
		return 1;
	}
}
