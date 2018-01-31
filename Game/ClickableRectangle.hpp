#pragma once

#include <SFML/Graphics.hpp>

#include "Events.hpp"

/**
 * @class	ClickableRectangle
 *
 * @brief	A clickable rectangle.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class ClickableRectangle : public virtual sf::RectangleShape {
	/** @brief	The master mouse left button down */
	EventConnection masterMouseLeftButtonDown;
	/** @brief	The master mouse left button up */
	EventConnection masterMouseLeftButtonUp;
	/** @brief	The master mouse right button down */
	EventConnection masterMouseRightButtonDown;
	/** @brief	The master mouse right button up */
	EventConnection masterMouseRightButtonUp;

public:
	/** @brief	The mouse left button down */
	EventSource<> mouseLeftButtonDown;
	/** @brief	The mouse left button up */
	EventSource<> mouseLeftButtonUp;
	/** @brief	The mouse right button down */
	EventSource<> mouseRightButtonDown;
	/** @brief	The mouse right button up */
	EventSource<> mouseRightButtonUp;

	/**
	 * @fn	void ClickableRectangle::bindMouseEvents()
	 *
	 * @brief	Bind mouse events
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

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

	/**
	 * @fn	void ClickableRectangle::unbindMouseEvents()
	 *
	 * @brief	Unbind mouse events
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	void unbindMouseEvents() {
		masterMouseLeftButtonDown.disconnect();
		masterMouseLeftButtonUp.disconnect();
		masterMouseRightButtonDown.disconnect();
		masterMouseRightButtonUp.disconnect();
	}
};