#pragma once

#include <SFML\Graphics.hpp>

#include "Events.hpp"

class FreeCamera {
	sf::View view;
	sf::RenderWindow& window;

	float speed;
	sf::Vector2i movement = { 0, 0 };
	sf::Vector2f position = { 0, 0 };

	EventConnection keyPressedConnection;
	EventConnection keyReleasedConnection;

public:
	FreeCamera(sf::RenderWindow& window) : view(sf::Vector2f(), static_cast<sf::Vector2f>(window.getSize())), window(window) { }

	FreeCamera(sf::RenderWindow& window, float cameraSpeed) : view(sf::Vector2f(), static_cast<sf::Vector2f>(window.getSize())), window(window), speed(cameraSpeed) { }

	void connect() {
		keyPressedConnection = game.keyboard.keyPressed.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::W) {
				movement += { 0, -1 };
			}
			else if (key == sf::Keyboard::Key::A) {
				movement += { -1, 0 };
			}
			else if (key == sf::Keyboard::Key::S) {
				movement += { 0, 1 };

			}
			else if (key == sf::Keyboard::Key::D) {
				movement += { 1, 0 };
			}
		});

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::W) {
				movement -= { 0, -1 };
			}
			else if (key == sf::Keyboard::Key::A) {
				movement -= { -1, 0 };

			}
			else if (key == sf::Keyboard::Key::S) {
				movement -= { 0, 1 };

			}
			else if (key == sf::Keyboard::Key::D) {
				movement -= { 1, 0 };
			}
		});
	}

	void disconnect() {
		keyPressedConnection.disconnect();
		keyReleasedConnection.disconnect();
	}

	void setCameraSpeed(float cameraSpeed) {
		speed = cameraSpeed;
	}

	void setPosition(sf::Vector2f pos) {
		position = pos;
	}

	void update(const float elapsedTime) {
		position += static_cast<sf::Vector2f>(movement) * speed * elapsedTime;

		view.setCenter(position);
		window.setView(view);
	}

	void reset() {
		view.setCenter(window.getDefaultView().getCenter());
		window.setView(view);
	}
};