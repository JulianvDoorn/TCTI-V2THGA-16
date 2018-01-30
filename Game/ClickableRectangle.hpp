#pragma once

#include <SFML/Graphics.hpp>

#include "Events.hpp"

class ClickableRectangle : public virtual sf::RectangleShape {
	EventConnection masterMouseLeftButtonDown;
	EventConnection masterMouseLeftButtonUp;
	EventConnection masterMouseRightButtonDown;
	EventConnection masterMouseRightButtonUp;

public:
	EventSource<> mouseLeftButtonDown;
	EventSource<> mouseLeftButtonUp;
	EventSource<> mouseRightButtonDown;
	EventSource<> mouseRightButtonUp;

	void bindMouseEvents() {
		masterMouseLeftButtonDown = game.mouse.mouseLeftButtonDown.connect([this](sf::Vector2i pos) {
			if (getGlobalBounds().contains(game.window->mapPixelToCoords(pos))) {
				mouseLeftButtonDown.fire();
			}
		});

		masterMouseRightButtonDown = game.mouse.mouseRightButtonDown.connect([this](sf::Vector2i pos) {
			if (getGlobalBounds().contains(game.window->mapPixelToCoords(pos))) {
				mouseRightButtonDown.fire();
			}
		});

		masterMouseLeftButtonUp = game.mouse.mouseLeftButtonUp.connect([this](sf::Vector2i pos) {
			if (getGlobalBounds().contains(game.window->mapPixelToCoords(pos))) {
				mouseLeftButtonUp.fire();
			}
		});

		masterMouseRightButtonUp = game.mouse.mouseRightButtonUp.connect([this](sf::Vector2i pos) {
			if (getGlobalBounds().contains(game.window->mapPixelToCoords(pos))) {
				mouseRightButtonUp.fire();
			}
		});
	}

	void unbindMouseEvents() {
		masterMouseLeftButtonDown.disconnect();
		masterMouseLeftButtonUp.disconnect();
		masterMouseRightButtonDown.disconnect();
		masterMouseRightButtonUp.disconnect();
	}
};