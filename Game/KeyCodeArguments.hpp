#pragma once

#include <SFML/Graphics.hpp>
#include "EventSignal.hpp"

class KeyCodeArguments : public EventArguments {
public:
	sf::Keyboard::Key key;

	KeyCodeArguments(sf::Keyboard::Key key) : key(key) { }
	KeyCodeArguments(const EventArguments& arguments) : key(static_cast<const KeyCodeArguments&>(arguments).key) { }
};