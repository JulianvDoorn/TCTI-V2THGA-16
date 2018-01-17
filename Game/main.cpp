#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Drawable.hpp"
#include "PhysicsObject.hpp"
#include "Ball.hpp"
#include "Player.hpp"
#include "EventHandler.hpp"
#include "Rectangle.hpp"

std::ostream& operator<<(std::ostream& os, sf::Vector2f v) {
	os << v.x;
	os << ", ";
	os << v.y;
	return os;
}

int main() {
	const float FPS = 60.0f;
	
	sf::RenderWindow window(sf::VideoMode(300, 900, 32), "Hello");
	window.setFramerateLimit((int) FPS);
	sf::Clock clock;




	Rectangle rectangle;
	rectangle.setSize({ 100, 50 });
	rectangle.setPosition({ 100, 100 });



	EventHandler& eventHandler0 = EventHandler::newEventHandler();
	eventHandler0.bindPhysicsObject(rectangle);

	EventHandler& eventHandler1 = EventHandler::newEventHandler();

	//std::cout << &eventHandler0 << std::endl;
	//std::cout << &eventHandler1 << std::endl;

	eventHandler0.bindFunction(EventType::KeyDown, EventFunction([](const EventSignal& signal) {
		KeyCodeArguments arguments = signal.getArguments();
		std::cout << "event handler 0" << std::endl;
	}));

	eventHandler1.bindFunction(EventType::KeyDown, EventFunction([](const EventSignal& signal) {
		KeyCodeArguments arguments = signal.getArguments();
		std::cout << "event handler 1" << std::endl;
	}));

	sf::Event ev;

	while (window.isOpen()) {
		float elapsedTime = clock.getElapsedTime().asSeconds();

		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}

			EventHandler::handleSFMLEvent(ev);
		}

		if (elapsedTime >= 1.0f / FPS) {
			window.setTitle(std::to_string(1 / elapsedTime));

			clock.restart();

			window.clear(sf::Color(0, 0, 0));
			
			rectangle.draw(window);

			window.display();
		}
	}

	return 0;
}