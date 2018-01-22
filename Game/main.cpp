#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "GameStates.hpp"
#include "Events.hpp"
#include "AssetManager.hpp"

int main() {

	const float FPS = 60.0f;

	sf::RenderWindow window(sf::VideoMode(1280, 720, 32), "Hello");

	game = Game(window);

	game.assets.load("arial", "arial.ttf");
	game.assets.load("game-over", "GameOver.wav");

	Statemachine statemachine(window);

	Button::setDefaultFont(game.assets.getFont("arial"));

	// State definitions

	GameOver gameOver(statemachine);
	Running running(statemachine);
	MainMenu mainMenu(statemachine);
	GamePauze gamePauzeMenu(statemachine);


	statemachine.doTransition("main-menu"); // initial state

	// End state definitions

	
	window.setFramerateLimit((int) FPS);
	sf::Clock clock;

	sf::Event ev;

	while (window.isOpen()) {
		float elapsedTime = clock.getElapsedTime().asSeconds();

		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}

			game.decodeSFMLEvent(ev);
		}

		if (elapsedTime >= 1.0f / FPS) {
			window.setTitle(std::to_string(1 / elapsedTime));
			clock.restart();
			statemachine.update(elapsedTime);
		}
	}

	return 0;
}