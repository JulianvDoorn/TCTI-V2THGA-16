#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "GameStates.hpp"
#include "Events.hpp"
#include "AssetManager.hpp"

#define DISABLE_CUTSCENE

int main() {
	const float FPS = 60.0f;

	sf::RenderWindow window(sf::VideoMode(1280, 720, 32), "Hello");

	game = Game(window);
	
	// Load assets
	AssetManager::instance()->load("arial", "arial.ttf");

	AssetManager::instance()->loadTexture("dikkebmw", "dikkebmw.jpg");

	Statemachine statemachine(window);

	Label::setDefaultFont(AssetManager::instance()->getFont("arial"));

	// State definitions
	statemachine.registerState<GameOver>("game-over");
	statemachine.registerState<Running>("running");
	statemachine.registerState<MainMenu>("main-menu");
	statemachine.registerState<GamePauze>("game-pauze");
	statemachine.registerState<SettingsMenu>("settings-menu");
	statemachine.registerState<Cutscene>("cutscene");

	#ifdef DISABLE_CUTSCENE
		statemachine.doTransition("main-menu");
	#else
		statemachine.doTransition("cutscene");
	#endif

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