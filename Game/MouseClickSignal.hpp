#pragma once

#include <SFML\Graphics.hpp>
#include "MouseClickArguments.hpp"
#include "EventSignal.hpp"

class MouseClickSignal : public EventSignal {
	MouseClickArguments mouseClickArguments;

public:
	MouseClickSignal(sf::Vector2i position) : mouseClickArguments(position) { }

	const EventArguments& getArguments() const override {
		return mouseClickArguments;
	}
};