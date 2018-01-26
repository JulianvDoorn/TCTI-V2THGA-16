#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "PhysicsObject.hpp"
#include "EventSource.hpp"
#include "EventConnection.hpp"
#include "BaseFactory.hpp"
#include "RectangleContainer.hpp"
#include "Button.hpp"

class Dock {
private:
	std::vector<std::shared_ptr<Rectangle>> rectanglesTemplates;
	
	bool isRectangleSelected = false;
	
	RectangleContainer &rectangles;
	std::unique_ptr<Rectangle> selectedRect;
	Button moveDockLeftBtn, moveDockRightBtn;

	sf::RenderTarget &window;

	float dockStartX = 25.0f, dockStartY = 685.0f;
	const float dockItemOffset = 55.0f;


public:
	Dock(RectangleContainer &_rectangles, sf::RenderTarget &_window) : rectangles(_rectangles), window(_window) {
		game.mouse.mouseLeftButtonDown.connect([this](const sf::Vector2i mousePos) {
			selectRectangle(mousePos);
		});

		game.mouse.mouseLeftButtonUp.connect([this](const sf::Vector2i mousePos) {
			if (isRectangleSelected) {
				selectedRect->setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(selectedRect->getPosition())));
				rectangles.add(std::move(selectedRect));

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

	void addRectangle(std::shared_ptr<Rectangle> r) {
		if (rectanglesTemplates.size() > 50) {
			throw "Unable to add more rectangles to the dock!";
		}

		r->setPosition({ dockStartX, dockStartY });

		dockStartX += dockItemOffset;

		rectanglesTemplates.push_back(r);
	}

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