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
