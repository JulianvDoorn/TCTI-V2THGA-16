#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "Rectangle.hpp"

class RectangleContainer : public std::vector<std::shared_ptr<Rectangle>> {
private:
	sf::RenderTarget &window;

public:
	RectangleContainer(sf::RenderTarget& _window) : window(_window) {
		game.mouse.mouseRightButtonDown.connect([this](const sf::Vector2i mousePos) {
			for (auto &rectangle : *this) {
				if (rectangle->getBounds().contains(window.mapPixelToCoords(mousePos))) {
					
					for (auto it = this->begin(); it != this->end(); )
					{
						if (*it == rectangle)
						{
							it = this->erase(it);
							return;
						}
						else
						{
							++it;
						}
					}
				}
			}
		});

		game.keyboard.keyPressed.connect([this](const sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::C) {
				this->clear();
			}
		});
	}

	void add(std::shared_ptr<Rectangle> rect) {
		std::vector<std::shared_ptr<Rectangle>>::push_back(rect);
	}

	/**void drawToCamera(sf::RenderTarget &window, std::shared_ptr<Rectangle> rectangle) {
		sf::Vector2f oldPos = rectangle->getPosition();
		sf::Vector2f cameraPos = window.mapPixelToCoords(static_cast<sf::Vector2i>(rectangle->getPosition()));

		rectangle->setPosition(cameraPos);

		window.draw(*rectangle);
		rectangle->setPosition(oldPos);
	}**/

	void draw() {
		for (auto rectangle : *this) {
			//sf::Vector2f oldPos = rectangle->getPosition();
			//sf::Vector2f cameraPos = rectangle->getPosition();

			//rectangle->setPosition(cameraPos);

			window.draw(*rectangle);
			//rectangle->setPosition(oldPos);
		}
	}
};
