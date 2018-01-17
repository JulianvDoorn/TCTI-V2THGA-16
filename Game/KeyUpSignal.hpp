#pragma once

#include "KeyCodeArguments.hpp"
#include "EventSignal.hpp"

class KeyUpSignal : public EventSignal {
	KeyCodeArguments keyUpArguments;

public:
	KeyUpSignal(sf::Keyboard::Key key) : keyUpArguments(key) { }

	const EventArguments& getArguments() const override {
		return keyUpArguments;
	}
};