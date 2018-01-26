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

		game.keyboard.keyPressed.connect([this](const sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Z) {
				if (!this->empty()) {
					this->pop_back();
				}
			}
		});
	}

	void add(std::shared_ptr<Rectangle> rect) {
		std::vector<std::shared_ptr<Rectangle>>::push_back(rect);
	}


	void draw() {
		for (auto rectangle : *this) {
			window.draw(*rectangle);
		}
	}
};
