#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "PhysicsObject.hpp"
#include "EventSource.hpp"
#include "EventConnection.hpp"
#include "BaseFactory.hpp"
#include "RectangleContainer.hpp"
#include "Button.hpp"

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
	
	/** @brief	True if an rectangle is selected, false if not */
	bool isRectangleSelected = false;
	
	/** @brief	The drag-and-dropped rectangles */
	//RectangleContainer &rectangles;
	Map &map;

	/** @brief	The selected rectangle as an unique pointer */
	std::unique_ptr<Rectangle> selectedRect;

	/**
	 * @property	Button moveDockLeftBtn, moveDockRightBtn
	 *
	 * @brief	Moves the dock to the left and the right.
	 *
	 * @return	The move dock right button.
	 */

	Button moveDockLeftBtn, moveDockRightBtn;

	/** @brief	The display window */
	sf::RenderTarget &window;

	/** @brief	The dock start x and y coordinates */
	float dockStartX = 25.0f, dockStartY = 685.0f;

	/** @brief	The dock item offset */
	const float dockItemOffset = 55.0f;


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

	Dock(Map &_map, sf::RenderTarget &_window) : map(_map), window(_window) {
		game.mouse.mouseLeftButtonDown.connect([this](const sf::Vector2i mousePos) {
			selectRectangle(mousePos);
		});

		game.mouse.mouseLeftButtonUp.connect([this](const sf::Vector2i mousePos) {
			if (isRectangleSelected) {
				selectedRect->setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(selectedRect->getPosition())));
				map.addDrawable(std::move(selectedRect));

				selectedRect.reset();
				isRectangleSelected = false;
			}
		});

		game.mouse.mouseMoved.connect([this](const sf::Vector2i mousePos) {
			if (isRectangleSelected) {
				selectedRect->setPosition(static_cast<sf::Vector2f>(mousePos));
			}
		});

		game.mouse.mouseRightButtonDown.connect([this](const sf::Vector2i mousePos) {
			if (isRectangleSelected) {
				selectedRect.reset();
				isRectangleSelected = false;
			}
		});

		moveDockLeftBtn.setPosition({ dockStartX, dockStartY });
		moveDockLeftBtn.setText("<");
		moveDockLeftBtn.setSize({ 40, 40 });
		moveDockLeftBtn.setBackgroundColor(sf::Color::Black);
		moveDockLeftBtn.setCharSize(18);

		dockStartX += 50;

		moveDockRightBtn.setText(">");
		moveDockRightBtn.setSize({ 40, 40 });
		moveDockRightBtn.setBackgroundColor(sf::Color::Black);
		moveDockRightBtn.setCharSize(18);

		
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
		if (rectanglesTemplates.size() > 50) {
			throw "Unable to add more rectangles to the dock!";
		}

		r->setPosition({ dockStartX, dockStartY });

		dockStartX += dockItemOffset;

		rectanglesTemplates.push_back(r);
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
				std::unique_ptr<Rectangle> temp = std::make_unique<Rectangle>();

				temp->setPosition(rectangle->getPosition());
				temp->setTexture(*rectangle->getTexture());
				temp->setTextureRect(rectangle->getTextureRect());
				temp->setSize(rectangle->getSize());

				selectedRect = std::move(temp);

				isRectangleSelected = true;

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
		sf::Vector2f oldPos = moveDockLeftBtn.getPosition();
		moveDockLeftBtn.setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(moveDockLeftBtn.getPosition())));

		moveDockLeftBtn.draw(window);
		moveDockLeftBtn.setPosition(oldPos);

	
		// Set the position of the selected rectangle to the position of the mouse.
		for (auto rectangle : rectanglesTemplates) {
			sf::Vector2f oldPos = rectangle->getPosition();
			sf::Vector2f cameraPos = window.mapPixelToCoords(static_cast<sf::Vector2i>(rectangle->getPosition()));
			
			rectangle->setPosition(cameraPos);

			window.draw(*rectangle);
			rectangle->setPosition(oldPos);

		}

		if (selectedRect && isRectangleSelected) {
			sf::Vector2f oldPos = selectedRect->getPosition();
			sf::Vector2f cameraPos = window.mapPixelToCoords(static_cast<sf::Vector2i>(selectedRect->getPosition()));

			selectedRect->setPosition(cameraPos);

			window.draw(*selectedRect);
			selectedRect->setPosition(oldPos);
		}

		moveDockRightBtn.setPosition({ dockStartX, dockStartY });
		oldPos = moveDockRightBtn.getPosition();
		moveDockRightBtn.setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(moveDockRightBtn.getPosition())));

		moveDockRightBtn.draw(window);
		moveDockRightBtn.setPosition(oldPos);

	}
};