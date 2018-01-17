#pragma once

#include "KeyCodeArguments.hpp"
#include "EventSignal.hpp"

class KeyDownSignal : public EventSignal {
	KeyCodeArguments keyDownArguments;

public:
	KeyDownSignal(sf::Keyboard::Key key) : keyDownArguments(key) { }

	const EventArguments& getArguments() const override {
		return keyDownArguments;
	}
};