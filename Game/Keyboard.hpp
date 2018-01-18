#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEventDecoder.hpp"
#include "EventSource.hpp"

class Keyboard : public SFMLEventDecoder {
public:
	Keyboard(sf::Window &window) { window.setKeyRepeatEnabled(false); }

	EventSource<sf::Keyboard::Key> keyPressed;
	EventSource<sf::Keyboard::Key> keyReleased;

	void decodeSFMLEvent(sf::Event ev) const override {
		if (ev.type == sf::Event::KeyPressed) {
			keyPressed.fire(ev.key.code);
		}
		else if (ev.type == sf::Event::KeyReleased) {
			keyReleased.fire(ev.key.code);
		}
	}
};