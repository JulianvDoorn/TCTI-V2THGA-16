#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "GameStates.hpp"
#include "Events.hpp"
#include "AssetManager.hpp"

int main() {
	// Game constants.
	const float FPS = 60.0f;
	const bool ENABLE_CUTSCENE = true;

	sf::RenderWindow window(sf::VideoMode(1280, 720, 32), "Hello");

	window.setFramerateLimit(static_cast<unsigned int>(FPS));

	game = Game(window);

	// Load assets.
	AssetManager::instance()->load("arial", "arial.ttf");

	// Create an new statemachine.
	Statemachine statemachine(window);

	// Set the default font.
	Label::setDefaultFont(AssetManager::instance()->getFont("arial"));

	// State definitions.
	statemachine.registerState<GameOver>("game-over");
	statemachine.registerState<Running>("running");
	statemachine.registerState<MainMenu>("main-menu");
	statemachine.registerState<GamePauze>("game-pauze");
	statemachine.registerState<SettingsMenu>("settings-menu");
	statemachine.registerState<Cutscene>("cutscene");

	if (ENABLE_CUTSCENE) {
		statemachine.doTransition("cutscene");
	} else {
		statemachine.doTransition("main-menu");
	}

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