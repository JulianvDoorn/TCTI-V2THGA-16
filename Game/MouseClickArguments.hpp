#pragma once

#include <SFML/Graphics.hpp>
#include "EventSignal.hpp"

class MouseClickArguments : public EventArguments {
public:
	sf::Vector2i position;

	MouseClickArguments(sf::Vector2i position) : position(position) { }
	MouseClickArguments(const EventArguments& arguments) : position(static_cast<const MouseClickArguments&>(arguments).position) { }
};