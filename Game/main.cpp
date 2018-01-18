#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Drawable.hpp"
#include "PhysicsObject.hpp"
#include "Ball.hpp"
#include "Player.hpp"
#include "Button.hpp"
#include "Rectangle.hpp"
#include "EventSource.hpp"

std::ostream& operator<<(std::ostream& os, sf::Vector2f v) {
	os << v.x;
	os << ", ";
	os << v.y;
	return os;
}

int main() {
	const float FPS = 60.0f;

	sf::RenderWindow window(sf::VideoMode(1280, 720, 32), "Hello");
	window.setFramerateLimit((int) FPS);
	sf::Clock clock;


	//Rectangle rectangle;
	//rectangle.setSize({ 100, 50 });
	//rectangle.setPosition({ 100, 100 });


	EventSource<char> eventSource;

	EventConnection<char> conn0 = eventSource.connect([](char c) {
		std::cout << "1st: " << c << std::endl;
	});
	enum class GameStates {
		START_MENU,
		MAIN_GAME,
	};

	GameStates gameState = GameStates::START_MENU;


	Ball ball = Ball();
	ball.setRadius(10);
	ball.setPosition({ 150, 500 });
	ball.setFillColor(sf::Color(255, 0, 0));

	sf::Font font;
	font.loadFromFile("arial.ttf");
	Button startButton = Button({ 300,100 }, { 640,360 }, 30, "start game", font);

	startButton.buttonPressed.connect([]() {
		std::cout << "Button start pressed!";
	});

	startButton.buttonReleased.connect([]() {
		std::cout << "Button start released!";
	});

	sf::Event ev;

	while (window.isOpen()) {
		// Wait until 1/60th of a second has passed, then update everything.
		float elapsedTime = clock.getElapsedTime().asSeconds();

		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) window.close();

			if (ev.type == sf::Event::MouseButtonPressed) startButton.buttonPressed.fire();
			if (ev.type == sf::Event::MouseButtonReleased) startButton.buttonReleased.fire();

			//eventHandler.HandleSFMLEvent(ev);
		}

		if (elapsedTime >= 1.0f / FPS) {
			window.setTitle(std::to_string(1 / elapsedTime));

			clock.restart();

			window.clear(sf::Color(0, 0, 0));
			switch (gameState) {
			case GameStates::START_MENU:
				startButton.draw(window);
				break;
			case GameStates::MAIN_GAME:
				ball.draw(window);
				break;
			}
			window.display();
		}


	}

	return 0;
}