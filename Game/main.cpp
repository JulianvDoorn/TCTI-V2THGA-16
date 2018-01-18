#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Drawable.hpp"
#include "PhysicsObject.hpp"
#include "Ball.hpp"
#include "Player.hpp"
#include "Rectangle.hpp"

#include "EventSource.hpp"

std::ostream& operator<<(std::ostream& os, sf::Vector2f v) {
	os << v.x;
	os << ", ";
	os << v.y;
	return os;
}

int main() {
	//const float FPS = 60.0f;
	
	//sf::RenderWindow window(sf::VideoMode(300, 900, 32), "Hello");
	//window.setFramerateLimit((int) FPS);
	//sf::Clock clock;




	//Rectangle rectangle;
	//rectangle.setSize({ 100, 50 });
	//rectangle.setPosition({ 100, 100 });


	EventSource<char> eventSource;
	
	EventConnection<char> conn0 = eventSource.connect([](char c) {
		std::cout << "1st: " << c << std::endl;
	});

	EventConnection<char> conn1 = eventSource.connect([](char c) {
		std::cout << "2nd: " << c << std::endl;
	});

	EventConnection<char> conn2 = eventSource.connect([](char c) {
		std::cout << "3rd: " << c << std::endl;
	});

	EventConnection<char> conn3 = eventSource.connect([](char c) {
		std::cout << "4th: " << c << std::endl;
	});

	EventConnection<char> conn4 = eventSource.connect([](char c) {
		std::cout << "5th: " << c << std::endl;
	});

	conn2.disconnect();
	conn0.disconnect();
	conn3.disconnect();
	conn4.disconnect();
	conn1.disconnect();

	eventSource.fire('a');

	//sf::Event ev;

	//while (window.isOpen()) {
	//	float elapsedTime = clock.getElapsedTime().asSeconds();

	//	while (window.pollEvent(ev)) {
	//		if (ev.type == sf::Event::Closed) {
	//			window.close();
	//		}

	//		EventHandler::handleSFMLEvent(ev);
	//	}

	//	if (elapsedTime >= 1.0f / FPS) {
	//		window.setTitle(std::to_string(1 / elapsedTime));

	//		clock.restart();

	//		window.clear(sf::Color(0, 0, 0));
	//		
	//		rectangle.draw(window);

	//		window.display();
	//	}
	//}

	system("pause");

	return 0;
}