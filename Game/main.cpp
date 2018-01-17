#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Shapes.hpp"
#include "Player.hpp"
#include "EventHandler.hpp"

std::ostream& operator<<(std::ostream& os, sf::Vector2f v) {
	os << v.x;
	os << ", ";
	os << v.y;
	return os;
}

int main() {
	const float FPS = 60.0f; // The desired FPS. (The number of updates each second).
	bool redraw = true;      // Do I redraw everything on the screen?

	std::vector<Drawable*> drawables;
	sf::View view(sf::Vector2f(150, 10), sf::Vector2f(150, 450));

	sf::RenderWindow window(sf::VideoMode(300, 900, 32), "Hello");
	window.setFramerateLimit((int)FPS);
	sf::Clock clock;



	Rectangle floor0;
	floor0.setSize({ 600, 100 });
	floor0.setPosition({ 0, 600 });

	Rectangle floor1;
	floor1.setSize({ 60, 10 });
	floor1.setPosition({ 0, 500 });



	Player player = Player(view, window);
	EventHandler eventHandler;

	eventHandler.BindFunction(EventType::KeyDown, EventFunction([&player](EventSignal& signal) {
		KeyCodeArguments arguments = signal.getArguments();

		switch (arguments.key + 'a') {
		case 'w':
			player.doJump();
			break;
		case 'd':
			player.walk(1);
			break;
		case 'a':
			player.walk(-1);
			break;
		}
	}));

	eventHandler.BindFunction(EventType::KeyUp, EventFunction([&player](EventSignal& signal) {
		KeyCodeArguments arguments = signal.getArguments();

		switch (arguments.key + 'a') {
		case 'd':
			player.walk(0);
			break;
		case 'a':
			player.walk(0);
			break;
		}
	}));


	sf::Event ev;

	while (window.isOpen()) {
		// Wait until 1/60th of a second has passed, then update everything.
		float elapsedTime = clock.getElapsedTime().asSeconds();

		// Handle input
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) window.close();

			eventHandler.HandleSFMLEvent(ev);
		}

		if (elapsedTime >= 1.0f / FPS) {
			window.setTitle(std::to_string(1 / elapsedTime));

			clock.restart();

			window.clear(sf::Color(0, 0, 0));
			
			player.resolveCollision(floor0);
			player.resolveCollision(floor1);

			player.draw(window);
			player.update(elapsedTime);

			floor0.draw(window);
			floor1.draw(window);


			window.display();
		}
	}

	return 0;
}