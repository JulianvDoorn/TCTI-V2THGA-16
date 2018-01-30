#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "PhysicsObject.hpp"
#include "EventSource.hpp"
#include "EventConnection.hpp"
#include "BaseFactory.hpp"
#include "Button.hpp"
#include "ObjectSelector.hpp"

/**
 * @class	Dock
 *
 * @brief	Visible dock for the map editor.
 *
 * @author	Wiebe
 * @date	26-1-2018
 */

class Dock {
private:
	/** @brief	The rectangles templates used for displaying texture dummy's */
	std::vector<std::shared_ptr<Rectangle>> rectanglesTemplates;
	
	/** @brief	The map displayed on the screen */
	Map &map;

	/** @brief	The selected item */
	ObjectSelector& selection;

	/** @brief	The display window */
	sf::RenderTarget &window;

	/** @brief	The dock start x and y coordinates */
	float dockStartX = 25.0f, dockStartY = 630.0f; // 685

	/** @brief	The dock item offset */
	const float dockItemOffset = 55.0f;

	/** @brief	Amount of items on the x axis */
	int xAxisItems = 0;


public:

	/**
	 * @fn	Dock::Dock(RectangleContainer &_rectangles, sf::RenderTarget &_window)
	 *
	 * @brief	Constructor
	 *
	 * @author	Wiebe
	 * @date	26-1-2018
	 *
	 * @param [in,out]	_rectangles	The rectangles.
	 * @param [in,out]	_window	   	The window.
	 */

	Dock(Map &_map, sf::RenderTarget &_window, ObjectSelector& selection) : map(_map), window(_window), selection(selection) {
		game.mouse.mouseLeftButtonDown.connect([this](const sf::Vector2i mousePos) {
			selectRectangle(mousePos);
		});
	}

	/**
	 * @fn	void Dock::addRectangle(std::shared_ptr<Rectangle> r)
	 *
	 * @brief	Adds a rectangle
	 *
	 * @author	Wiebe
	 * @date	26-1-2018
	 *
	 * @param	r	A std::shared_ptr&lt;Rectangle&gt; to process.
	 */

	void addRectangle(std::shared_ptr<Rectangle> r) {
		if (xAxisItems > 22) {
			dockStartY += 55;
			dockStartX = 25;
			xAxisItems = 0;
		}

		r->setPosition({ dockStartX, dockStartY });

		dockStartX += dockItemOffset;

		rectanglesTemplates.push_back(r);
		xAxisItems++;
	}

	/**
	 * @fn	void Dock::selectRectangle(sf::Vector2i mousePos)
	 *
	 * @brief	Select a rectangle
	 *
	 * @author	Wiebe
	 * @date	26-1-2018
	 *
	 * @param	mousePos	The mouse position.
	 */

	void selectRectangle(sf::Vector2i mousePos) {
		for (auto &rectangle : rectanglesTemplates) {
			if (rectangle->getBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
				Rectangle* temp = new Rectangle();

				map.addObject(temp);
				map.addDrawable(temp);

				temp->setPosition(game.window->mapPixelToCoords(mousePos));
				temp->setTexture(*rectangle->getTexture());
				temp->setTextureRect(rectangle->getTextureRect());
				temp->setSize(rectangle->getSize());

				selection.select(temp);

				return;
			}
		}
	}

	/**
	 * @fn	void Dock::draw()
	 *
	 * @brief	Draw the created rectangles.
	 *
	 * @author	Wiebe
	 * @date	26-1-2018
	 */

	void draw() {
		// Set the position of the selected rectangle to the position of the mouse.
		for (auto rectangle : rectanglesTemplates) {
			sf::Vector2f oldPos = rectangle->getPosition();
			sf::Vector2f cameraPos = window.mapPixelToCoords(static_cast<sf::Vector2i>(rectangle->getPosition()));
			
			rectangle->setPosition(cameraPos);

			window.draw(*rectangle);
			rectangle->setPosition(oldPos);
		}
	}
};