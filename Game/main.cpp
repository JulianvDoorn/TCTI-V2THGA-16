#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Shapes.hpp"
#include "Player.hpp"
#include "Button.hpp"
#include "Rectangle.hpp"
#include "EventSource.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Antagonist.hpp"
#include "CollisionObjects.hpp"

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

	sf::View view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720));

	Mouse mouse = Mouse(window);
	Keyboard keyboard(window);

	CollisionObjects objects;

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
	Player player = Player(view, window, keyboard);

	Antagonist death = Antagonist(window);
	objects.add(death);

	Rectangle floor0;
	floor0.setSize({ 600, 100 });
	floor0.setPosition({ 0, 600 });
	objects.add(floor0);

	Rectangle floor1;
	floor1.setSize({ 60, 10 });
	floor1.setPosition({ 0, 500 });
	objects.add(floor1);

	Rectangle wall;
	wall.setSize({ 20, 50 });
	wall.setPosition({ 250, 550 });
	objects.add(wall);


	Ball ball = Ball();
	ball.setRadius(10);
	ball.setPosition({ 150, 500 });
	ball.setFillColor(sf::Color(255, 0, 0));

	sf::Font font;
	font.loadFromFile("arial.ttf");




	Button startButton = Button({ 300,100 }, { 640,360 }, 30, "start game", font, mouse);

	startButton.buttonReleased.connect([&gameState]() {
		gameState = GameStates::MAIN_GAME;
	});

	sf::Event ev;

	while (window.isOpen()) {
		// Wait until 1/60th of a second has passed, then update everything.
		float elapsedTime = clock.getElapsedTime().asSeconds();

		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) window.close();

			mouse.decodeSFMLEvent(ev);
			keyboard.decodeSFMLEvent(ev);
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
				//ball.draw(window);

				player.update(elapsedTime);
				death.update(elapsedTime);

				player.detectCollision(objects);

				//player.resolveCollision(floor0);
				//player.resolveCollision(floor1);
				//player.resolveCollision(wall);
				//player.deathByAntagonist(death);

				player.draw(window);
				death.draw(window);


				floor0.draw(window);
				floor1.draw(window);
				wall.draw(window);


				break;
			}


			window.display();
		}


	}

	return 0;
}