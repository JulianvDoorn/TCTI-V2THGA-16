#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEventDecoder.hpp"
#include "EventSource.hpp"

class Mouse : public SFMLEventDecoder {
	sf::RenderWindow& window;

public:
	Mouse(sf::RenderWindow& window) : window(window) { }

	EventSource<> mouseLeftButtonUp;
	EventSource<> mouseRightButtonUp;
	EventSource<> mouseLeftButtonDown;
	EventSource<> mouseRightButtonDown;
	EventSource<sf::Vector2i> mouseMoved;

	void decodeSFMLEvent(sf::Event ev) const override {
		switch (ev.type) {
		case sf::Event::MouseMoved:
			mouseMoved.fire(sf::Mouse::getPosition(window));
			return;
		case sf::Event::MouseButtonPressed:
			if (ev.mouseButton.button == sf::Mouse::Left) {
				mouseLeftButtonDown.fire();
			} else if (ev.mouseButton.button == sf::Mouse::Right) {
				mouseRightButtonDown.fire();
			}
		case sf::Event::MouseButtonReleased:
			if (ev.mouseButton.button == sf::Mouse::Left) {
				mouseLeftButtonUp.fire();
			}	else if (ev.mouseButton.button == sf::Mouse::Right) {
				mouseRightButtonUp.fire();
			}
		}
	}
};