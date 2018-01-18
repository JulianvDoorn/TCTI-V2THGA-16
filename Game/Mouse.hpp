#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEventDecoder.hpp"
#include "EventSource.hpp"

class Mouse : public SFMLEventDecoder {
	sf::RenderWindow& window;

public:
	Mouse(sf::RenderWindow& window) : window(window) { }

	EventSource<sf::Vector2i> mouseLeftButtonUp;
	EventSource<sf::Vector2i> mouseRightButtonUp;
	EventSource<sf::Vector2i> mouseLeftButtonDown;
	EventSource<sf::Vector2i> mouseRightButtonDown;
	EventSource<sf::Vector2i> mouseMoved;

	void decodeSFMLEvent(sf::Event ev) const override {
		switch (ev.type) {
		case sf::Event::MouseMoved:
			mouseMoved.fire(sf::Mouse::getPosition(window));
			return;
		case sf::Event::MouseButtonPressed:
			if (ev.mouseButton.button == sf::Mouse::Left) {
				mouseLeftButtonDown.fire(sf::Mouse::getPosition(window));
			} else if (ev.mouseButton.button == sf::Mouse::Right) {
				mouseRightButtonDown.fire(sf::Mouse::getPosition(window));
			}
		case sf::Event::MouseButtonReleased:
			if (ev.mouseButton.button == sf::Mouse::Left) {
				mouseLeftButtonUp.fire(sf::Mouse::getPosition(window));
			}	else if (ev.mouseButton.button == sf::Mouse::Right) {
				mouseRightButtonUp.fire(sf::Mouse::getPosition(window));
			}
		}
	}
};