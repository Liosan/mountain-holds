// Mountainhomes - an open-source game about dwarfs.
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

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Logger.h"
using mh::foundation::Logger;

int main(int argc, char* argv[]) 
{
	sf::Window App(sf::VideoMode(800, 600), "Mountainhomes");
	Logger::LogInfo("MH initialized");

	while (App.isOpen()) {
		sf::Event Event;
		while (App.pollEvent(Event)) {
			if (Event.type == sf::Event::Closed)
				App.close();
		}
		App.display();
	}
}
