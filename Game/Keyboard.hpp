#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEventDecoder.hpp"
#include "EventSource.hpp"

class Keyboard : public SFMLEventDecoder {
public:
	Keyboard() { }

	EventSource<sf::Keyboard::Key> keyPressed;
	EventSource<sf::Keyboard::Key> keyReleased;

	void decodeSFMLEvent(sf::Event ev) const override {
		switch (ev.type) {
		case sf::Event::KeyPressed:
			keyPressed.fire(ev.key.code);
		case sf::Event::KeyReleased:
			keyReleased.fire(ev.key.code);
		}
	}
};