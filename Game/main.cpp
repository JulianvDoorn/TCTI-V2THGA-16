#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Drawable.hpp"
#include "PhysicsObject.hpp"
#include "Ball.hpp"
#include "Player.hpp"
#include "Button.hpp"

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
	sf::View view2(sf::Vector2f(150, 10), sf::Vector2f(150, 450));

    enum class GameStates {
        START_MENU,
        MAIN_GAME,
    };

    GameStates gameState = GameStates::START_MENU;

	sf::RenderWindow window(sf::VideoMode(1280, 720, 32), "Hello");
	window.setFramerateLimit((int) FPS);
	sf::Clock clock;



	Ball ball = Ball();
	ball.setRadius(10);
	ball.setPosition({ 150, 500 });
	ball.setFillColor(sf::Color(255, 0, 0));


	Player player = Player(view2, window);

    Button startButton = Button({300,100},{640,360}, 30,"start game");

	sf::Event ev;

	while (window.isOpen()) {
		// Wait until 1/60th of a second has passed, then update everything.
		float elapsedTime = clock.getElapsedTime().asSeconds();

		// Handle input
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) window.close();
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
                    //			player.draw(window);
                    //			player.update(elapsedTime);
                    //     ball.update(elapsedTime);
                    break;
            }
            window.display();
		}
	}

	return 0;
}